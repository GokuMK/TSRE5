/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef MAPDATA_H
#define	MAPDATA_H

#include <QObject>

class QImage;

class MapData : public QObject {
    Q_OBJECT 
public:
    float maxlat, minlat, maxlon, minlon;
    int tileX, tileZ;
    float level;
    int tileSize;
    bool init = false;
    
    MapData();
    MapData(const MapData& orig);
    virtual ~MapData();
    virtual bool draw(QImage* myImage);
    virtual void load();
    
signals:
    virtual void loaded();
    virtual void statusInfo(QString val);
    
public slots:

private:

};

#endif	/* MAPDATA_H */

