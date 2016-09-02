/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
            
            if (name.toUpper() == ("PLACEMARK")) {
                placemark = true;
                markerList.emplace_back();
            } else if (name.toUpper() == ("PLACEMARK")) {
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
                //qDebug() << reader.text();
                markerList.back().name = reader.text().toString();
            }
            if(coordinates){
                //qDebug() << reader.text();
                QStringList cl = reader.text().toString().split(" ");
                QStringList c = cl[0].split(",");
                if(c.length() > 1){
                    markerList.back().lat = c[1].toFloat();
                    markerList.back().lon = c[0].toFloat();
                    markerList.back().type = 0;
                    float lat;
                    float lon;
                        for(int i = 0; i < cl.length(); i++){
                            c = cl[i].split(",");
                            if(c.length() < 2) continue;
                            lat = c[1].toFloat();
                            lon = c[0].toFloat();
                            igh = MstsCoordinates::ConvertToIgh(lat, lon);
                            ppp = MstsCoordinates::ConvertToTile(igh);
                            markerList.back().tileX.push_back(ppp->TileX);
                            markerList.back().tileZ.push_back(ppp->TileZ);
                            markerList.back().x.push_back(ppp->X*2048-1024);
                            markerList.back().y.push_back(0);
                            markerList.back().z.push_back(ppp->Z*2048-1024);
                            //markerList.back().pointsX.push_back((ppp->X*2048-1024) - 2048*(markerList.back().tileX - ppp->TileX));
                            //markerList.back().pointsZ.push_back((ppp->Z*2048-1024) + 2048*(markerList.back().tileZ - ppp->TileZ));
                            //markerList.back().pointsY.push_back(0);
                        }
                }
            }
        }
        
        reader.readNext();
    }
    loaded = true;
}

CoordsKml::~CoordsKml() {
}

