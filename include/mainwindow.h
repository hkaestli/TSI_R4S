#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define COLUMNS 42
#define ROWS 22

#include <QMainWindow>
#include <qcustomplot.h>

#include <TFile.h>
#include <TTree.h>

#include "configuration.h"
#include "dictionary.h"

extern CConfiguration cfg;

namespace Ui {
class MainWindow;
}

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

    void on_takeData_clicked();

    void onLoopTimeout();
    void onTakeDataTimeout();

    void on_columnwise_clicked();

    void on_rowwise_clicked();

    void on_internalCal_clicked();

    void on_laser_clicked();

    void on_externalCal_clicked();

    void on_loadScript_clicked();

    void on_tab_currentChanged(int index);

    void on_full_stateChanged(int arg1);

    void on_dacscan_clicked();

    void on_full_2_stateChanged(int arg1);

    void on_dacscan_2_clicked();

private:
    void GetFrame();
    void endTakeData();

private:
    int nx;
    int ny;
    int eventCounter;
    int old_Vcal_Dac_value;
    CDictionary dictDac;
    bool dark;
    bool loop_running;
    bool take_data_running;
    QCPColorMap *colorMap;
    QCustomPlot *customPlot;
    Ui::MainWindow *ui;
    std::vector<double> darkfield;
    std::vector<uint16_t> data;
    QTimer *loopTimer;
    QTimer *takeDataTimer;
    bool columnwise;
    int triggerSource;//  0: internal cal, 1: laser, 2: source
    TFile *fileOut;
    TTree *eventTree;
    int ana[COLUMNS][ROWS];
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
