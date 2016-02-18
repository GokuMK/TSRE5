#ifndef PROPERTIESSPEEDPOST_H
#define	PROPERTIESSPEEDPOST_H

#include "PropertiesAbstract.h"

class SpeedpostObj;

class PropertiesSpeedpost : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesSpeedpost();
    virtual ~PropertiesSpeedpost();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void flipSignal();
    void speedEnabled(QString val);
    void numberEnabled(QString val);
signals:
    
private:
    SpeedpostObj* sobj;
    QLabel speedpostType;
    QLineEdit speed;
    QLineEdit number;
    QComboBox ptb;
    QCheckBox chFlipShape;
};

#endif	/* PROPERTIESSPEEDPOST_H */

