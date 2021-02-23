#include "configuration.h"

CConfiguration::CConfiguration(QObject *parent)
    : QAbstractTableModel(parent),
      activeConfiguration(NMOS_LINEAR)
{
}

void CConfiguration::Init()
{
    ParamList config;
    CParameter p(&CTestboard::SetVanaN, 0, 1800, "Vana N");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVanaP, 0, 1800, "Vana P");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVdig, 0, 2000, "Vdig");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVddio, 0, 1800, "VDDIO");
    config.push_back(p);
    p=CParameter(&CTestboard::SetV18, 0, 1800, "V18");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVbiasD, 0, 2400, "Bias D");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVbiasR, 0, 1800, "Bias R");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVcascN, 0, 1800, "Vcasc N");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVn0, 0, 1800, "Vn0");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVn1, 0, 1800, "Vn1");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVn2, 0, 1800, "Vn2");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVfb, 0, 1800, "Vfb");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVprefb, 0, 1800, "Vprfb");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVcascP, 0, 1800, "Vcasc P");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVp0, 0, 1800, "Vp0");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVp1, 0, 1800, "Vp1");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVp2, 0, 1800, "Vp2");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVcal, 0, 1800, "Vcal");
    config.push_back(p);
    p=CParameter(&CTestboard::SetThold, 1500, 3000, "t hold","a/u");
    config.push_back(p);
    p=CParameter(&CTestboard::SetIbiasRO, 0, 2000, "IBias ro");
    config.push_back(p);
    p=CParameter(&CTestboard::SetIbiasIO, 0, 2000, "IBias I/O");
    config.push_back(p);
    p=CParameter(&CTestboard::SetVoffset, 0, 1800, "VOffset");
    config.push_back(p);
    for(int i=0; i<CONFIGURATIONS; i++){            // clone configuration
        configurations.push_back(config);           // initially all configurations are identical
    }                                               // and read from the testboard
    activeConfiguration=NMOS_LINEAR;
    for(int i=0; i<CONFIGURATIONS; i++){
        for(int j=0; j<PARAMETERS; j++){
            configurations[i][j].writeValue();
            emit dataChanged(createIndex(i,j), createIndex(i, j), {Qt::DisplayRole});
        }
    }
}


int CConfiguration::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return PARAMETERS;
    return 0;
}

int CConfiguration::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return CONFIGURATIONS;
    return 0;
}

QVariant CConfiguration::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role!=Qt::DisplayRole)
        return QVariant();

    int cfg = index.row();
    int param = index.column();

    CParameter p= configurations[cfg][param];
    return p.getValue();
}

bool CConfiguration::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        int cfg = index.row();
        Activate(cfg);
        int param = index.column();
        configurations[cfg][param].setValue(value.toInt());
        emit dataChanged(index, index, QVector<int>() << role);
       return true;
    }
    return false;
}

void CConfiguration::Activate(int cfg)
{
    if(cfg<0 || cfg>CONFIGURATIONS) return;
    if (cfg==activeConfiguration) return;
    activeConfiguration=cfg;
    for(int i=0; i<PARAMETERS; i++){
        configurations[activeConfiguration][i].writeValue();
    }
}


void CConfiguration::Activate(int col, int row)
{
    int cfg=NMOS_LINEAR;                            // linear NMOS input amplifiers
    if(col>19) cfg=PMOS;                            // PMOS input amplifiers
    else if(col<10 && row<5) cfg=NMOS_ELT;          // ELT NMOS input amplifiers
    Activate(cfg);
}


Qt::ItemFlags CConfiguration::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}
