#ifndef PROPERTIESUNDEFINED_H
#define	PROPERTIESUNDEFINED_H

#include <QtWidgets>
#include "PropertiesAbstract.h"

class WorldObj;

class PropertiesUndefined : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesUndefined();
    virtual ~PropertiesUndefined();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    
signals:
    
    
private:

};

#endif	/* PROPERTIESUNDEFINED_H */

