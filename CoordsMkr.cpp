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
#include "IghCoords.h"

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

            //IghCoordinate* igh = MstsCoordinates::ConvertToIgh(x, -z, 0, 0);
            //qDebug() << igh->Line << " === " << igh->Sample;
            //LatitudeLongitudeCoordinate* latlon = MstsCoordinates::ConvertToLatLon(igh);
            //qDebug() << latlon->Latitude << " === " << latlon->Longitude;
            igh = MstsCoordinates::ConvertToIgh(markerList.back().lat, markerList.back().lon);
            //qDebug() << igh->Line << " === " << igh->Sample;
            ppp = MstsCoordinates::ConvertToTile(igh);
            //qDebug() << ppp->TileX << " === " << ppp->TileZ << " " << ppp->X << " === " << ppp->Z;
            //igh = MstsCoordinates::ConvertToIgh(ppp);
            //qDebug() << igh->Line << " === " << igh->Sample;
            //latlon = MstsCoordinates::ConvertToLatLon(igh);
            //qDebug() << latlon->Latitude << " === " << latlon->Longitude;
            markerList.back().tileX = ppp->TileX;
            markerList.back().tileZ = ppp->TileZ;
            markerList.back().x = ppp->X*2048-1024;
            markerList.back().z = ppp->Z*2048-1024;

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
