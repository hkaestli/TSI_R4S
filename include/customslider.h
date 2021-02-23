#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QWidget>
#include <QAbstractItemModel>
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
    void Init(int value, int minimum, int maximum, QString name, QString unit="mV");

public slots:
    void on_value_editingFinished();
    void on_slider_valueChanged(int position);
    void showValue(int number);
    void setModel(QAbstractItemModel *c, QModelIndex in) { model=c; index=in;};
    QString getName(){ return ui->name->text();};
private:
    Ui::CustomSlider *ui;
    QAbstractItemModel *model;
    QModelIndex index;
};

#endif // CUSTOMSLIDER_H
