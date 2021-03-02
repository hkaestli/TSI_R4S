#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "command.h"

#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

extern CInterpreter cmd_intp;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), dark(false), loop_running(false), take_data_running(false),
    ui(new Ui::MainWindow)
{
    cfg.Init();
    ui->setupUi(this);
    ui->configWidget->setModel(&cfg);
    setupColorMap(ui->customPlot);
    darkfield.resize(COLUMNS*ROWS,0.0);
    data.resize(COLUMNS*ROWS,0);
    loopTimer = new QTimer(this);
    takeDataTimer = new QTimer(this);
    columnwise=true;
    triggerSource = 0;
}

MainWindow::~MainWindow()
{
    delete loopTimer;
    delete ui;
}

void MainWindow::Init()
{
   ui->average->Init(1, 1, 100, "Average","Frames");

   connect(loopTimer, &QTimer::timeout, this, &MainWindow::onLoopTimeout);
   connect(takeDataTimer, &QTimer::timeout, this, &MainWindow::onTakeDataTimeout);
   connect(ui->configWidget->tabWidget, &QTabWidget::currentChanged, this, &MainWindow::on_tab_currentChanged);
   connect(ui->configWidget->loadScript, &QPushButton::clicked, this, &MainWindow::on_loadScript_clicked);
   on_columnwise_clicked();
   on_internalCal_clicked();
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

//======================================================
// the button marked "single"
// is like "loop" with just one loop made
void MainWindow::on_image_clicked()
{
    if(loop_running){
        printf("Stop loop before clicking single\n");
    }else if (take_data_running) {
        printf("Stop data taking before clicking single\n");
    }else{
        onLoopTimeout();
    }
}

//======================================================
// loop related


void MainWindow::on_loop_clicked()
{

    if (take_data_running){
        // in case "take data" is running just ignore the click
        printf("Stop take data before starting a loop\n");
    }else{
        if(loop_running){
            loop_running=false;
            ui->loop->setText("Loop");
            ui->loop->setChecked(false);
            loopTimer->stop();
        }else {
            loop_running=true;
            ui->loop->setText("Stop");
            ui->loop->setChecked(true);
            loopTimer->start(200);
        }
    }
}

void MainWindow::onLoopTimeout()
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
    // braucht es nur einmal pro run
    uint8_t roMode = 3;
    tb.Daq_Open(50000); // Anzahl pixel: 42 x 22 = 924 waeren genug

    // prepare ADC
    tb.r4s_AdcDelay(7);
    tb.r4s_Enable(roMode);
    tb.uDelay(400);

    // bei jedem event
    // take data
    tb.Daq_Start(); // macht adcs scharf
    tb.r4s_Start(); // startet chip sequence
    tb.uDelay(3000);

    tb.Daq_Stop();

    // stop ADC
    tb.r4s_Enable(0);

    tb.Daq_Read(data);// liest daten von dtb -> PC

    // nur 1x pro run
    tb.Daq_Close();
    tb.Flush();
}

//======================================================
// take data related

void MainWindow::on_takeData_clicked(){

    if (loop_running){
        printf("Stop loop before pressing take data\n");
    }else{

        if(take_data_running){
            // end takeData
            printf("Stop data taking clicked. Events taken: %d \n", eventCounter);
            endTakeData();
        }else {
            take_data_running=true;
            ui->takeData->setText("Stop");
            ui->takeData->setChecked(true);

            eventCounter = 0;
            int n = ui->numberOfEvents->value();
            printf("Take Data clicked. Start data taking of %d events.\n", n);

            // open file
            fileOut = new TFile("output.root","recreate");
            eventTree = new TTree("eventTree","eventTree");
            eventTree->Branch("eventBranch",ana, "ana[42][22]/I");
            // using the definitions COLUMS and ROWS compiles but is considered as "variable" definition at run time and crashes

            // prepare dtb for data taking
            uint8_t roMode = 3;
            tb.Daq_Open(50000); // Anzahl pixel: 42 x 22 = 924 waeren genug

            // prepare ADC
            tb.r4s_AdcDelay(7);
            tb.r4s_Enable(roMode);
            //tb.uDelay(400);
            // start timer to trigger a readout sequence once every 200ms
            takeDataTimer->start(200);
        }
    }
}


