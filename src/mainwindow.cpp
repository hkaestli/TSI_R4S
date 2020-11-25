#include "../include/mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), dark(false), running(false),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupColorMap(ui->customPlot);
    darkfield.resize(COLUMNS*ROWS,0.0);
    data.resize(COLUMNS*ROWS,0);
    timer = new QTimer(this);
    columnwise=true;
}

MainWindow::~MainWindow()
{
    delete timer;
    delete ui;
}

void MainWindow::Init()
{

   ui->vana_n->Init(&CTestboard::r4s_SetVana_n, 1800, 0, 1800, "Vana N");
   ui->vana_p->Init(&CTestboard::r4s_SetVana_p, 1250, 0, 1800, "Vana P");
   ui->vd->Init(&CTestboard::r4s_SetVdig, 1800, 0, 2000, "Vdig");
   ui->vddio->Init(&CTestboard::r4s_SetVddio, 1800, 0, 1800, "VDDIO");
   ui->v18->Init(&CTestboard::r4s_SetV18, 1800, 0, 1800, "V18");
   ui->biasd->Init(&CTestboard::r4s_SetVbias_d, 2400, 0, 2400, "Bias D");
   ui->biasr->Init(&CTestboard::r4s_SetVbias_r, 1800, 0, 1800, "Bias R");
   ui->vcascn->Init(&CTestboard::r4s_SetVcasc_n, 800, 0, 1800, "Vcasc N");
   ui->vn0->Init(&CTestboard::r4s_SetVn0, 740, 0, 1800, "Vn0");
   ui->vn1->Init(&CTestboard::r4s_SetVn1, 730, 0, 1800, "Vn1");
   ui->vn2->Init(&CTestboard::r4s_SetVn2, 750, 0, 1800, "Vn2");
   ui->vfb->Init(&CTestboard::r4s_SetVfb, 1100, 0, 1800, "Vfb");
   ui->vprfb->Init(&CTestboard::r4s_SetVprefb, 700, 0, 1800, "Vprfb");
   ui->vcascp->Init(&CTestboard::r4s_SetVcasc_p, 1100, 0, 1800, "Vcasc P");
   ui->vp0->Init(&CTestboard::r4s_SetVp0, 820, 0, 1800, "Vp0");
   ui->vp1->Init(&CTestboard::r4s_SetVp1, 725, 0, 1800, "Vp1");
   ui->vp2->Init(&CTestboard::r4s_SetVp2, 685, 0, 1800, "Vp2");

   ui->vcal->Init(&CTestboard::r4s_SetVcal, 1700, 0, 1800, "Vcal");
   ui->vhold->Init(&CTestboard::r4s_SetMeasureHold, 2610, 1500, 3000, "t hold","a/u");

   ui->biasro->Init(&CTestboard::r4s_SetIbiasro, 525, 0, 2000, "IBias ro");
   ui->biasio->Init(&CTestboard::r4s_SetIbiasio, 370, 0, 2000, "IBias I/O");
   ui->offset->Init(&CTestboard::r4s_SetVoffset, 460, 0, 1800, "VOffset");

   ui->average->Init(1, 1, 100, "Average","Frames");

   connect(timer, &QTimer::timeout, this, &MainWindow::onTimeout);

   ui->columnwise->setChecked(true);
   on_columnwise_clicked();
}

void MainWindow::setupColorMap(QCustomPlot *cP)
{
  nx = COLUMNS-2;
  ny = ROWS-2;
  customPlot=cP;
  // configure axis rect:
  customPlot->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom); // this will also allow rescaling the color scale by dragging/zooming
  customPlot->axisRect()->setupFullAxesBox(true);

  // set up the QCPColorMap:
  colorMap = new QCPColorMap(customPlot->xAxis, customPlot->yAxis);
  colorMap->data()->setSize(nx, ny); // we want the color map to have nx * ny data points
  colorMap->data()->setRange(QCPRange(0.0, ((double)nx-1.0)), QCPRange(0.0, ((double) ny-1.0)));
  colorMap->setTightBoundary(false);
  // clear histogram
  for (int xIndex=0; xIndex<nx; ++xIndex)
  {
    for (int yIndex=0; yIndex<ny; ++yIndex)
    {
      colorMap->data()->setCell(xIndex, yIndex, 0);
    }
  }

  // add a color scale:
  QCPColorScale *colorScale = new QCPColorScale(customPlot);
  customPlot->plotLayout()->addElement(0, 1, colorScale); // add it to the right of the main axis rect
  colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
  colorMap->setColorScale(colorScale); // associate the color map with the color scale

  // set the color gradient of the color map to one of the presets:
  colorMap->setGradient(QCPColorGradient::gpPolar);
  // we could have also created a QCPColorGradient instance and added own colors to
  // the gradient, see the documentation of QCPColorGradient for what's possible.

  // rescale the data dimension (color) such that all data points lie in the span visualized by the color gradient:
  colorMap->rescaleDataRange();
  colorMap->setInterpolate(false);

  // make sure the axis rect and color scale synchronize their bottom and top margins (so they line up):
  QCPMarginGroup *marginGroup = new QCPMarginGroup(customPlot);
  customPlot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
  colorScale->setDataRange(QCPRange(-1024.0, 1024.0));

  // rescale the key (x) and value (y) axes so the whole color map is visible:
  customPlot->xAxis->setRange(-0.5, (double)nx -0.5);
  customPlot->yAxis->setRange(-0.5, (double)ny -0.5);
}


