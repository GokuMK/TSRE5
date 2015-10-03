#ifndef PROPERTIESPLATFORM_H
#define	PROPERTIESPLATFORM_H

#include "PropertiesAbstract.h"

class PropertiesPlatform : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesPlatform();
    virtual ~PropertiesPlatform();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    //void copyFEnabled();
        
signals:
    
private:
    QCheckBox leftSide;
    QCheckBox rightSide;
    QCheckBox disablePlatform;
    QLineEdit nameStation;
    QLineEdit namePlatform;
    QLineEdit waitMin;
    QLineEdit waitSec;
    QLineEdit waitPas;
    
};

#endif	/* PROPERTIESPLATFORM_H */

