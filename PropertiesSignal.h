#ifndef PROPERTIESSIGNAL_H
#define	PROPERTIESSIGNAL_H

#include "PropertiesAbstract.h"

class SignalObj;

class PropertiesSignal : public PropertiesAbstract{
    Q_OBJECT
    
public:
    PropertiesSignal();
    virtual ~PropertiesSignal();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);

public slots:
    void chSubEnabled(int idx);    
    void flipSignal();
    
signals:
    
private:
    static const int maxSubObj = 10;
    QLineEdit name;
    QLineEdit description;
    QCheckBox chSub[maxSubObj];
    QCheckBox chFlipShape;
    QPushButton bSub[maxSubObj];
    QLineEdit dSub[maxSubObj];
    QGridLayout vSub[maxSubObj];
    QWidget wSub[maxSubObj];
    SignalObj* sobj;
    QSignalMapper signalsChSect;
};

#endif	/* PROPERTIESSIGNAL_H */

