#ifndef PROPERTIESCARSPAWNER_H
#define	PROPERTIESCARSPAWNER_H

#include "PropertiesAbstract.h"

class PlatformObj;

class PropertiesCarspawner : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesCarspawner();
    virtual ~PropertiesCarspawner();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void carNumberEnabled(QString val);
    void carSpeedEnabled(QString val);
        
signals:
    
private:
    QLineEdit carNumber;
    QLineEdit carSpeed;
    PlatformObj* cobj;
    QLineEdit lengthPlatform;
};

#endif	/* PROPERTIESCARSPAWNER_H */

