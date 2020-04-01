/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "WorldObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "ParserX.h"
#include "GLMatrix.h"
#include <math.h>
#include <QString>
#include "StaticObj.h"
#include "DynTrackObj.h"
#include "ForestObj.h"
#include "TransferObj.h"
#include "TrackObj.h"
#include "SpeedpostObj.h"
#include "SignalObj.h"
#include "PlatformObj.h"
#include "TrWatermarkObj.h"
#include "LevelCrObj.h"
#include "PickupObj.h"
#include "HazardObj.h"
#include "RulerObj.h"
#include "SoundRegionObj.h"
#include "SoundSourceObj.h"
#include "CarSpawnerObj.h"
#include "Game.h"
#include "TS.h"
#include "TerrainLib.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

TrackItemObj* WorldObj::pointer3d = NULL;

void WorldObj::loadingFixes(){

}

int WorldObj::isTrackObj(QString sh) {
    if (sh == "signal") {
        return 1;
    } else if (sh == "speedpost") {
        return 1;
    } else if (sh == "collideobject") {
        return 0;
    } else if (sh == "platform") {
        return 1;
    } else if (sh == "siding") {
        return 1;
    } else if (sh == "carspawner") {
        return 2;
    } else if (sh == "levelcr") {
        return 3;
    } else if (sh == "pickup") {
        return 1;
    } else if (sh == "hazard") {
        return 1;
    } else if (sh == "soundregion") {
        return 1;
    } 
    return 0;
}