void MainWindow::on_clear_pressed()
{
    for (int xIndex=0; xIndex<nx; ++xIndex)
    {
      for (int yIndex=0; yIndex<ny; ++yIndex)
      {
        colorMap->data()->setCell(xIndex, yIndex, 0);
      }
    }
    customPlot->replot();
}

void MainWindow::on_unzoom_x_clicked()
{
   customPlot->xAxis->setRange(-0.5, (double)nx -0.5);
   customPlot->replot();
}

void MainWindow::on_unzoom_y_clicked()
{
    customPlot->yAxis->setRange(-0.5, (double)ny -0.5);
    customPlot->replot();
}

void MainWindow::on_image_clicked()
{
    if(running){
        running=false;
        ui->loop->setText("Loop");
        ui->loop->setChecked(false);
        timer->stop();
    }
    DACScan(Hold,2500,2550,1);
 //   onTimeout();
}

void MainWindow::onTimeout()
{
    GetFrame();

    int x, y;
    if(columnwise){
       for (x = 0; x < COLUMNS; x++){
          for (y = 0; y < ROWS; y++) {
              int value = (int) data[y+ROWS*x];
              if ((value & 0x1000) != 0) // ADC overrange
                  value = -5000;
              else if ((value & 0x0800) != 0) // negative
                  value -= 0x1000;
              if(x<nx && y<ny) colorMap->data()->setCell(x, y, (double) value - darkfield[y+ROWS*x] );
           }
       }
    } else {
       for (y = 0; y < ROWS; y++) {
          for (x = 0; x < COLUMNS; x++){
              int value = (int) data[x+COLUMNS*y];
              if ((value & 0x1000) != 0) // ADC overrange
                  value = -5000;
              else if ((value & 0x0800) != 0) // negative
                  value -= 0x1000;
              if(x<nx && y<ny) colorMap->data()->setCell(x, y, (double) value - darkfield[y+ROWS*x] );
           }
       }
    }
    customPlot->replot();
}

void MainWindow::GetFrame()
{
    uint8_t roMode = 3;
    tb.Daq_Open(50000);

    // prepare ADC
    tb.SignalProbeADC(PROBEA_SDATA1, GAIN_1);
    tb.r4s_AdcDelay(7);
    tb.r4s_Enable(roMode);
    tb.uDelay(400);

    // take data
    tb.Daq_Start();
    tb.r4s_Start();
    tb.uDelay(3000);
    tb.Daq_Stop();

    // stop ADC
    tb.r4s_Enable(0);

    tb.Daq_Read(data);
    tb.Daq_Close();
    tb.Flush();
}

void MainWindow::on_darkfield_clicked()
{
    if(dark){
       dark=false;
       ui->darkfield->setText("Set");
       ui->darkfield->setChecked(false);
       for(unsigned int i=0; i<darkfield.size(); i++) darkfield[i]=0.0;
    }else {
       dark=true;
       ui->darkfield->setText("Clear");
       ui->darkfield->setChecked(true);
       GetFrame();
       if(columnwise){
          for(unsigned int i=0; i<darkfield.size(); i++) {
              int value = (int) data[i];
              if ((value & 0x1000) != 0) // ADC overrange
                     value = -5000;
              else if ((value & 0x0800) != 0) // negative
                      value -= 0x1000;
              darkfield[i]=(double) value;
          }
       } else {
          for (int y = 0; y < ROWS; y++) {
             for (int x = 0; x < COLUMNS; x++){
                 int value = (int) data[x+COLUMNS*y];
                 if ((value & 0x1000) != 0) // ADC overrange
                      value = -5000;
                 else if ((value & 0x0800) != 0) // negative
                      value -= 0x1000;
                 darkfield[y+ROWS*x] = (double) value;
             }
          }
       }
    }
    onTimeout();
}

