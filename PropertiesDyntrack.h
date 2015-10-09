#ifndef PROPERTIESDYNTRACK_H
#define	PROPERTIESDYNTRACK_H

#include "PropertiesAbstract.h"

class DynTrackObj;

class PropertiesDyntrack : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesDyntrack();
    virtual ~PropertiesDyntrack();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void flexEnabled();
    void flexData(int x, int z, float* p);
    void chSectEnabled(int idx);
    
signals:
    void enableTool(QString val);
    
private:
    QCheckBox chSect[5];
    QDoubleSpinBox sSectA[5];
    QDoubleSpinBox sSectR[5];
    QFormLayout vSect[5];
    QWidget wSect[5];
    DynTrackObj* dobj;
    QSignalMapper dyntrackChSect;
};

#endif	/* PROPERTIESDYNTRACK_H */

