#ifndef PROPERTIESPICKUP_H
#define	PROPERTIESPICKUP_H

#include "PropertiesAbstract.h"

class PickupObj;

class PropertiesPickup : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesPickup();
    virtual ~PropertiesPickup();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
        
signals:
    
private:
    PickupObj* pobj;
    QLineEdit eBrokenFileName;
    QComboBox cPickupType;
    QComboBox cAnimType;
    QLineEdit eCapacity;
    QLineEdit eContent;
    QLineEdit eFill;
    QLineEdit eSpeedMin;
    QLineEdit eSpeedMax;
    QLineEdit eAnimLength;
    QCheckBox chInfinite;
    QCheckBox chBroken;
};

#endif	/* PROPERTIESPICKUP_H */

