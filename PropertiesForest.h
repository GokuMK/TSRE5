#ifndef PROPERTIESFOREST_H
#define	PROPERTIESFOREST_H

#include "PropertiesAbstract.h"

class PropertiesForest : public PropertiesAbstract{
    Q_OBJECT
public:
    PropertiesForest();
    virtual ~PropertiesForest();
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    
public slots:
        
signals:
    
private:
    QLineEdit sizeX;
    QLineEdit sizeY;
    QLineEdit population;
    QLineEdit densitykm;
};

#endif	/* PROPERTIESFOREST_H */

