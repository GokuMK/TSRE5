#include "CoordsKml.h"
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

CoordsKml::CoordsKml(QString path) {
    qDebug() << "kml!";
    loaded = false;

    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "no file" << file.errorString();
        return;
    }
    qDebug() <<  "file";
    QByteArray data = file.readAll();
    
    bool placemark = false;
    bool coordinates = false;
    bool nametext = false;
    int iii = 0, uuu = 0;
    IghCoordinate* igh;
    PreciseTileCoordinate* ppp;
    
    QXmlStreamReader reader((data));
    reader.readNext();
    QString name;
    QXmlStreamAttributes attr;
    while (!reader.isEndDocument()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            attr = reader.attributes();
            
            if (name.toUpper() == ("PLACEMARK")) {
                placemark = true;
                markerList.emplace_back();
            } else if (name.toUpper() == ("COORDINATES")) {
                if(placemark)
                    coordinates = true;
            } else if (name.toUpper() == ("NAME")) {
                if(placemark){
                    nametext = true;
                }
            }
        } else if (reader.isEndElement()) {
            name = reader.name().toString();
            if (name.toUpper() == ("PLACEMARK")) {
                placemark = false;
            }
            if (name.toUpper() == ("COORDINATES")) {
                if(placemark)
                    coordinates = false;
            } else if (name.toUpper() == ("NAME")) {
                if(placemark){
                    nametext = false;
                }
            }
        } else if (reader.isCharacters()) {
            if(nametext){
                qDebug() << reader.text();
                markerList.back().name = reader.text().toString();
            }
            if(coordinates){
                qDebug() << reader.text();
                QStringList c = reader.text().toString().split(",");
                if(c.length() < 2) return;
                markerList.back().lat = c[1].toFloat();
                markerList.back().lon = c[0].toFloat();
                markerList.back().type = 0;
                igh = MstsCoordinates::ConvertToIgh(markerList.back().lat, markerList.back().lon);
                ppp = MstsCoordinates::ConvertToTile(igh);
                markerList.back().tileX = ppp->TileX;
                markerList.back().tileZ = ppp->TileZ;
                markerList.back().x = ppp->X*2048-1024;
                markerList.back().z = ppp->Z*2048-1024;
            }
        }
        
        reader.readNext();
    }
    loaded = true;
}

CoordsKml::~CoordsKml() {
}

