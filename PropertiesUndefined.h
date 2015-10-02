#ifndef PROPERTIESUNDEFINED_H
#define	PROPERTIESUNDEFINED_H

#include <QtWidgets>

class WorldObj;

class PropertiesUndefined : public QWidget{
    Q_OBJECT
public:
    PropertiesUndefined();
    virtual ~PropertiesUndefined();
    void showObj(WorldObj* obj);
    
public slots:
    
signals:
    
private:
    QLabel *infoLabel;
};

#endif	/* PROPERTIESUNDEFINED_H */

