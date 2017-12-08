/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GeoTerrainFile.h"
#include <QImage>

GeoTerrainFile::GeoTerrainFile() {
}

GeoTerrainFile::~GeoTerrainFile() {
}

bool GeoTerrainFile::load(int lat, int lon){
    return false;
}

bool GeoTerrainFile::isLoaded(){
    return false;
}

void GeoTerrainFile::draw(QImage* &image){

}

float GeoTerrainFile::getHeight(float lat, float lon){
    return 0;
}