void MainWindow::onTakeDataTimeout()
{

    // check if r4s sequence is still running
    if (tb.r4s_Running()){
        printf("timeout triggered while seq is still running\n");
        tb.r4s_Stop();
    }
    // read data from last event
    tb.Daq_Stop();

    // stop ADC
    tb.r4s_Enable(0);
    tb.Daq_Read(data);// copies data form dtb to the PC

    // close daq
    tb.Daq_Close();
    tb.Flush();

    // check if there is data
    if (data.size() == 924){

        // one should have put the correct expected datalenth here (22 x 42 = 924)
        // but we assume if there is data it is correct

        // put sequencial data to array
        int x, y;
        if(columnwise){
            for (x = 0; x < COLUMNS; x++){
                for (y = 0; y < ROWS; y++) {
                    int value = (int) data[y+ROWS*x];
                    if ((value & 0x1000) != 0) // ADC overrange
                        value = -5000;
                    else if ((value & 0x0800) != 0) // negative
                        value -= 0x1000;
                    ana[x][y] = value;
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
                    ana[x][y] = value;
                    if(x<nx && y<ny) colorMap->data()->setCell(x, y, (double) value - darkfield[y+ROWS*x] );
                }
            }
        }

        eventTree->Fill();

        eventCounter++;

        if ((eventCounter%10)==0){
            printf("Events taken: %d \n", eventCounter);
            customPlot->replot();
        }

    }else{
        printf("data.size() = %d \n", (int)data.size());
    }

    if ((eventCounter+1) >  ui->numberOfEvents->value()  ){

        printf("Stop data taking after recording %d  events\n", eventCounter);
        endTakeData();
    }

    // start take data sequence for next event

    uint8_t roMode = 3;
    tb.Daq_Open(50000); // Anzahl pixel: 42 x 22 = 924 waeren genug

    // prepare ADC
    tb.r4s_AdcDelay(7);
    tb.r4s_Enable(roMode);
    tb.uDelay(400);

    tb.Daq_Start(); // macht adcs scharf
    tb.r4s_Start(); // startet chip sequence
    //tb.uDelay(3000)
}

void MainWindow::endTakeData(){

    take_data_running=false;
    ui->takeData->setText("Take Data");
    ui->takeData->setChecked(false);
    takeDataTimer->stop();

    // close Daq
    tb.r4s_Stop();
    tb.Daq_Close();
    tb.Flush();
    eventCounter = 0;

    eventTree->Write();
    delete eventTree;

    // close file, delete tree
    fileOut->Close();
    delete fileOut;

}

// =============================================

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
    onLoopTimeout();
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


void MainWindow::on_internalCal_clicked()
{
    printf("Switching to internal calibrate\n");
    triggerSource = 0;
    //tb.r4s_SetSeq.....();
}

void MainWindow::on_laser_clicked()
{
    printf("Laser inf not yet implemented\n");
    triggerSource = 1;
    //tb.r4s_SetSeq.....();
}

void MainWindow::on_externalCal_clicked()
{
    printf("Source trigger not yet implemented\n");
    triggerSource = 2;
    //tb.r4s_SetSeq.....();
}

void MainWindow::on_tab_currentChanged(int index)
{
    cfg.Activate(index);
}

void MainWindow::on_loadScript_clicked()
{
    QString script=QString("script/")+ui->configWidget->getScript()+".roc";
    FILE *cf = fopen(script.toStdString().c_str(), "rt");
    if (cf) {
        cmd_intp.run(cf, 1);
        fclose(cf);
    }
}

void MainWindow::on_full_stateChanged(int arg1)
{
    if(arg1){
        ui->col->hide();
        ui->row->hide();
        ui->label_col->hide();
        ui->label_row->hide();
    } else {
        ui->col->show();
        ui->row->show();
        ui->label_col->show();
        ui->label_row->show();
    }
}

void MainWindow::on_full_2_stateChanged(int arg1)
{
    if(arg1){
        ui->col_2->hide();
        ui->row_2->hide();
        ui->label_col_2->hide();
        ui->label_row_2->hide();
    } else {
        ui->col_2->show();
        ui->row_2->show();
        ui->label_col_2->show();
        ui->label_row_2->show();
    }
}

