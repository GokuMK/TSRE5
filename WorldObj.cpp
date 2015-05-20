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
    this->modified = false;
    this->tRotation[0] = 0;
    this->tRotation[1] = 0;
}

WorldObj::WorldObj(const WorldObj& orig) {
}

WorldObj::~WorldObj() {
}

bool WorldObj::allowNew(){
    return false;
}

void WorldObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->loaded = false;
    this->selected = false;
}

void WorldObj::set(QString sh, int val) {

}

void WorldObj::set(QString sh, float* val) {

}

void WorldObj::set(QString sh, QString val) {

}

void WorldObj::set(QString sh, FileBuffer* data) {
    if (sh == ("uid")) {
        UiD = ParserX::parsujUint(data);
        return;
    }
    if (sh == ("staticflags")) {
        staticFlags = ParserX::parsuj16(data);
        return;
    }
    if (sh == ("position")) {
        position[0] = ParserX::parsujr(data);
        position[1] = ParserX::parsujr(data);
        position[2] = ParserX::parsujr(data);
        jestPQ++;
        return;
    }
    if (sh == ("qdirection")) {
        qDirection[0] = ParserX::parsujr(data);
        qDirection[1] = ParserX::parsujr(data);
        qDirection[2] = ParserX::parsujr(data);
        qDirection[3] = ParserX::parsujr(data);
        if(fabs(qDirection[0]) + fabs(qDirection[1]) + fabs(qDirection[2]) + fabs(qDirection[3]) < 3)
            jestPQ++;
        return;
    }
    if (sh == ("matrix3x3")) {
        matrix3x3 = new float[9];
        matrix3x3[0] = ParserX::parsujr(data);
        matrix3x3[1] = ParserX::parsujr(data);
        matrix3x3[2] = ParserX::parsujr(data);
        matrix3x3[3] = ParserX::parsujr(data);
        matrix3x3[4] = ParserX::parsujr(data);
        matrix3x3[5] = ParserX::parsujr(data);
        matrix3x3[6] = ParserX::parsujr(data);
        matrix3x3[7] = ParserX::parsujr(data);
        matrix3x3[8] = ParserX::parsujr(data);
        Quat::fromMat3((float*)&qDirection, matrix3x3);
        Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        //Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        jestPQ++;
        return;
    }
    if (sh == ("vdbid")) {
        //qDebug()<< ParserX::parsujr(data);
        vDbId = ParserX::parsujUint(data);
        return;
    }
    if (sh == ("staticdetaillevel")) {
        staticDetailLevel = ParserX::parsujr(data);
        return;
    }
    if (sh == ("collideflags")) {
        collideFlags = ParserX::parsujr(data);
        return;
    }
    qDebug() << "=" << sh;
    return;
}

void WorldObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
}

void WorldObj::deleteVBO(){
    
}

void WorldObj::setMartix(){
    Mat4::fromRotationTranslation(this->matrix, qDirection, position);
    Mat4::rotate(this->matrix, this->matrix, M_PI, 0, -1, 0);
}

void WorldObj::translate(float px, float py, float pz){
    this->position[0]+=px;
    this->position[1]+=py;
    this->position[2]+=pz;
    this->modified = true;
    setMartix();
}

void WorldObj::rotate(float x, float y, float z){
    this->tRotation[0] += x;
    this->tRotation[1] += y;
    if(matrix3x3 != NULL) matrix3x3 = NULL;
    if(x!=0) Quat::rotateX(this->qDirection, this->qDirection, x);
    if(y!=0) Quat::rotateY(this->qDirection, this->qDirection, y);
    if(z!=0) Quat::rotateZ(this->qDirection, this->qDirection, z);
    this->modified = true;
    setMartix();
}

void WorldObj::resize(float x, float y, float z){
    
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

void WorldObj::initPQ(float* p, float* q){
    this->position[0] = p[0];
    this->position[1] = p[1];
    this->position[2] = -p[2];
    this->qDirection[0] = q[0];
    this->qDirection[1] = q[1];
    this->qDirection[2] = -q[2];
    this->qDirection[3] = q[3];
    this->jestPQ = 2;
}

void WorldObj::save(QTextStream* out){
    
}

Ref::RefItem* WorldObj::getRefInfo(){
    Ref::RefItem* r = new Ref::RefItem();
    r->type = this->type;
    r->filename = this->fileName;
    return r;
}

bool WorldObj::getBorder(float* border){
    return false;
}

void WorldObj::drawBox(){

    if (!box.loaded) {
        float bound[6];
        if (!getBorder((float*)&bound)) return;
        
        float* punkty = new float[72];
        float* ptr = punkty;
        
        for(int i=0; i<2; i++)
            for(int j=4; j<6; j++){
                *ptr++ = bound[i];
                *ptr++ = bound[2];
                *ptr++ = bound[j];
                *ptr++ = bound[i];
                *ptr++ = bound[3];
                *ptr++ = bound[j];
            }
        for(int i=0; i<2; i++)
            for(int j=2; j<4; j++){
                *ptr++ = bound[i];
                *ptr++ = bound[j];
                *ptr++ = bound[4];
                *ptr++ = bound[i];
                *ptr++ = bound[j];
                *ptr++ = bound[5];
            }
        for(int i=4; i<6; i++)
            for(int j=2; j<4; j++){
                *ptr++ = bound[0];
                *ptr++ = bound[j];
                *ptr++ = bound[i];
                *ptr++ = bound[1];
                *ptr++ = bound[j];
                *ptr++ = bound[i];
            }

        box.setMaterial(0.0, 0.0, 1.0);
        box.init(punkty, ptr-punkty, box.V, GL_LINES);

        delete punkty;
    }
    

    box.render();
};

bool WorldObj::select(){
    this->selected = true;
}

bool WorldObj::unselect(){
    this->selected = false;
}

bool WorldObj::isSelected(){
    return this->selected;
}
