/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TrackShape.h"

TrackShape::TrackShape() {
    
}

TrackShape::TrackShape(int t) {
    this->id = t;
}

QString TrackShape::getHashString(){
    if(filename.length() > 0)
        return filename;
    
    QString hash;
    for(int i = 0; i < numpaths; i++){
        for(int j = 0; j < path[i].n; j++)
        hash += QString::number(path[i].sect[j], 16);
    }
    return hash;
}

TrackShape::~TrackShape() {
}

