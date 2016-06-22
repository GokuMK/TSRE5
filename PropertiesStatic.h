#ifndef PROPERTIESSTATIC_H
#define	PROPERTIESSTATIC_H

#include "PropertiesAbstract.h"

class StaticObj;

class PropertiesStatic : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesStatic();
    virtual ~PropertiesStatic();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void copyFEnabled();
    void copyREnabled();
    void pasteREnabled();
    void rotYEnabled();
    void resetRotEnabled();
        
signals:
    
private:
    StaticObj *staticObj;
    QCheckBox checkboxAnim;
    QCheckBox checkboxTerrain;
};

#endif	/* PROPERTIESSTATIC_H */

