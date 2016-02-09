#ifndef PROPERTIESSOUNDSOURCE_H
#define	PROPERTIESSOUNDSOURCE_H

#include "PropertiesAbstract.h"

class SoundSourceObj;

class PropertiesSoundSource : public PropertiesAbstract{
    Q_OBJECT 
public:
    bool support(WorldObj* obj);
    void showObj(WorldObj* obj);
    PropertiesSoundSource();
    virtual ~PropertiesSoundSource();
private:
    SoundSourceObj *sobj = NULL;
    QComboBox sources;
    QLineEdit sName;
};

#endif	/* PROPERTIESSOUNDSOURCE_H */

