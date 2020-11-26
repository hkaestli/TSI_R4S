#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QWidget>
#include "ui_customslider.h"
#include "pixel_dtb.h"

extern CTestboard tb;

namespace Ui {
class CustomSlider;
}

class CustomSlider : public QWidget
{
    Q_OBJECT

public:
    explicit CustomSlider(QWidget *parent = 0);
    ~CustomSlider();
    void Init(void (CTestboard::*member)(int), int value, int minimum, int maximum, QString name, QString unit="mV");
    void Init(int value, int minimum, int maximum, QString name, QString unit="mV");

public slots:
    void on_value_editingFinished();
    void on_slider_valueChanged(int position);


private:
    Ui::CustomSlider *ui;
    int maximum, minimum;
    void (CTestboard::*func)(int);
};

#endif // CUSTOMSLIDER_H
