#ifndef PROPERTIESTRACKOBJ_H
#define	PROPERTIESTRACKOBJ_H

#include "PropertiesAbstract.h"

class PropertiesTrackObj : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesTrackObj();
    virtual ~PropertiesTrackObj();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    void updateObj(WorldObj* obj);
public slots:
    void copyFEnabled();
        
signals:
    
private:
    QLineEdit elevProm;
    QLineEdit elev1inXm;
    
};

#endif	/* PROPERTIESTRACKOBJ_H */

