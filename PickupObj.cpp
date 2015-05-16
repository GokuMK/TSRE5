/* 
 * File:   PickupObj.cpp
 * Author: Goku
 * 
 * Created on 16 maja 2015, 08:45
 */

#include "PickupObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>

PickupObj::PickupObj() {
    this->shape = -1;
    this->loaded = false;
}

PickupObj::PickupObj(const PickupObj& orig) {
}

PickupObj::~PickupObj() {
}

void PickupObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->skipLevel = 1;
    this->modified = false;
    
    setMartix();
}

void PickupObj::set(QString sh, FileBuffer* data) {
    if (sh == ("speedrange")) {
        speedRange[0] = ParserX::parsujr(data);
        speedRange[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("pickuptype")) {
        pickupType[0] = ParserX::parsujr(data);
        pickupType[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("pickupanimdata")) {
        pickupAnimData[0] = ParserX::parsujr(data);
        pickupAnimData[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("pickupcapacity")) {
        pickupCapacity[0] = ParserX::parsujr(data);
        pickupCapacity[1] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
        return;
    }
    if (sh == ("filename")) {
        fileName = ParserX::odczytajtc(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void PickupObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {

};

void PickupObj::save(QTextStream* out){
    if (!loaded) return;

*(out) << "	Pickup (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		SpeedRange ( "<<this->speedRange[0]<<" "<<this->speedRange[1]<<" )\n";
*(out) << "		PickupType ( "<<this->pickupType[0]<<" "<<this->pickupType[1]<<" )\n";
*(out) << "		PickupAnimData ( "<<this->pickupAnimData[0]<<" "<<this->pickupAnimData[1]<<" )\n";
*(out) << "		PickupCapacity ( "<<this->pickupCapacity[0]<<" "<<this->pickupCapacity[1]<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[0]<<" "<<this->trItemId[1]<<" )\n";
*(out) << "		CollideFlags ( "<<this->collideFlags<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}