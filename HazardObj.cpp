#include "HazardObj.h"
#include "ParserX.h"
#include <QDebug>
#include "GLMatrix.h"
#include "TexLib.h"
#include "GLUU.h"
#include "ShapeLib.h"

HazardObj::HazardObj() {
}

HazardObj::HazardObj(const HazardObj& orig) {
}

HazardObj::~HazardObj() {
}

void HazardObj::load(int x, int y) {
    this->shape = ShapeLib::addShape(resPath, fileName);
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 1;

    setMartix();
}

void HazardObj::set(QString sh, FileBuffer* data) {
    if (sh == ("filename")) {
        fileName = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[0] = ParserX::parsujr(data);
        trItemId[1] = ParserX::parsujr(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void HazardObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {

};

void HazardObj::save(QTextStream* out){
    if (!loaded) return;

*(out) << "	Hazard (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		TrItemId ( "<<this->trItemId[0]<<" "<<this->trItemId[1]<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}