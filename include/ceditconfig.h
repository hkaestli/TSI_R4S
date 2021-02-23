#ifndef CEDITCONFIG_H
#define CEDITCONFIG_H

#include <QWidget>
#include <QTableView>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include "customslider.h"
#include "configuration.h"
#include "dictionary.h"


class CEditConfig : public QTableView
{
    Q_OBJECT

public:
    explicit CEditConfig(QWidget *parent = nullptr);
    ~CEditConfig();
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                     const QVector<int> &roles = QVector<int>()) override;
    void setModel(QAbstractItemModel *model) override;
    QString getScript() { return script->text();};
    QTabWidget *tabWidget;
    QPushButton *loadScript;

private:
    void CreateField(Specifier &s, int cfg, QRect geom, QWidget *tab);
    CDictionary dict;
    QLineEdit *script;
    QWidget *tab;
    QWidget *tab_2;
    QWidget *tab_3;
    CustomSlider *dac[CONFIGURATIONS][PARAMETERS]={{nullptr}};
    QLabel *title;
    void setupUi();
};

#endif // CEDITCONFIG_H
