/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "CoordsRoutePlaces.h"
#include "GLMatrix.h"
#include <math.h>
#include <QDebug>
#include "Game.h"
#include "TDB.h"
#include "TRitem.h"
#include "IghCoords.h"

CoordsRoutePlaces::~CoordsRoutePlaces() {
}

CoordsRoutePlaces::CoordsRoutePlaces(TDB *tdb) {
    QMap<QString, bool> stations;
    IghCoordinate igh;
    PreciseTileCoordinate coords;
    LatitudeLongitudeCoordinate latlon;
    
    for(int i = 0; i < tdb->iTRitems; i++){
        TRitem* n = tdb->trackItems[i];
        if (n == NULL) 
            continue;
        if (n->trItemRData == NULL)
            continue;
        if (n->stationName.length() > 0){
            if(stations[n->stationName] == false){
                stations[n->stationName] = true;
                markerList.emplace_back();
                markerList.back().name = n->stationName;
                markerList.back().type = 0;
                markerList.back().tileX.push_back(n->trItemRData[3]);
                markerList.back().tileZ.push_back(n->trItemRData[4]);
                markerList.back().x.push_back(n->trItemRData[0]);
                markerList.back().z.push_back(n->trItemRData[2]);
                
                coords.TileX = n->trItemRData[3];
                coords.TileZ = n->trItemRData[4];
                coords.setWxyz(n->trItemRData[0], 0, -n->trItemRData[2]);
                MstsCoordinates::ConvertToIgh(&coords, &igh);
                MstsCoordinates::ConvertToLatLon(&igh, &latlon);
                markerList.back().lat = latlon.Latitude;
                markerList.back().lon = latlon.Longitude;
            }
        }
    }
    if (markerList.size() > 0)
        loaded = true;
}