WorldObj* WorldObj::createObj(int sh) {
    WorldObj* nowy;
    if (sh == TS::Static) {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->sstatic;
    } else if(sh == TS::Signal) {
        nowy = (WorldObj*) (new SignalObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->signal;
    } else if (sh == TS::Speedpost) {
        nowy = (WorldObj*) (new SpeedpostObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->speedpost;
    } else if (sh == TS::TrackObj) {
        nowy = (WorldObj*) (new TrackObj());
        (nowy)->resPath = Game::root + "/global/shapes";
        (nowy)->typeID = (nowy)->trackobj;
    } else if (sh == TS::Gantry || sh == TS::Gantry2) {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->gantry;
    } else if (sh == TS::CollideObject) {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->collideobject;
    } else if (sh == TS::Dyntrack || sh == TS::DynTrack2) {
        nowy = (WorldObj*) (new DynTrackObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->dyntrack;
    } else if (sh == TS::Forest) {
        nowy = (WorldObj*) (new ForestObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->forest;
    } else if (sh == TS::Transfer || sh == TS::Transfer2) {
        nowy = (WorldObj*) (new TransferObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->transfer;
    } else if (sh == TS::Platform) {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->platform;
    } else if (sh == TS::Siding || sh == TS::Siding2) {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->siding;
    } else if (sh == TS::CarSpawner || sh == TS::CarSpawner2) {
        nowy = (WorldObj*) (new CarSpawnerObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";     
        (nowy)->typeID = (nowy)->carspawner;
    } else if (sh == TS::LevelCr) {
        nowy = (WorldObj*) (new LevelCrObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";   
        (nowy)->typeID = (nowy)->levelcr;
    } else if (sh == TS::Pickup || sh == TS::Pickup2) {
        nowy = (WorldObj*) (new PickupObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->pickup;
    } else if (sh == TS::Hazard) {
        nowy = (WorldObj*) (new HazardObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->hazard;
    }/* else if (sh == TS::) {
        nowy = (WorldObj*) (new SoundSourceObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->soundsource;
    } else if (sh == TS::) {
        nowy = (WorldObj*) (new SoundRegionObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->soundregion;
    }*/ else {
        qDebug() << " Unsupported WorldObj !!! " << sh;
        //(*nowy) = new WorldObj();
        return NULL;
        //
    }
    (nowy)->type = TS::IdName[sh];
    return nowy;
}

WorldObj* WorldObj::createObj(QString sh) {
    WorldObj* nowy;
    if (sh == "static") {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->sstatic;
    } else if (sh == "signal") {
        nowy = (WorldObj*) (new SignalObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->signal;
    } else if (sh == "speedpost") {
        nowy = (WorldObj*) (new SpeedpostObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->speedpost;
    } else if (sh == "trackobj") {
        nowy = (WorldObj*) (new TrackObj());
        (nowy)->resPath = Game::root + "/global/shapes";
        (nowy)->typeID = (nowy)->trackobj;
    } else if (sh == "gantry") {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->gantry;
    } else if (sh == "collideobject") {
        nowy = (WorldObj*) (new StaticObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->collideobject;
    } else if (sh == "dyntrack") {
        nowy = (WorldObj*) (new DynTrackObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->dyntrack;
    } else if (sh == "forest") {
        nowy = (WorldObj*) (new ForestObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->forest;
    } else if (sh == "transfer") {
        nowy = (WorldObj*) (new TransferObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/textures";
        (nowy)->typeID = (nowy)->transfer;
    } else if (sh == "platform") {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";
        (nowy)->typeID = (nowy)->platform;
    } else if (sh == "siding") {
        nowy = (WorldObj*) (new PlatformObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->siding;
    } else if (sh == "carspawner") {
        nowy = (WorldObj*) (new CarSpawnerObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";     
        (nowy)->typeID = (nowy)->carspawner;
    } else if (sh == "levelcr") {
        nowy = (WorldObj*) (new LevelCrObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";   
        (nowy)->typeID = (nowy)->levelcr;
    } else if (sh == "pickup") {
        nowy = (WorldObj*) (new PickupObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";       
        (nowy)->typeID = (nowy)->pickup;
    } else if (sh == "hazard") {
        nowy = (WorldObj*) (new HazardObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->hazard;
    } else if (sh == "soundsource") {
        nowy = (WorldObj*) (new SoundSourceObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->soundsource;
    } else if (sh == "soundregion") {
        nowy = (WorldObj*) (new SoundRegionObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->soundregion;
    } else if (sh == "ruler") {
        nowy = (WorldObj*) (new RulerObj());
        (nowy)->resPath = Game::root + "/routes/" + Game::route + "/shapes";    
        (nowy)->typeID = (nowy)->ruler;
    } else {
        qDebug() << " Unsupported WorldObj !!! " + sh;
        //(*nowy) = new WorldObj();
        return NULL;
        //
    }
    (nowy)->type = sh;
    return nowy;
}

QString WorldObj::getResPath(Ref::RefItem* sh) {
    if (sh->type == "static") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->getShapeName();
    } else if (sh->type == "signal") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->getShapeName();
    } else if (sh->type == "speedpost") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->getShapeName();
    } else if (sh->type == "trackobj") {
        return Game::root + "/global/shapes/"+sh->getShapeName();
    } else if (sh->type == "gantry") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->getShapeName();
    } else if (sh->type == "collideobject") {
        return Game::root + "/routes/" + Game::route + "/shapes/"+sh->getShapeName();
    } else {
        return "";
    }
    return "";
}

WorldObj::WorldObj() {
    this->typeObj = this->worldobj;
    this->shape = -1;
    this->loaded = false;
    this->selected = false;
    this->modified = false;
    this->tRotation[0] = 0;
    this->tRotation[1] = 0;
}

WorldObj::WorldObj(const WorldObj& o) {
    typeObj = o.typeObj;
    UiD = o.UiD;
    typeID = o.typeID;
    fileName = o.fileName;
    Vec3::copy(position, (float*)o.position);
    Vec3::copy(placedAtPosition, (float*)o.placedAtPosition);
    Vec3::copy(firstPosition, (float*)o.firstPosition);
    Quat::copy(qDirection, (float*)o.qDirection);
    memcpy(matrix, o.matrix, sizeof(float)*16);
    staticDetailLevel = o.staticDetailLevel;
    staticFlags = o.staticFlags;
    vDbId = o.vDbId;
    collideFlags = o.collideFlags;
    collideFunction = o.collideFunction;
    type = o.type;
    resPath = o.resPath;
    skipLevel = o.skipLevel;
    x = o.x;
    y = o.y;
    shape = o.shape;
    shapePointer = o.shapePointer;
    loaded = o.loaded;
    size = o.size;
    jestPQ = o.jestPQ;
    modified = o.modified;
    selectionColor.x = o.selectionColor.x;
    selectionColor.y = o.selectionColor.y;
    selectionColor.z = o.selectionColor.z;
    snapable = o.snapable;
    Vec2::copy(tRotation, (float*)o.tRotation);
    endp = o.endp;
    templateName = o.templateName;
    sectionIdx = o.sectionIdx;
    trLoaded = o.trLoaded;
    if(o.matrix3x3 != NULL){
        matrix3x3 = new float[9];
        memcpy(matrix3x3, o.matrix3x3, sizeof(float)*9);
    }
        
}

WorldObj* WorldObj::clone(){
    return new WorldObj(*this);
}

WorldObj::~WorldObj() {
}

bool WorldObj::allowNew(){
    return false;
}

bool WorldObj::isTrackItem(){
    return false;
}

bool WorldObj::isSoundItem(){
    return false;
}

bool WorldObj::hasLinePoints(){
    return false;
}

void WorldObj::getLinePoints(float *&punkty){
    return;
}

bool WorldObj::isInternalLodControl(){
    return internalLodControl;
}

void WorldObj::setInternalLodControl(bool val){
    internalLodControl = val;
}

// Use this function to init object if loaded before route data.
void WorldObj::loadInit(){
    
}

int WorldObj::updateTrackSectionInfo(QHash<int,int> shapes, QHash<int,int> sections){
    return 0;
}

void WorldObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->loaded = false;
    this->selected = false;
}

void WorldObj::set(QString sh, long long int val) {
    if (sh == ("x")) {
        this->x = val;
        return;
    }
    if (sh == ("z")) {
        this->y = val;
        return;
    }
    if (sh == ("staticflags")) {
        staticFlags = val;
        return;
    }
}

void WorldObj::set(QString sh, float* val) {

}

void WorldObj::set(QString sh, QString val) {

}

void WorldObj::set(int sh, FileBuffer* data) {
    if (sh == TS::UiD) {
        data->off++;
        UiD = data->getUint();
        return;
    }
    if (sh == TS::StaticFlags) {
        data->off++;
        staticFlags = data->getUint();
        return;
    }
    if (sh == TS::Position) {
        data->off++;
        position[0] = data->getFloat();
        position[1] = data->getFloat();
        position[2] = data->getFloat();
        jestPQ++;
        return;
    }
    if (sh == TS::QDirection) {
        data->off++;
        qDirection[0] = data->getFloat();
        qDirection[1] = data->getFloat();
        qDirection[2] = data->getFloat();
        qDirection[3] = data->getFloat();
        if(fabs(qDirection[0]) + fabs(qDirection[1]) + fabs(qDirection[2]) + fabs(qDirection[3]) < 3)
            jestPQ++;
        return;
    }
    if (sh == TS::Matrix3x3) {
        data->off++;
        matrix3x3 = new float[9];
        matrix3x3[0] = data->getFloat();
        matrix3x3[1] = data->getFloat();
        matrix3x3[2] = data->getFloat();
        matrix3x3[3] = data->getFloat();
        matrix3x3[4] = data->getFloat();
        matrix3x3[5] = data->getFloat();
        matrix3x3[6] = data->getFloat();
        matrix3x3[7] = data->getFloat();
        matrix3x3[8] = data->getFloat();
        Quat::fromMat3((float*)&qDirection, matrix3x3);
        Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        //Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        jestPQ++;
        return;
    }
    if (sh == TS::VDbId) {
        data->off++;
        vDbId = data->getUint();
        return;
    }
    if (sh == TS::StaticDetailLevel) {
        data->off++;
        staticDetailLevel = data->getUint();
        return;
    }
    if (sh == TS::CollideFlags) {
        data->off++;
        collideFlags = data->getUint();
        return;
    }
    if (sh == TS::CollideFunction) {
        data->off++;
        collideFunction = data->getUint();
        return;
    }
    if (sh == TS::MaxVisDistance) {
        data->off++;
        // 4byte undefined data
        return;
    }
    qDebug() << "worldObj "<<this->type<<" unknown: " << sh;
    return;
}

void WorldObj::set(QString sh, FileBuffer* data) {
    if (sh == ("uid")) {
        UiD = ParserX::GetUInt(data);
        return;
    }
    if (sh == ("staticflags")) {
        staticFlags = ParserX::GetHex(data);
        return;
    }
    if (sh == ("position")) {
        position[0] = ParserX::GetNumber(data);
        position[1] = ParserX::GetNumber(data);
        position[2] = ParserX::GetNumber(data);
        jestPQ++;
        return;
    }
    if (sh == ("qdirection")) {
        qDirection[0] = ParserX::GetNumber(data);
        qDirection[1] = ParserX::GetNumber(data);
        qDirection[2] = ParserX::GetNumber(data);
        qDirection[3] = ParserX::GetNumber(data);
        if(fabs(qDirection[0]) + fabs(qDirection[1]) + fabs(qDirection[2]) + fabs(qDirection[3]) < 3)
            jestPQ++;
        return;
    }
    if (sh == ("matrix3x3")) {
        matrix3x3 = new float[9];
        matrix3x3[0] = ParserX::GetNumber(data);
        matrix3x3[1] = ParserX::GetNumber(data);
        matrix3x3[2] = ParserX::GetNumber(data);
        matrix3x3[3] = ParserX::GetNumber(data);
        matrix3x3[4] = ParserX::GetNumber(data);
        matrix3x3[5] = ParserX::GetNumber(data);
        matrix3x3[6] = ParserX::GetNumber(data);
        matrix3x3[7] = ParserX::GetNumber(data);
        matrix3x3[8] = ParserX::GetNumber(data);
        Quat::fromMat3((float*)&qDirection, matrix3x3);
        Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        //Vec4::normalize((float*)&qDirection,(float*)&qDirection);
        jestPQ++;
        return;
    }
    if (sh == ("vdbid")) {
        //qDebug()<< ParserX::GetNumber(data);
        vDbId = ParserX::GetUInt(data);
        return;
    }
    if (sh == ("staticdetaillevel")) {
        staticDetailLevel = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("collideflags")) {
        collideFlags = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("collidefunction")) {
        collideFunction = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("shapetemplate")) {
        templateName = ParserX::GetStringInside(data);
        return;
    }
    qDebug() << "worldObj "<<this->type<<" unknown: " << sh;
    return;
}

void WorldObj::updateSim(float deltaTime){
    
}

void WorldObj::reload(){
    
}

QString WorldObj::getTemplate(){
    return templateName;
}

void WorldObj::setTemplate(QString name){
    templateName = name;
    modified = true;
}
    
bool WorldObj::isSimilar(WorldObj* obj){
    return false;
}

void WorldObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {

}

void WorldObj::deleteVBO(){
    
}

void WorldObj::removedFromTDB(){
    
}

void WorldObj::snapped(int side){
    
}

void WorldObj::loadSnapablePoints(){
    
}

void WorldObj::setMartix(){
    Mat4::fromRotationTranslation(this->matrix, qDirection, position);
    Mat4::rotate(this->matrix, this->matrix, M_PI, 0, -1, 0);
}

void WorldObj::flip(bool flipShape){
    
}

void WorldObj::translate(float px, float py, float pz){
    this->position[0]+=px;
    this->position[1]+=py;
    this->position[2]+=pz;
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    this->modified = true;
    setMartix();
}

void WorldObj::randomTransform(Ref::RandomTransformation * transformation){
    if((transformation->reX - transformation->rbX) != 0){
        int range = abs(transformation->reX - transformation->rbX);
        float angle = (M_PI*(transformation->rbX + (std::rand()%range)))/180.0;
        Quat::rotateX(this->qDirection, this->qDirection, angle);
    }
    if((transformation->reY - transformation->rbY) != 0){
        int range = abs(transformation->reY - transformation->rbY);
        float angle = (M_PI*(transformation->rbY + (std::rand()%range)))/180.0;
        Quat::rotateY(this->qDirection, this->qDirection, angle);
    }
    if((transformation->reZ - transformation->rbZ) != 0){
        int range = abs(transformation->reZ - transformation->rbZ);
        float angle = (M_PI*(transformation->rbZ + (std::rand()%range)))/180.0;
        Quat::rotateZ(this->qDirection, this->qDirection, angle);
    }
    ///
    if((transformation->tbX - transformation->teX) != 0){
        int range = abs(transformation->teX - transformation->tbX);
        float translation = transformation->tbX + ((float)((std::rand()%1000))/1000)*range;
        this->position[0] += translation;
        this->placedAtPosition[0] = this->position[0];
    }
    if((transformation->tbY - transformation->teY) != 0){
        float range = abs(transformation->teY - transformation->tbY);
        float translation = transformation->tbY + ((float)((std::rand()%1000))/1000)*range;
        this->position[1] += translation;
        this->placedAtPosition[1] = this->position[1];
    }
    if((transformation->tbZ - transformation->teZ) != 0){
        float range = abs(transformation->teZ - transformation->tbZ);
        float translation = transformation->tbZ + ((float)((std::rand()%1000))/1000)*range;
        this->position[2] += translation;
        this->placedAtPosition[2] = this->position[2];
    }
    if(matrix3x3 != NULL) matrix3x3 = NULL;
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

void WorldObj::setPosition(int x, int z, float* p){
    this->position[0] = -2048*(this->x-x) + p[0];
    this->position[1] = p[1];
    this->position[2] = -2048*(this->y-z) + p[2];
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    this->modified = true;
    deleteVBO();
}

void WorldObj::setPosition(float* p){
    this->position[0] = p[0];
    this->position[1] = p[1];
    this->position[2] = p[2];
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    this->modified = true;
    deleteVBO();
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
    this->placedAtPosition[0] = this->position[0];
    this->placedAtPosition[1] = this->position[1];
    this->placedAtPosition[2] = this->position[2];
    this->firstPosition[0] = this->position[0];
    this->firstPosition[1] = this->position[1];
    this->firstPosition[2] = -this->position[2];
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
    r->filename.push_back(this->fileName);
    r->staticFlags = this->staticFlags;
    return r;
}

bool WorldObj::getSimpleBorder(float* border){
    return false;
}

void WorldObj::drawBox(){
    if (!box.loaded) {
        QVector<float> punkty;
        if (!this->getBoxPoints(punkty)) return;

        box.setMaterial(0.0, 0.0, 1.0);
        box.init((float*)&punkty[0], punkty.size(), box.V, GL_LINES);
        //box.setLineWidth(3);
    }
    
    box.render();
};

bool WorldObj::getBoxPoints(QVector<float>& points){
    return false;
}

bool WorldObj::select(){
    this->selected = true;
    return true;
}

bool WorldObj::select(int value){
    return select();
}

bool WorldObj::unselect(){
    this->selected = false;
    return false;
}

bool WorldObj::isSelected(){
    return this->selected;
}

void WorldObj::initTrItems(float *tpos){
    
}

void WorldObj::deleteTrItems(){
    
}

QString WorldObj::getShapePath(){
    return "";
}

float *WorldObj::getPosition(){
    return (float*)position;
}

float *WorldObj::getQuatRotation(){
    return (float*)qDirection;
}

int WorldObj::getDefaultDetailLevel(){
    return 0;
}

int WorldObj::getCustomDetailLevel(){
    return staticDetailLevel;
}

int WorldObj::getCurrentDetailLevel(){
    if(staticDetailLevel >= 10)
        return 10;
    if(staticDetailLevel >= 0)
        return staticDetailLevel;
    else
        return getDefaultDetailLevel();
}

void WorldObj::setCustomDetailLevel(int val){
    if(val < 0) val = -1;
    if(val > 10) val = 10;
    staticDetailLevel = val;
    this->modified = true;
}

bool WorldObj::customDetailLevelEnabled(){
    if(staticDetailLevel >= 0) return true;
    return false;
}

bool WorldObj::isAnimated(){
    if((staticFlags & 0x80000) == 0x80000)
        return true;
    return false;
}

bool WorldObj::isTerrainObj(){
    if((staticFlags & 0x40000) == 0x40000)
        return true;
    return false;
}

float WorldObj::getElevation(){
    float vect[3];
    vect[0] = 0; vect[1] = 0; vect [2] = 1000;
    Vec3::transformQuat(vect, vect, qDirection);
    return asin(-vect[1]/1000.0);
}

WorldObj::ShadowType WorldObj::getShadowType(){
    if((staticFlags & 0xFF000) == 0x0)
        return WorldObj::ShadowNone;
    if((staticFlags & 0x2000) == 0x2000)
        return WorldObj::ShadowRound;
    if((staticFlags & 0x4000) == 0x4000)
        return WorldObj::ShadowRect;
    if((staticFlags & 0x8000) == 0x8000)
        return WorldObj::ShadowTreeline;
    if((staticFlags & 0x10000) == 0x10000)
        return WorldObj::ShadowDynamic;
    return WorldObj::ShadowNone;
}

void WorldObj::setAnimated(bool val){
    if(val == true)
        staticFlags = staticFlags | 0x80000;
    else
        staticFlags = staticFlags & (~0x80000);
    
    if(shapePointer != NULL && shapeState > 0)
        shapePointer->setAnimated(shapeState, isAnimated());
    
    this->modified = true;
}

void WorldObj::setTerrainObj(bool val){
    if(val == true)
        staticFlags = staticFlags | 0x40000;
    else
        staticFlags = staticFlags & (~0x40000);
    this->modified = true;
}

void WorldObj::setShadowType(WorldObj::ShadowType val){
    this->modified = true;
    staticFlags = staticFlags & (~0x1E000);
    if(val == WorldObj::ShadowNone)
        return;
    else if(val == WorldObj::ShadowRound)
        staticFlags = staticFlags | 0x2000;
    else if(val == WorldObj::ShadowRect)
        staticFlags = staticFlags | 0x4000;
    else if(val == WorldObj::ShadowTreeline)
        staticFlags = staticFlags | 0x8000;
    else if(val == WorldObj::ShadowDynamic)
        staticFlags = staticFlags | 0x10000;
}

void WorldObj::setCollisionType(int val){
    this->modified = true;
    if(val < 0){
        collideFlags = collideFlags | (2);
        return;
    }
    collideFlags = collideFlags & (~2);
    collideFunction = val;
    return;
}

void WorldObj::adjustPositionToTerrain(){
    position[1] = Game::terrainLib->getHeight(x, y, position[0], position[2]);
    this->modified = true;
    setMartix();
}

void WorldObj::adjustRotationToTerrain(){
    float rot[2];
    Game::terrainLib->getRotation((float*)&rot, x, y, position[0], position[2]);
    this->tRotation[0] += rot[0];
    this->tRotation[1] += rot[1];
    if(matrix3x3 != NULL) matrix3x3 = NULL;
    float *q = Quat::create();
    
    float vect[3];
    vect[0] = 0; vect[1] = 0; vect [2] = 1;
    Vec3::transformQuat(vect, vect, qDirection);
    if(vect[2] < 0)
        vect[0] = -vect[0];
    if(rot[0]!=0) Quat::rotateX(q, q, -rot[0]);
    if(rot[1]!=0) Quat::rotateZ(q, q, rot[1]);
    Quat::rotateY(q, q, asin((vect[0])));
    if(vect[2] < 0)
        Quat::rotateY(q, q, M_PI);
    this->qDirection[0] = q[0];
    this->qDirection[1] = q[1];
    this->qDirection[2] = q[2];
    this->qDirection[3] = q[3];
    this->modified = true;
    setMartix();
}

int WorldObj::getCollisionType(){
    return 0;
}

int WorldObj::getCollisionFlags(){
    return collideFlags;
}

void WorldObj::insertSnapablePoints(QVector<float>& points){
    return;
}

QString WorldObj::getName(){
    return this->type;
}
