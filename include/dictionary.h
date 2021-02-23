#ifndef CDICTIONARY_H
#define CDICTIONARY_H

#include <map>
#include <QString>

enum DACs{VanaN, VanaP, Vdig, VDDIO, V18, BiasD, BiasR, VcascN, Vn0, Vn1, Vn2,
          Vfb, Vprfb, VcascP, Vp0, Vp1, Vp2, Vcal, Hold, IBiasRO, IBiasIO, VOffset};

class Specifier{
public:
    Specifier(): id(0), min(0), max(0), name(""), units(""){};
    Specifier(int Id, int Min, int Max, QString Name, QString Units="mV"){
            id=Id;
            min=Min;
            max=Max;
            name=Name;
            units=Units;
    };
    int getId() { return id; };
    int getMin() { return min; };
    int getMax() { return max; };
    QString getName() { return name; };
    QString getUnits() { return units; };
private:
        int id;
        int min;
        int max;
        QString name;
        QString units;
} ;


class CDictionary {
public:
    CDictionary(){
        Specifier dac(VanaN, 0, 1800, "Vana N");
        DACs.insert({"vanan",dac});
        dac=Specifier(VanaP, 0, 1800, "Vana P");
        DACs.insert({"vanap",dac});
        dac=Specifier(Vdig, 0, 2000, "Vdig");
        DACs.insert({"vdig",dac});
        dac=Specifier(VDDIO, 0, 1800, "Vdd IO");
        DACs.insert({"vddio",dac});
        dac=Specifier(V18, 0, 1800, "V18");
        DACs.insert({"v18",dac});
        dac=Specifier(BiasD, 0, 2400, "Vbias Diode");
        DACs.insert({"vbiasd",dac});
        dac=Specifier(BiasR, 0, 1800, "Vbias Res");
        DACs.insert({"vbiasr",dac});
        dac=Specifier(VcascN, 0, 1800, "Vcasc N");
        DACs.insert({"vcascn",dac});
        dac=Specifier(Vn0, 0, 1800, "Vn0");
        DACs.insert({"vn0",dac});
        dac=Specifier(Vn1, 0, 1800, "Vn1");
        DACs.insert({"vn1",dac});
        dac=Specifier(Vn2, 0, 1800, "Vn2");
        DACs.insert({"vn2",dac});
        dac=Specifier(Vfb, 0, 1800, "Vfb");
        DACs.insert({"vfb",dac});
        dac=Specifier(Vprfb, 0, 1800, "Vprefb");
        DACs.insert({"vprefb",dac});
        dac=Specifier(VcascP, 0, 1800, "Vcasc P");
        DACs.insert({"vcascp",dac});
        dac=Specifier(Vp0, 0, 1800, "Vp0");
        DACs.insert({"vp0",dac});
        dac=Specifier(Vp1, 0, 1800, "Vp1");
        DACs.insert({"vp1",dac});
        dac=Specifier(Vp2, 0, 1800, "Vp2");
        DACs.insert({"vp2",dac});
        dac=Specifier(Vcal, 0, 1800, "Vcal");
        DACs.insert({"vcal",dac});
        dac=Specifier(Hold, 0, 3000, "t hold", "a/u");
        DACs.insert({"thold",dac});
        dac=Specifier(IBiasRO, 0, 1800, "VIbias ro");
        DACs.insert({"ibiasro",dac});
        dac=Specifier(IBiasIO, 0, 1800, "VIbias I/O");
        DACs.insert({"ibiasio",dac});
        dac=Specifier(VOffset, 0, 1800, "VOffset");
        DACs.insert({"voffset",dac});
    }
    Specifier data(QString name){ return DACs[name]; };
    int Id(QString name){ return DACs[name].getId(); };
    Specifier data(int id){
        for(it=DACs.begin(); it!=DACs.end(); it++){
            if(it->second.getId()==id) return it->second;
        }
        return Specifier();
    }

private:
    std::map<QString,Specifier> DACs;
    std::map<QString, Specifier>::iterator it;
};

#endif // CDICTIONARY_H
