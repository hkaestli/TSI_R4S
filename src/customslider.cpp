#include "customslider.h"


CustomSlider::CustomSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomSlider), model(nullptr)
{
    ui->setupUi(this);
    connect(ui->slider,&QSlider::sliderMoved, this, &CustomSlider::on_slider_valueChanged);
    connect(ui->value, &QLineEdit::editingFinished , this, &CustomSlider::on_value_editingFinished);
}

CustomSlider::~CustomSlider()
{
    delete ui;
}

void CustomSlider::on_slider_valueChanged(int value)
{
   if(model) {
       model->setData(index, value);
   }
}

void CustomSlider::on_value_editingFinished()
{
    if(model){
       int value=ui->value->text().toInt();
       model->setData(index, value);
    }
}

void CustomSlider::showValue(int number)
{
    ui->value->setText(QString::number(number));
    ui->slider->setSliderPosition(number);
}

void CustomSlider::Init(int value, int min, int max, QString name, QString unit){
    ui->name->setText(name);
    ui->unit->setText(unit);
    ui->slider->setRange(min, max);
    ui->value->setText(QString::number(value));
    ui->slider->setSliderPosition(value);
    ui->slider->setSingleStep(10);
}
