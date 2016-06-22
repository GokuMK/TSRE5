#ifndef PROPERTIESTRACKOBJ_H
#define	PROPERTIESTRACKOBJ_H

#include "PropertiesAbstract.h"

class TrackObj;

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
    void copyREnabled();
    void pasteREnabled();
    void rotYEnabled();
    void resetRotEnabled();
    
signals:
    
private:
    TrackObj* trackObj;
    QLineEdit elevProm;
    QLineEdit elev1inXm;
    
};

#endif	/* PROPERTIESTRACKOBJ_H */

