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
}

WorldObj::WorldObj(const WorldObj& orig) {
}

WorldObj::~WorldObj() {
}

void WorldObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->loaded = false;
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

void WorldObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov) {
}