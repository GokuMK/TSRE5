#ifndef PROPERTIESSIGNAL_H
#define	PROPERTIESSIGNAL_H

#include "PropertiesAbstract.h"

class SignalWindow;
class SignalObj;

class PropertiesSignal : public PropertiesAbstract{
    Q_OBJECT
    
public:
    PropertiesSignal();
    virtual ~PropertiesSignal();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);

public slots:
    void flipSignal();
    void showSubObjList();
    
signals:
    
private:
    QLineEdit name;
    QLineEdit description;
    QCheckBox chFlipShape;
    SignalObj* sobj;
    SignalWindow* signalWindow;
};

#endif	/* PROPERTIESSIGNAL_H */

