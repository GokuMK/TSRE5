#include "WorldObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "ParserX.h"
#include "GLMatrix.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


WorldObj::WorldObj() {
    this->shape = -1;
    this->loaded = false;
    this->selected = false;
}

WorldObj::WorldObj(const WorldObj& orig) {
}

WorldObj::~WorldObj() {
}

void WorldObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->loaded = false;
    this->selected = false;
}

void WorldObj::set(QString sh, FileBuffer* data) {
    if (sh == ("uid")) {
        UiD = ParserX::parsujr(data);
    }
    if (sh == ("staticflags")) {
        staticFlags = ParserX::parsuj16(data);
    }
    if (sh == ("position")) {
        position[0] = ParserX::parsujr(data);
        position[1] = ParserX::parsujr(data);
        position[2] = ParserX::parsujr(data);
        jestPQ++;
    }
    if (sh == ("qdirection")) {
        qDirection[0] = ParserX::parsujr(data);
        qDirection[1] = ParserX::parsujr(data);
        qDirection[2] = ParserX::parsujr(data);
        qDirection[3] = ParserX::parsujr(data);
        jestPQ++;
    }
    if (sh == ("vdbid")) {
        vDbId = ParserX::parsujr(data);
    }
    if (sh == ("staticdetaillevel")) {
        staticDetailLevel = ParserX::parsujr(data);
    }
    return;
}

void WorldObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
}

void WorldObj::setMartix(){
    Mat4::fromRotationTranslation(this->matrix, qDirection, position);
    Mat4::rotate(this->matrix, this->matrix, M_PI, 0, -1, 0);
}

void WorldObj::translate(float px, float py, float pz){
    this->position[0]+=px;
    this->position[1]+=py;
    this->position[2]+=pz;
    
    setMartix();
}

void WorldObj::setPosition(float* p){
    this->position[0] = p[0];
    this->position[1] = p[1];
    this->position[2] = p[2];
}

void WorldObj::setNewQdirection(){
    this->qDirection[0] = 0;
    this->qDirection[1] = 0;
    this->qDirection[2] = 0;
    this->qDirection[3] = 1;
}

void WorldObj::setQdirection(float* q){
    this->qDirection[0] = q[0];
    this->qDirection[1] = q[1];
    this->qDirection[2] = q[2];
    this->qDirection[3] = q[3];
}

void WorldObj::initPQ(float* p){
    this->position[0] = p[0];
    this->position[1] = p[1];
    this->position[2] = -p[2];
    this->qDirection[0] = 0;
    this->qDirection[1] = 0;
    this->qDirection[2] = 0;
    this->qDirection[3] = 1;
    this->jestPQ = 2;
}

void WorldObj::save(QTextStream out){
    
}