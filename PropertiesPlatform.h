#ifndef PROPERTIESPLATFORM_H
#define	PROPERTIESPLATFORM_H

#include "PropertiesAbstract.h"

class PlatformObj;

class PropertiesPlatform : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesPlatform();
    virtual ~PropertiesPlatform();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void leftSideEnabled(int state);
    void rightSideEnabled(int state);
    void disablePlatformEnabled(int state);
    void nameStationEnabled(QString val);
    void namePlatformEnabled(QString val);
    void waitMinEnabled(QString val);
    void waitSecEnabled(QString val);
    void waitPasEnabled(QString val);
        
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
    PlatformObj* pobj;
};

#endif	/* PROPERTIESPLATFORM_H */