void MainWindow::on_loop_clicked()
{
    if(running){
       running=false;
       ui->loop->setText("Loop");
       ui->loop->setChecked(false);
       timer->stop();
    }else {
       running=true;
       ui->loop->setText("Stop");
       ui->loop->setChecked(true);
       timer->start(200);
    }
}

void MainWindow::on_columnwise_clicked()
{
    columnwise=true;
    tb.r4s_SetSeqMeasureColumnReadout();
    printf("Switching to columnwise readout.\n");
}

void MainWindow::on_rowwise_clicked()
{
    columnwise=false;
    tb.r4s_SetSeqMeasureReadout();
    printf("Switching to rowwise readout.\n");
}

void MainWindow::DACScan(int DAC, int start, int stop, int step)
{
    // programm pixel
    tb.SignalProbeADC(PROBEA_SDATA1, GAIN_1);
    vector<uint32_t> prog(1);
    prog[ 0] = 0x054321;
    tb.r4s_SetSequence(prog);
    tb.r4s_Start();
    tb.uDelay(3000);
    tb.Flush();

    // DAC scan
    uint8_t roMode = 3;
    tb.Daq_Open(50000);

    // prepare ADC
    tb.r4s_AdcDelay(7);
    tb.r4s_Enable(roMode);
    tb.uDelay(400);

    tb.r4s_SetSeqMeasureValue();
    tb.Daq_Start();

    for(int i=start; i<stop; i+=step)
    {
        SetDAC(DAC, i);
        // take data
        tb.r4s_Start();
        tb.uDelay(3000);
        tb.Flush();
    }

    tb.Daq_Stop();
    // stop ADC
    tb.r4s_Enable(0);
    tb.Daq_Read(data);
    tb.Daq_Close();
    tb.Flush();

    int n=0;
    for(int i=start; i<stop; i+=step)
    {
        double mean=0;
        for (int j=0; j<10; j++) {
            int value = (int) data[10*n+j];
            if ((value & 0x1000) != 0) // ADC overrange
                   value = -5000;
            else if ((value & 0x0800) != 0) // negative
                    value -= 0x1000;
            mean+=(double)value;
        }
        mean=mean/10.0;
        printf("%d : %f\n",i,mean);
        n++;
    }
    if(columnwise) tb.r4s_SetSeqMeasureColumnReadout();
    else tb.r4s_SetSeqMeasureReadout();
}

void MainWindow::SetDAC(int DAC, int value)
{
    switch(DAC){
       case VanaN:
          tb.r4s_SetVana_n((uint16_t) value);
          break;
       case VanaP:
          tb.r4s_SetVana_p((uint16_t) value);
          break;
       case Vdig:
          tb.r4s_SetVdig((uint16_t) value);
          break;
       case VDDIO:
          tb.r4s_SetVddio((uint16_t) value);
          break;
       case V18:
          tb.r4s_SetV18((uint16_t) value);
          break;
       case BiasD:
          tb.r4s_SetVbias_d((uint16_t) value);
          break;
       case BiasR:
          tb.r4s_SetVbias_r((uint16_t) value);
          break;
    case VcascN:
       tb.r4s_SetVcasc_n((uint16_t) value);
       break;
    case Vn0:
       tb.r4s_SetVn0((uint16_t) value);
       break;
    case Vn1:
       tb.r4s_SetVn1((uint16_t) value);
       break;
    case Vn2:
       tb.r4s_SetVn2((uint16_t) value);
       break;
    case Vfb:
       tb.r4s_SetVfb((uint16_t) value);
       break;
    case Vprfb:
       tb.r4s_SetVprefb((uint16_t) value);
       break;
    case VcascP:
       tb.r4s_SetVcasc_p((uint16_t) value);
       break;
    case Vp0:
       tb.r4s_SetVp0((uint16_t) value);
       break;
    case Vp1:
       tb.r4s_SetVp1((uint16_t) value);
       break;
    case Vp2:
       tb.r4s_SetVp2((uint16_t) value);
       break;
    case Vcal:
       tb.r4s_SetVcal((uint16_t) value);
       break;
    case Hold:
       tb.r4s_SetMeasureHold((uint16_t) value);
       break;
    case IBiasRO:
       tb.r4s_SetIbiasro((uint16_t) value);
       break;
    case IBiasIO:
       tb.r4s_SetIbiasio((uint16_t) value);
       break;
    case VOffset:
       tb.r4s_SetVoffset((uint16_t) value);
       break;
    }
}
