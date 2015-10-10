#ifndef PROPERTIESSIDING_H
#define	PROPERTIESSIDING_H

#include "PropertiesAbstract.h"

class PlatformObj;

class PropertiesSiding : public PropertiesAbstract{
    Q_OBJECT 
public:
    PropertiesSiding();
    virtual ~PropertiesSiding();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void disablePlatformEnabled(int state);
    void namePlatformEnabled(QString val);
        
signals:
    
private:
    PlatformObj* pobj;
    QCheckBox disablePlatform;
    QLineEdit namePlatform;
};

#endif	/* PROPERTIESSIDING_H */

