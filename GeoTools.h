#ifndef GEOTOOLS_H
#define	GEOTOOLS_H

#include <QtWidgets>
#include "Route.h"

class GeoTools : public QWidget{
    Q_OBJECT
public:
    GeoTools(QString name);
    virtual ~GeoTools();
    
public slots:
    void mapTileShowToolEnabled();
    void mapTileLoadToolEnabled();
    void heightTileLoadToolEnabled();
    
signals:
    void enableTool(QString name);
    
private:

};

#endif	/* GEOTOOLS_H */

