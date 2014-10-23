#include "WorldObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

QString WorldObj::attributes[1] = {""};

WorldObj::WorldObj() {
    this->shape = -1;
    this->loaded = false;
}

WorldObj::WorldObj(const WorldObj& orig) {
}

WorldObj::~WorldObj() {
}

void WorldObj::load(QString path, int x, int y){
    this->x = x;
    this->y = y;
    this->loaded = false;
}

bool WorldObj::isAttribute(QString){
    return false;
}

void WorldObj::render(GLUU* gluu){
}

void WorldObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov){
}