void MainWindow::on_dacscan_clicked()
{
    bool ok;
    CDictionary dict;
    int DAC=dict.Id(ui->DAC->text());
    int start=ui->min->text().toInt(&ok);
    int stop=ui->max->text().toInt(&ok);
    int step=ui->step->text().toInt(&ok);
    QString filename=ui->dir->text();
    QDir dir;
    dir.mkpath(filename);
    filename+="/";
    filename+=ui->filename->text();
    TFile *rootFile = new TFile(filename.toStdString().c_str(),"recreate");
    std::map<int,double> result;
    int col_min=ui->col->text().toInt();
    int row_min=ui->row->text().toInt();
    int col_max, row_max;
    if(ui->full->isChecked()){
        col_min=0;
        col_max=39;
        row_min=0;
        row_max=19;
    } else{
        col_max=col_min;
        row_max=row_min;
    }
    TH1D* hists[40*20];
    for(int col=col_min; col<=col_max; col++){
        for(int row=row_min; row<=row_max; row++){
            cfg.Activate(col, row);
            tb.SetPixCal(col,row);
            result.clear();
            tb.DACScan(DAC, start, stop, step, result);

            hists[col*20+row]=new TH1D(Form("ph_c%02d_r%02d",col,row),Form("%s Scan Column %2d Row %2d",ui->plotname->text().toStdString().c_str(), col,row), (stop-start)/step , (double) start, (double) stop);
            for(int i=start; i<=stop; i+=step)
            {
               hists[col*20+row]->Fill((double) i+step/2, result[i]);
            }
        }
    }
    rootFile->Write();
    rootFile->Close();
    delete rootFile;
}


void MainWindow::on_dacscan_2_clicked()
{
    bool ok;
     CDictionary dict;
     int DAC1=dict.Id(ui->DAC_21->text());
     int start1=ui->min_21->text().toInt(&ok);
     int stop1=ui->max_21->text().toInt(&ok);
     int step1=ui->step_21->text().toInt(&ok);
     int DAC2=dict.Id(ui->DAC_22->text());
     int start2=ui->min_22->text().toInt(&ok);
     int stop2=ui->max_22->text().toInt(&ok);
     int step2=ui->step_22->text().toInt(&ok);
     QString filename=ui->dir->text();
     QDir dir;
     dir.mkpath(filename);
     filename+="/";
     filename+=ui->filename->text();
     TFile *rootFile = new TFile(filename.toStdString().c_str(),"recreate");
     std::vector<double> result;
     int col_min=ui->col_2->text().toInt();
     int row_min=ui->row_2->text().toInt();
     int col_max, row_max;
     if(ui->full_2->isChecked()){
         col_min=0;
         col_max=39;
         row_min=0;
         row_max=19;
     } else{
         col_max=col_min;
         row_max=row_min;
     }
     TH2D* hists[40*20];
     int nbins1=(stop1-start1)/step1+1;
     int nbins2=(stop2-start2)/step2+1;
     for(int col=col_min; col<=col_max; col++){
         for(int row=row_min; row<=row_max; row++){
             cfg.Activate(col, row);
             tb.SetPixCal(col,row);
             result.clear();
             tb.DACDACScan(DAC1, start1, stop1, step1, DAC2, start2, stop2, step2, result);
             hists[col*20+row]=new TH2D(Form("ph_c%02d_r%02d",col,row),Form("%s Scan Column %2d Row %2d",ui->plotname->text().toStdString().c_str(), col,row),
                                        nbins1 , (double) start1-step1/2, (double) stop1+step1/2,
                                        nbins2 , (double) start2-step2/2, (double) stop2+step2/2);
             int a=0;
             int b=0;
             for(int i=start1; i<=stop1; i+=step1) {
                 a=0;
                 for(int j=start2; j<=stop2; j+=step2) {
                     hists[col*20+row]->Fill((double) i, (double) j, result[b+a*nbins2]);
                     b++;
                 }
                 a++;
             }
         }
     }
     rootFile->Write();
     rootFile->Close();
     delete rootFile;

}
