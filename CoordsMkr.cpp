/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "CoordsMkr.h"
#include <QFile>
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include <cstdlib>
#include "Vector2f.h"
#include "TerrainLib.h"
#include "TS.h"
#include "Game.h"
#include "FileFunctions.h"
#include "ReadFile.h"
#include "GeoCoordinates.h"

CoordsMkr::CoordsMkr(QString path) {
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    ParserX::NextLine(data);

    QString sh = "";
    //ParserX::szukajsekcji1(sh, data);
    //ParserX::GetNumber(data);

    IghCoordinate* igh;
    PreciseTileCoordinate* ppp;

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("marker")) {
            markerList.emplace_back();
            markerList.back().lon = ParserX::GetNumber(data);
            markerList.back().lat = ParserX::GetNumber(data);
            markerList.back().name = ParserX::GetString(data);
            markerList.back().type = ParserX::GetNumber(data);

            //IghCoordinate* igh = Game::GeoCoordConverter->ConvertToIgh(x, -z, 0, 0);
            //qDebug() << igh->Line << " === " << igh->Sample;
            //LatitudeLongitudeCoordinate* latlon = Game::GeoCoordConverter->ConvertToLatLon(igh);
            //qDebug() << latlon->Latitude << " === " << latlon->Longitude;
            igh = Game::GeoCoordConverter->ConvertToInternal(markerList.back().lat, markerList.back().lon);
            //qDebug() << igh->Line << " === " << igh->Sample;
            ppp = Game::GeoCoordConverter->ConvertToTile(igh);
            //qDebug() << ppp->TileX << " === " << ppp->TileZ << " " << ppp->X << " === " << ppp->Z;
            //igh = Game::GeoCoordConverter->ConvertToIgh(ppp);
            //qDebug() << igh->Line << " === " << igh->Sample;
            //latlon = Game::GeoCoordConverter->ConvertToLatLon(igh);
            //qDebug() << latlon->Latitude << " === " << latlon->Longitude;
            markerList.back().tileX.push_back(ppp->TileX);
            markerList.back().tileZ.push_back(ppp->TileZ);
            markerList.back().x.push_back(ppp->X*2048-1024);
            markerList.back().z.push_back(ppp->Z*2048-1024);

            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }

    if (markerList.size() > 0)
        loaded = true;
}

CoordsMkr::~CoordsMkr() {
}
