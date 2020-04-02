/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ErrorMessage.h"
#include "GeoCoordinates.h"

ErrorMessage::ErrorMessage() {
}

ErrorMessage::ErrorMessage(QString type, QString source, QString description) {
    this->type = type;
    this->source = source;
    this->description = description;
}

ErrorMessage::ErrorMessage(QString type, QString source, QString description, QString action) {
    this->type = type;
    this->source = source;
    this->description = description;
    this->action = action;
}

ErrorMessage::ErrorMessage(const ErrorMessage& orig) {
    type = orig.type;
    source = orig.source;
    description = orig.description;
    action = orig.action;
}

ErrorMessage::~ErrorMessage() {
}

void ErrorMessage::setLocation(PreciseTileCoordinate* c){
    coords = c;
}

void ErrorMessage::setObject(GameObj* o){
    obj = o;
}

void ErrorMessage::setLocationXYZ(float wX, float wZ, float x, float y, float z){
    if(coords == NULL)
        coords = new PreciseTileCoordinate();
    coords->TileX = wX;
    coords->TileZ = wZ;
    coords->wX = x;
    coords->wY = y;
    coords->wZ = z;
}

