#include "PlatformObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

PlatformObj::PlatformObj() {
    this->shape = -1;
    this->loaded = false;
}

PlatformObj::PlatformObj(const PlatformObj& orig) {
}

PlatformObj::~PlatformObj() {
}

void PlatformObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->skipLevel = 1;
    
    setMartix();
}

void PlatformObj::set(QString sh, FileBuffer* data) {
    if (sh == ("sidingdata") || sh == ("platformdata") ) {
        platformData = ParserX::parsuj16(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
        trItemId[trItemIdCount++] = ParserX::parsujr(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void PlatformObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {

};

void PlatformObj::save(QTextStream* out){
    if (!loaded) return;
int l;
QString flags;
    flags = QString::number(this->staticFlags, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++)
        flags = "0"+flags;
    
QString flags2;
    flags2 = QString::number(this->platformData, 16);
    l = flags2.length();
    for(int i=0; i<8-l; i++)
        flags2 = "0"+flags2; 
    
if(type == "siding")
*(out) << "	Siding (\n";
if(type == "platform")
*(out) << "	Platform (\n";

*(out) << "		UiD ( "<<this->UiD<<" )\n";
if(type == "siding")
*(out) << "		SidingData ( "<<flags2<<" )\n";
if(type == "platform")
*(out) << "		PlatformData ( "<<flags2<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[0]<<" "<<this->trItemId[1]<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[2]<<" "<<this->trItemId[3]<<" )\n";
*(out) << "		StaticFlags ( "<<flags<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}