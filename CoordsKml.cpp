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
#include "GeoCoordinates.h"

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
    bool styletext = false;
    bool styleobj = false;
    QString styleName = "";
    bool colortext = false;
    int iii = 0, uuu = 0;
    IghCoordinate* igh;
    PreciseTileCoordinate* ppp;
    
    QXmlStreamReader reader((data));
    reader.readNext();
    QString name;
    QXmlStreamAttributes attr;
    while (!reader.isEndDocument()) {
        //qDebug() << reader.name().toString();
        if (reader.isStartElement()) {
            name = reader.name().toString();
            attr = reader.attributes();
            
            if (name.toUpper() == ("PLACEMARK")) {
                placemark = true;
                markerList.push_back(Marker());
            } else if (name.toUpper() == ("PLACEMARK")) {
                placemark = true;
                markerList.push_back(Marker());
            } else if (name.toUpper() == ("COORDINATES")) {
                if(placemark){
                    coordinates = true;
                    //if(Game::markerLines){
                    //    if(markerList.back().tileX.size() > 1)
                    //        markerList.push_back(Marker());
                    //}
                }
            } else if (name.toUpper() == ("LINESTRING") || name.toUpper() == ("LINEARRING") ) {
                if(placemark){
                    if(markerList.back().tileX.size() > 0)
                        markerList.back().segmentPtr[markerList.back().tileX.size()] = markerList.back().tileX.size();
                        //markerList.back().segmentPtr.push_back(markerList.back().tileX.size());
                }
            } else if (name.toUpper() == ("NAME")) {
                if(placemark){
                    nametext = true;
                }
            } else if (name.toUpper() == ("STYLEURL")) {
                if(placemark){
                    styletext = true;
                }
            } else if (name.toUpper() == ("STYLE")) {
                    styleobj = true;
                    if(reader.attributes().size() > 0){
                        for(iii = 0; iii < reader.attributes().size(); iii++){
                            //qDebug() << reader.attributes()[iii].name() << reader.attributes()[iii].value();
                            if(reader.attributes()[iii].name() == "id")
                                styleName = reader.attributes()[iii].value().toString();
                        }
                                
                    }
            } else if (name.toUpper() == ("COLOR")) {
                if(styleobj){
                    colortext = true;
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
            } else if (name.toUpper() == ("STYLEURL")) {
                if(placemark){
                    styletext = false;
                }
            } else if (name.toUpper() == ("STYLE")) {
                    styleobj = false;
            } else if (name.toUpper() == ("COLOR")) {
                if(styleobj){
                    colortext = false;
                }
            }

        } else if (reader.isCharacters()) {
            if(nametext){
                //qDebug() << reader.text();
                markerList.back().name = reader.text().toString();
            }
            if(styletext){
                //qDebug() << reader.text();
                markerList.back().style = reader.text().toString().trimmed().replace("#","");
            }
            if(colortext){
                //qDebug() << reader.text();
                style[styleName].color = QString("#") + reader.text().toString().trimmed();
                //qDebug() << style[styleName].color;
            }
            if(coordinates){
                //qDebug() << reader.text();
                QStringList cl = reader.text().toString().trimmed().split(" ");
                QStringList c = cl[0].split(",");
                if(c.length() > 1){
                    markerList.back().lat = c[1].toFloat();
                    markerList.back().lon = c[0].toFloat();
                    markerList.back().type = 0;
                    //qDebug() << markerList.back().lat << markerList.back().lon;
                    float lat;
                    float lon;
                        for(int i = 0; i < cl.length(); i++){
                            c = cl[i].split(",");
                            if(c.length() < 2) continue;
                            lat = c[1].toFloat();
                            lon = c[0].toFloat();
                            igh = Game::GeoCoordConverter->ConvertToInternal(lat, lon);
                            ppp = Game::GeoCoordConverter->ConvertToTile(igh);
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

