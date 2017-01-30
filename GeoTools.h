/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    void msg(QString text, QString val);
    
signals:
    void enableTool(QString name);
    
private:
    QMap<QString, QPushButton*> buttonTools;
};

#endif	/* GEOTOOLS_H */

