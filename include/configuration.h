#ifndef CCONFIGURATION_H
#define CCONFIGURATION_H

#include <QAbstractTableModel>
#include <vector>
#include "pixel_dtb.h"

extern CTestboard tb;

const int PARAMETERS= 22;                       // number of parameters per configuration

const int CONFIGURATIONS= 3;                    // number of different configuration
enum CONFIG {NMOS_LINEAR=0, NMOS_ELT, PMOS};    // these are the possible configurations

class CParameter
{
public:
    CParameter(void (CTestboard::*member)(int), int Min, int Max, QString Name, QString Units="mV") :
        value(Min), min(Min), max(Max), name(Name), units(Units),
        tb_function(member){ }
    void setValue(int number) {
        value=number;
        if(value<min) value=min;
        else if(value>max) value=max;
        (tb.*tb_function)(value);
        tb.Flush();
    };
    void writeValue() {
            (tb.*tb_function)(value);
             tb.Flush();
    };
    int getValue() { return value; };
    int getMin() { return min; };
    int getMax() { return max; };
    QString getName() { return name; };
    QString getUnits() { return units; };

private:
    int value;
    int min;
    int max;
    QString name;
    QString units;
    void (CTestboard::*tb_function)(int);
};

typedef std::vector<CParameter> ParamList;

class CConfiguration : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit CConfiguration(QObject *parent = nullptr);

    void Init();
    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    CParameter getParam(int cfg, int parameter){ return configurations[cfg][parameter];};

    Qt::ItemFlags flags(const QModelIndex& index) const override;

public slots:
    void Activate(int cfg);

public:
    void Activate(int col, int row);

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override
    {
            return createIndex(row, column);
    };
    QModelIndex index(int column) const
    {
            return createIndex(activeConfiguration, column);
    };

public slots:
    void currentChanged(int index) { Activate(index);};

private:
    int activeConfiguration;
    std::vector<ParamList> configurations;
};

#endif // CCONFIGURATION_H
