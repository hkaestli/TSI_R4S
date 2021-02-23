#include <QAbstractTableModel>
#include "ceditconfig.h"
#include "mainwindow.h"

CEditConfig::CEditConfig(QWidget *parent) :
    QTableView(parent)
{
    setupUi();
}

CEditConfig::~CEditConfig()
{
    for(int i=0; i<CONFIGURATIONS; i++){
        for(int j=0; j<PARAMETERS; j++){
            if(dac[i][j]) delete dac[i][j];
        }
    }
}

void CEditConfig::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    for(QVector<int>::const_iterator it=roles.begin(); it!=roles.end(); it++){
        if((*it)==Qt::EditRole){
            if(dac[topLeft.row()][topLeft.column()])
                dac[topLeft.row()][topLeft.column()]->showValue(topLeft.data().toInt());
        }
    }
}

void CEditConfig::setupUi()
{
    tabWidget = new QTabWidget(this);
    tabWidget->setGeometry(QRect(15, 5, 460, 550));

    tab = new QWidget();
    tabWidget->addTab(tab, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab), "NMOS linear ");
    {
        int cfg=NMOS_LINEAR;
        Specifier s=dict.data("vdig");
        CreateField(s, cfg, QRect(20, 10, 430, 30), tab);
        s=dict.data("v18");
        CreateField(s, cfg, QRect(20, 35, 430, 30), tab);
        s=dict.data("vddio");
        CreateField(s, cfg, QRect(20, 60, 430, 30), tab);
        s=dict.data("vbiasd");
        CreateField(s, cfg, QRect(20, 85, 430, 30), tab);

        s=dict.data("vanan");
        CreateField(s, cfg, QRect(20, 155, 430, 30), tab);
        s=dict.data("vcascn");
        CreateField(s, cfg, QRect(20, 180, 430, 30), tab);
        s=dict.data("vn0");
        CreateField(s, cfg, QRect(20, 205, 430, 30), tab);
        s=dict.data("vn1");
        CreateField(s, cfg, QRect(20, 230, 430, 30), tab);
        s=dict.data("vn2");
        CreateField(s, cfg, QRect(20, 255, 430, 30), tab);
        s=dict.data("vfb");
        CreateField(s, cfg, QRect(20, 280, 430, 30), tab);

        s=dict.data("voffset");
        CreateField(s, cfg, QRect(20, 350, 430, 30), tab);
        s=dict.data("ibiasro");
        CreateField(s, cfg, QRect(20, 375, 430, 30), tab);
        s=dict.data("ibiasio");
        CreateField(s, cfg, QRect(20, 400, 430, 30), tab);

        s=dict.data("vcal");
        CreateField(s, cfg, QRect(20, 445, 430, 30), tab);
        s=dict.data("thold");
        CreateField(s, cfg, QRect(20, 470, 430, 30), tab);
    }
    tab_2 = new QWidget();
    tabWidget->addTab(tab_2, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab_2), "NMOS ELT");
    {
        int cfg=NMOS_ELT;
        Specifier s=dict.data("vdig");
        CreateField(s, cfg, QRect(20, 10, 430, 30), tab_2);
        s=dict.data("v18");
        CreateField(s, cfg, QRect(20, 35, 430, 30), tab_2);
        s=dict.data("vddio");
        CreateField(s, cfg, QRect(20, 60, 430, 30), tab_2);
        s=dict.data("vbiasd");
        CreateField(s, cfg, QRect(20, 85, 430, 30), tab_2);
        s=dict.data("vbiasr");
        CreateField(s, cfg, QRect(20, 110, 430, 30), tab_2);

        s=dict.data("vanan");
        CreateField(s, cfg, QRect(20, 155, 430, 30), tab_2);
        s=dict.data("vcascn");
        CreateField(s, cfg, QRect(20, 180, 430, 30), tab_2);
        s=dict.data("vn0");
        CreateField(s, cfg, QRect(20, 205, 430, 30), tab_2);
        s=dict.data("vn1");
        CreateField(s, cfg, QRect(20, 230, 430, 30), tab_2);
        s=dict.data("vn2");
        CreateField(s, cfg, QRect(20, 255, 430, 30), tab_2);
        s=dict.data("vfb");
        CreateField(s, cfg, QRect(20, 280, 430, 30), tab_2);
        s=dict.data("vprefb");
        CreateField(s, cfg, QRect(20, 305, 430, 30), tab_2);

        s=dict.data("voffset");
        CreateField(s, cfg, QRect(20, 350, 430, 30), tab_2);
        s=dict.data("ibiasro");
        CreateField(s, cfg, QRect(20, 375, 430, 30), tab_2);
        s=dict.data("ibiasio");
        CreateField(s, cfg, QRect(20, 400, 430, 30), tab_2);

        s=dict.data("vcal");
        CreateField(s, cfg, QRect(20, 445, 430, 30), tab_2);
        s=dict.data("thold");
        CreateField(s, cfg, QRect(20, 470, 430, 30), tab_2);
    }
    tab_3 = new QWidget();
    tabWidget->addTab(tab_3, QString());
    tabWidget->setTabText(tabWidget->indexOf(tab_3), "PMOS");
    {
        int cfg=PMOS;
        Specifier s=dict.data("vdig");
        CreateField(s, cfg, QRect(20, 10, 430, 30), tab_3);
        s=dict.data("v18");
        CreateField(s, cfg, QRect(20, 35, 430, 30), tab_3);
        s=dict.data("vddio");
        CreateField(s, cfg, QRect(20, 60, 430, 30), tab_3);
        s=dict.data("vbiasd");
        CreateField(s, cfg, QRect(20, 85, 430, 30), tab_3);
        s=dict.data("vbiasr");
        CreateField(s, cfg, QRect(20, 110, 430, 30), tab_3);

        s=dict.data("vanap");
        CreateField(s, cfg, QRect(20, 155, 430, 30), tab_3);
        s=dict.data("vcascp");
        CreateField(s, cfg, QRect(20, 180, 430, 30), tab_3);
        s=dict.data("vp0");
        CreateField(s, cfg, QRect(20, 205, 430, 30), tab_3);
        s=dict.data("vp1");
        CreateField(s, cfg, QRect(20, 230, 430, 30), tab_3);
        s=dict.data("vp2");
        CreateField(s, cfg, QRect(20, 255, 430, 30), tab_3);
        s=dict.data("vfb");
        CreateField(s, cfg, QRect(20, 280, 430, 30), tab_3);

        s=dict.data("voffset");
        CreateField(s, cfg, QRect(20, 350, 430, 30), tab_3);
        s=dict.data("ibiasro");
        CreateField(s, cfg, QRect(20, 375, 430, 30), tab_3);
        s=dict.data("ibiasio");
        CreateField(s, cfg, QRect(20, 400, 430, 30), tab_3);

        s=dict.data("vcal");
        CreateField(s, cfg, QRect(20, 445, 430, 30), tab_3);
        s=dict.data("thold");
        CreateField(s, cfg, QRect(20, 470, 430, 30), tab_3);
    }
    tabWidget->setCurrentIndex(0);
    loadScript = new QPushButton(this);
    loadScript->setText("Load script");
    loadScript->setGeometry(QRect(320, 560, 112, 36));
    script = new QLineEdit(this);
    script->setGeometry(QRect(70, 566, 211, 25));

    QMetaObject::connectSlotsByName(this);

} // setupUi

void CEditConfig::CreateField(Specifier &s, int cfg, QRect geom, QWidget *tab)
{
    int id=s.getId();
    dac[cfg][id] = new CustomSlider(tab);
    dac[cfg][id]->setGeometry(geom);
    dac[cfg][id]->Init(0., s.getMin(), s.getMax(),s.getName(), s.getUnits());
}


void CEditConfig::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    for(int cfg=0; cfg< CONFIGURATIONS; cfg++){
        for(int id=0; id<PARAMETERS; id++){
            if(dac[cfg][id])    {
                QModelIndex in=model->index(cfg, id);
                dac[cfg][id]->setModel(model, in);
            }
        }
    }
}

