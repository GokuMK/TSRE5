#ifndef PROPERTIESCARSPAWNER_H
#define	PROPERTIESCARSPAWNER_H

#include "PropertiesAbstract.h"

class PropertiesCarspawner : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesCarspawner();
    virtual ~PropertiesCarspawner();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
    //void copyFEnabled();
        
signals:
    
private:
    QLineEdit carNumber;
    QLineEdit carSpeed;
    
};

#endif	/* PROPERTIESCARSPAWNER_H */

