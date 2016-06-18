#include "CoordsGpx.h"
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

CoordsGpx::CoordsGpx(QString path) {
    qDebug() << "gpx!";
    loaded = false;

    QFile file(path);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "no file" << file.errorString();
        return;
    }
    qDebug() <<  "file";
    QByteArray data = file.readAll();
    
    bool placemark = false;
    bool line = false;
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
            
            if (name.toUpper() == ("WPT")) {
                placemark = true;
                markerList.emplace_back();
                float lat = 0;
                float lon = 0;
                if(attr.size() == 2){
                    if(attr[0].name().toString().toUpper() == "LAT")
                        lat = attr[0].value().toFloat();
                    if(attr[0].name().toString().toUpper() == "LON")
                        lon = attr[0].value().toFloat();
                    if(attr[1].name().toString().toUpper() == "LAT")
                        lat = attr[1].value().toFloat();
                    if(attr[1].name().toString().toUpper() == "LON")
                        lon = attr[1].value().toFloat();
                    igh = MstsCoordinates::ConvertToIgh(lat, lon);
                    ppp = MstsCoordinates::ConvertToTile(igh);
                    markerList.back().tileX.push_back(ppp->TileX);
                    markerList.back().tileZ.push_back(ppp->TileZ);
                    markerList.back().x.push_back(ppp->X*2048-1024);
                    markerList.back().y.push_back(0);
                    markerList.back().z.push_back(ppp->Z*2048-1024);
                }
            } else if (name.toUpper() == ("TRK")) {
                placemark = true;
                markerList.emplace_back();
            } else if (name.toUpper() == ("TRKPT")) {
                float lat = 0;
                float lon = 0;
                if(attr.size() == 2){
                    if(attr[0].name().toString().toUpper() == "LAT")
                        lat = attr[0].value().toFloat();
                    if(attr[0].name().toString().toUpper() == "LON")
                        lon = attr[0].value().toFloat();
                    if(attr[1].name().toString().toUpper() == "LAT")
                        lat = attr[1].value().toFloat();
                    if(attr[1].name().toString().toUpper() == "LON")
                        lon = attr[1].value().toFloat();
                    igh = MstsCoordinates::ConvertToIgh(lat, lon);
                    ppp = MstsCoordinates::ConvertToTile(igh);
                    markerList.back().tileX.push_back(ppp->TileX);
                    markerList.back().tileZ.push_back(ppp->TileZ);
                    markerList.back().x.push_back(ppp->X*2048-1024);
                    markerList.back().y.push_back(0);
                    markerList.back().z.push_back(ppp->Z*2048-1024);
                }
            } else if (name.toUpper() == ("NAME")) {
                if(placemark){
                    nametext = true;
                }
            }
        } else if (reader.isEndElement()) {
            name = reader.name().toString();
            if (name.toUpper() == ("WPT")) {
                placemark = false;
            }
            if (name.toUpper() == ("TRK")) {
                placemark = false;
            } 
            if (name.toUpper() == ("NAME")) {
                if(placemark){
                    nametext = false;
                }
            }
        } else if (reader.isCharacters()) {
            if(nametext){
                //qDebug() << reader.text();
                markerList.back().name = reader.text().toString();
            }
        }
        
        reader.readNext();
    }
    loaded = true;
}

CoordsGpx::~CoordsGpx() {
}

