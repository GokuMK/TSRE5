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
    QLineEdit name;
    QLineEdit description;
    QCheckBox chSub[10];
    QCheckBox chFlipShape;
    QPushButton bSub[10];
    QLineEdit dSub[10];
    QGridLayout vSub[10];
    QWidget wSub[10];
    SignalObj* sobj;
    QSignalMapper signalsChSect;
};

#endif	/* PROPERTIESSIGNAL_H */

