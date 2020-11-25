#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define COLUMNS 42
#define ROWS 22

#include <QMainWindow>
#include <qcustomplot.h>

namespace Ui {
class MainWindow;
}

enum DACs{VanaN, VanaP, Vdig, VDDIO, V18, BiasD, BiasR, VcascN, Vn0, Vn1, Vn2,
          Vfb, Vprfb, VcascP, Vp0, Vp1, Vp2, Vcal, Hold, IBiasRO, IBiasIO, VOffset};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setupColorMap(QCustomPlot *customPlot);
    void Init();

private slots:
    void on_clear_pressed();

    void on_unzoom_x_clicked();

    void on_unzoom_y_clicked();

    void on_image_clicked();

    void on_darkfield_clicked();

    void on_loop_clicked();

    void onTimeout();
    void on_columnwise_clicked();

    void on_rowwise_clicked();

    void DACScan(int DAC, int start, int stop, int step);

private:
    void GetFrame();
    void SetDAC(int DAC, int value);


private:
    int nx;
    int ny;
    bool dark;
    bool running;
    QCPColorMap *colorMap;
    QCustomPlot *customPlot;
    Ui::MainWindow *ui;
    std::vector<double> darkfield;
    std::vector<uint16_t> data;
    QTimer *timer;
    bool columnwise;

};

class MyQApplication : QApplication
{
public:
    MyQApplication(int& argc, char** argv) : QApplication(argc, argv){};
    void Show(){
        w.show();
        w.activateWindow();
        w.raise();
        exec();
    };
    void Init(){ w.Init();};
private:
    MainWindow w;
};

#endif // MAINWINDOW_H
