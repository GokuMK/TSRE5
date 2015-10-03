#ifndef PROPERTIESSIDING_H
#define	PROPERTIESSIDING_H

#include "PropertiesAbstract.h"

class PropertiesSiding : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesSiding();
    virtual ~PropertiesSiding();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    //void copyFEnabled();
        
signals:
    
private:
    QCheckBox disablePlatform;
    QLineEdit namePlatform;
};

#endif	/* PROPERTIESSIDING_H */

