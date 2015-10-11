#ifndef PROPERTIESABSTRACT_H
#define	PROPERTIESABSTRACT_H

#include <QtWidgets>

class WorldObj;

class PropertiesAbstract : public QWidget{
    Q_OBJECT
public:
    PropertiesAbstract();
    virtual ~PropertiesAbstract();
    virtual bool support(WorldObj* obj);
    virtual void showObj(WorldObj* obj);
    
protected:
    QLabel *infoLabel;    
    QLineEdit flags;
    QLineEdit fileName;
    QLineEdit posX;
    QLineEdit posY;
    QLineEdit posZ;
    QLineEdit quat;
    QLineEdit tX;
    QLineEdit tY;
    QLineEdit uid;
private:

};

#endif	/* PROPERTIESABSTRACT_H */

