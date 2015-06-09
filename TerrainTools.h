/* 
 * File:   TerrainTools.h
 * Author: Goku
 *
 * Created on 1 czerwca 2015, 20:39
 */

#include <QtWidgets>
#include "Route.h"

#ifndef TERRAINTOOLS_H
#define	TERRAINTOOLS_H

class TerrainTools : public QWidget{
    Q_OBJECT

public:
    TerrainTools(QString name);
    virtual ~TerrainTools();
    
public slots:
    void heightToolEnabled();
    void paintToolEnabled();
    
signals:
    void enableTool(QString name);
    
private:

};

#endif	/* TERRAINTOOLS_H */

