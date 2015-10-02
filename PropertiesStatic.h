#ifndef PROPERTIESSTATIC_H
#define	PROPERTIESSTATIC_H

#include "PropertiesAbstract.h"

class PropertiesStatic : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesStatic();
    virtual ~PropertiesStatic();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    void copyFEnabled();
        
signals:
    
private:
    QCheckBox checkboxAnim;
    QCheckBox checkboxTerrain;
};

#endif	/* PROPERTIESSTATIC_H */

