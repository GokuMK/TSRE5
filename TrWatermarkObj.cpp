#include "TrWatermarkObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include "Game.h"
#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

TrWatermarkObj::TrWatermarkObj() {
    this->shape = -1;
    this->UiD = 0;
    this->dstLevel = 0;
    this->loaded = false;
}

TrWatermarkObj::TrWatermarkObj(int level) {
    this->UiD = 0;
    this->dstLevel = level;
    this->shape = -1;
    this->loaded = false;
    this->modified = false;
}

TrWatermarkObj::TrWatermarkObj(const TrWatermarkObj& orig) {
}

TrWatermarkObj::~TrWatermarkObj() {
}

void TrWatermarkObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->loaded = true;
    this->skipLevel = 1;
}

void TrWatermarkObj::set(QString sh, FileBuffer* data) {
    return;
}

void TrWatermarkObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    return;
};

void TrWatermarkObj::save(QTextStream* out){
    if (!loaded) return;
    if (Game::deleteTrWatermarks) return;
    
*(out) << "	Tr_Watermark ( "<<this->dstLevel<<" )\n";
}