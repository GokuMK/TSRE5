/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef MAPDATAURLIMAGE_H
#define	MAPDATAURLIMAGE_H

#include "MapData.h"
#include <unordered_map>
#include <vector>

class MapDataUrlImage : public MapData {
    Q_OBJECT
public:
    MapDataUrlImage();
    MapDataUrlImage(const MapDataUrlImage& orig);
    virtual ~MapDataUrlImage();
    bool draw(QImage* myImage);
    void load();
    
signals:
    void loaded();
    void statusInfo(QString val);
    
private:

};

#endif	/* MAPDATAURLIMAGE_H */

