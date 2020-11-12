#include "include/customslider.h"


CustomSlider::CustomSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomSlider)
{
    func=NULL;
    func32=NULL;
    ui->setupUi(this);
    connect(ui->slider,&QSlider::sliderMoved, this, &CustomSlider::on_slider_valueChanged);
    connect(ui->value, &QLineEdit::editingFinished , this, &CustomSlider::on_value_editingFinished);
}

CustomSlider::~CustomSlider()
{
    delete ui;
}

void CustomSlider::on_slider_valueChanged(int position)
{
    ui->value->setText(QString::number( position ) );
    if(func) (tb.*func)((uint16_t) position);
    else if(func32) (tb.*func32)((uint32_t) position);
    tb.Flush();
}

void CustomSlider::on_value_editingFinished()
{
    int value=ui->value->text().toInt();
    if (value<minimum) {
        value=minimum;
        ui->value->setText(QString::number(value));
    } else if (value>maximum){
        value=maximum;
        ui->value->setText(QString::number(value));
    }
    ui->slider->setSliderPosition(value);
    if(func) (tb.*func)((uint16_t) value);
    else if(func32) (tb.*func32)((uint32_t) value);
    tb.Flush();
}

void CustomSlider::Init(void(CTestboard::*member)(uint16_t), int value, int min, int max, QString name, QString unit){
    func=member;
    func32=NULL;
    minimum=min;
    maximum=max;
    ui->name->setText(name);
    ui->unit->setText(unit);
    ui->slider->setRange(minimum, maximum);
    if(value<minimum) value=minimum;
    if(value>maximum) value=maximum;
    ui->value->setText(QString::number(value));
    ui->slider->setSliderPosition(value);
    ui->slider->setSingleStep(10);
}

void CustomSlider::Init(void(CTestboard::*member)(uint32_t), int value, int min, int max, QString name, QString unit){
    func=NULL;
    func32=member;
    minimum=min;
    maximum=max;
    ui->name->setText(name);
    ui->unit->setText(unit);
    ui->slider->setRange(minimum, maximum);
    if(value<minimum) value=minimum;
    if(value>maximum) value=maximum;
    ui->value->setText(QString::number(value));
    ui->slider->setSliderPosition(value);
    ui->slider->setSingleStep(10);
}

void CustomSlider::Init(int value, int min, int max, QString name, QString unit){
    func=NULL;
    func32=NULL;
    minimum=min;
    maximum=max;
    ui->name->setText(name);
    ui->unit->setText(unit);
    ui->slider->setRange(minimum, maximum);
    if(value<minimum) value=minimum;
    if(value>maximum) value=maximum;
    ui->value->setText(QString::number(value));
    ui->slider->setSliderPosition(value);
    ui->slider->setSingleStep(10);
}
