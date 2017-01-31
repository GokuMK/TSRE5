/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GroupObj.h"
#include "GLMatrix.h"
#include "Route.h"

GroupObj::GroupObj() {
    this->typeID = WorldObj::groupobject;
    this->type = "groupobject";
}

GroupObj::GroupObj(const GroupObj& o) : WorldObj(o) {
    this->typeID = WorldObj::groupobject;
    this->type = "groupobject";
    this->objects.append(o.objects);
    this->selected = true;
    this->pivot.set = o.pivot.set;
    this->pivot.x = o.pivot.x;
    this->pivot.z = o.pivot.z;
    Vec3::copy(this->pivot.position, (float*)o.pivot.position);
    Vec3::copy(this->pivot.qDirection, (float*)o.pivot.qDirection);
}

WorldObj* GroupObj::clone(){
    return new GroupObj(*this);
}

GroupObj::~GroupObj() {
}

void GroupObj::fromNewObjects(GroupObj* objList, Route* route, int x, int z, float *p){
    this->unselect();
    objects.clear();
    float *q;
    WorldObj * wobj;
    
    pivot.set = objList->pivot.set;
    Vec3::copy(pivot.position, (float*)objList->pivot.position);
    Vec3::copy(pivot.qDirection, (float*)objList->pivot.qDirection);
    pivot.x = objList->pivot.x;
    pivot.z = objList->pivot.z;
    float tp[3];
    int tpx;
    int tpz;
    float tpos[3];
    int pid = -1;
    float oldQrot[4];
    float newQrot[4];
    if (pivot.set == -2 ) {
        Vec3::copy(tp, pivot.position);
        Quat::copy(oldQrot, pivot.qDirection);
        Quat::copy(newQrot, pivot.qDirection);
        tpx = pivot.x;
        tpz = pivot.z;
    } else {
        pid = pivot.set;
        if(pid < 0 ||pid >= objList->objects.size())
            pid = 0;
        Vec3::copy(tp, objList->objects[pid]->position);
        Quat::copy(oldQrot, objList->objects[pid]->qDirection);
        tpx = objList->objects[pid]->x;
        tpz = objList->objects[pid]->y;
        if(objList->objects[pid] != NULL){
            //Vec3::sub(tpos, tp, objList->objects[pid]->position);
            //Vec3::sub(tpos, p, tpos);
            q = Quat::create();
            Quat::copy(q, objList->objects[pid]->qDirection);
            wobj = route->placeObject(x, z, p, q, objList->objects[pid]->getRefInfo());
            if(wobj != NULL)
                this->addObject(wobj);
            Vec3::add(tp, tp, Vec3::sub(tpos, p, wobj->position));
            tp[0] += (x-wobj->x)*2048;
            tp[2] += (z-wobj->y)*2048;
            Quat::copy(newQrot, wobj->qDirection);
        }
    }
    float tQrot[4];
    Quat::invert(newQrot, newQrot);
    Quat::multiply(tQrot, oldQrot, newQrot);
    Quat::invert(tQrot, tQrot);
    
    for(int i = 0; i < objList->objects.size(); i++){
        if(i == pid) continue;
        if(objList->objects[i] != NULL){
            Vec3::sub(tpos, tp, objList->objects[i]->position);
            Vec3::transformQuat(tpos, tpos, tQrot);
            Vec3::sub(tpos, p, tpos);
            tpos[0] -= (tpx - objList->objects[i]->x)*2048;
            tpos[2] -= (tpz - objList->objects[i]->y)*2048;
            q = Quat::create();
            Quat::copy(q, objList->objects[i]->qDirection);
            Quat::multiply(q, q, tQrot);
            wobj = route->placeObject(x, z, tpos, q, objList->objects[i]->getRefInfo());
            if(wobj != NULL)
                this->addObject(wobj);
        }
    }
    this->select();
}

void GroupObj::addObject(WorldObj* obj){
    if(obj == NULL) 
        return;
    if(!selected){
        objects.clear();
        selected = true;
        pivot.set = -1;
    }
    for(int i = 0; i < this->objects.size(); i++){
        if(this->objects[i] == obj){
            this->objects[i]->unselect();
            this->objects.remove(i);
            return;
        }
    }
    if(pivot.set == -1){
        Vec3::copy((float*)pivot.position,(float*)obj->position);
        Vec3::copy((float*)pivot.qDirection,(float*)obj->qDirection);
        pivot.set = this->objects.size()-1;
        pivot.x = obj->x;
        pivot.z = obj->y;
    }
    this->objects.push_back(obj);
    obj->select();
}

bool GroupObj::select(){
    this->selected = true;
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->select();
    }
    return true;
}

bool GroupObj::select(int value){
    return select();
}

bool GroupObj::unselect(){
    this->selected = false;
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->unselect();
    }
}

void GroupObj::setIndividualRotation(bool val){
    this->individualRotation = val;
}

bool GroupObj::isIndividualRotation(){
    return this->individualRotation;
}

unsigned int GroupObj::count(){
    return objects.size();
}

float *GroupObj::getPosition(){
    if(pivot.set < 0)
        return (float*)pivot.position;
    else 
        return (float*)objects[pivot.set]->position;
}

float *GroupObj::getQuatRotation(){
    if(pivot.set < 0){
        return (float*)pivot.qDirection;
    }else {
        return (float*)objects[pivot.set]->qDirection;
    }
}

void GroupObj::translate(float px, float py, float pz){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->translate(px, py, pz);
    }
}

void GroupObj::rotate(float x, float y, float z){
    if(individualRotation){
        for(int i = 0; i < this->objects.size(); i++){
            this->objects[i]->rotate(x, y, z);
        }
    } else {
        float tp[3];
        float tpos[3];
        int pid = 0;
        if (pivot.set == -2 ) {
            Vec3::copy(tp, pivot.position);
        } else {
            pid = pivot.set;
            if(pid < 0 ||pid >= objects.size())
                pid = 0;
            Vec3::copy(tp, objects[pid]->position);
        }  
        float* q = Quat::create();
        for(int i = 0; i < this->objects.size(); i++){
            this->objects[i]->rotate(-x, y, z);
            Vec3::sub(tpos, tp, objects[i]->position);
            Quat::fill(q);
            Quat::rotateY(q, q, y);
            Quat::rotateX(q, q, x);
            Quat::rotateZ(q, q, z);
            Vec3::transformQuat(tpos, tpos, q);
            this->objects[i]->setPosition(tp);
            this->objects[i]->translate(-tpos[0], -tpos[1], -tpos[2]);
        }
    }
}

void GroupObj::resize(float x, float y, float z){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->resize(x, y, z);
    }
}

void GroupObj::setPosition(int x, int z, float* p){
    float tpos[3];
    float tp[3];
    int pid = 0;
    if (pivot.set == -2 ) {
        Vec3::copy(tp, pivot.position);
    } else {
        pid = pivot.set;
        if(pid < 0 ||pid >= objects.size())
            pid = 0;
        Vec3::copy(tp, objects[pid]->position);
    }  
    
    for(int i = 0; i < objects.size(); i++){
        Vec3::sub(tpos, tp, objects[i]->position);
        Vec3::sub(tpos, p, tpos);
        objects[i]->setPosition(x, z, tpos);
    }
}

void GroupObj::setPosition(float* p){
    float tpos[3];
    float tp[3];
    int pid = 0;
    if (pivot.set == -2 ) {
        Vec3::copy(tp, pivot.position);
    } else {
        pid = pivot.set;
        if(pid < 0 ||pid >= objects.size())
            pid = 0;
        Vec3::copy(tp, objects[pid]->position);
    }  
    for(int i = 0; i < objects.size(); i++){
        Vec3::sub(tpos, tp, objects[i]->position);
        Vec3::sub(tpos, p, tpos);
        objects[i]->setPosition(tpos);
    }
}

void GroupObj::setNewQdirection(){
    float q[4];
    Quat::fill(q);
    setQdirection(q);
    
    //for(int i = 0; i < this->objects.size(); i++){
    //    this->objects[i]->setNewQdirection();
    //}
}

void GroupObj::setQdirection(float* q){
    if(individualRotation){
        for(int i = 0; i < this->objects.size(); i++){
            this->objects[i]->setQdirection(q);
        }
    } else {
        float tp[3];
        float *qqq = Quat::create();
        float tpos1[3];
        float tpos2[3];
        int pid = 0;
        if (pivot.set == -2 ) {
            Vec3::copy(tp, pivot.position);
            Quat::copy(qqq, pivot.qDirection);
            Vec3::copy(pivot.qDirection, q);
        } else {
            pid = pivot.set;
            if(pid < 0 ||pid >= objects.size())
                pid = 0;
            Vec3::copy(tp, objects[pid]->position);
            Quat::copy(qqq, objects[pid]->qDirection);
            this->objects[pid]->setQdirection(q);
        }  
        float* qq = Quat::create();
        Quat::fill(qq);
        Quat::multiply(qq, qq, qqq);
        for(int i = 0; i < this->objects.size(); i++){
            if(i == pid) continue;
            tpos2[0] = 0;
            tpos2[1] = 0;
            tpos2[2] = Vec3::distance(tp, objects[i]->position);
            Vec3::transformQuat(tpos2, tpos2, q);
            //qDebug() << "tpos2" << tpos2[0]<< tpos2[2];
            this->objects[i]->setPosition(tp);
            this->objects[i]->translate(-tpos2[0], -tpos2[1], -tpos2[2]);
            this->objects[i]->setQdirection(q);
        }
    }
}

void GroupObj::initPQ(float* p, float* q){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->initPQ(p, q);
    }
}

void GroupObj::setMartix(){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setMartix();
    }
}

void GroupObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    
}

void GroupObj::adjustPositionToTerrain(){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->adjustPositionToTerrain();
    }
}

void GroupObj::adjustRotationToTerrain(){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->adjustRotationToTerrain();
    }
}

int GroupObj::getDefaultDetailLevel(){
    if(this->objects.size() > 0)
        return this->objects[0]->getDefaultDetailLevel();
    return 0;
}

int GroupObj::getCustomDetailLevel(){
    if(this->objects.size() > 0)
        return this->objects[0]->getCustomDetailLevel();
    return 0;
}

int GroupObj::getCurrentDetailLevel(){
    if(this->objects.size() > 0)
        return this->objects[0]->getCurrentDetailLevel();
    return 0;
}

void GroupObj::setCustomDetailLevel(int val){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setCustomDetailLevel(val);
    }
}

bool GroupObj::customDetailLevelEnabled(){
    if(this->objects.size() > 0)
        return this->objects[0]->customDetailLevelEnabled();
    return false;
}

bool GroupObj::isAnimated(){
    if(this->objects.size() > 0)
        return this->objects[0]->isAnimated();
    return false;
}

bool GroupObj::isTerrainObj(){
    if(this->objects.size() > 0)
        return this->objects[0]->isTerrainObj();
    return false;
}

WorldObj::ShadowType GroupObj::getShadowType(){
    if(this->objects.size() > 0)
        return this->objects[0]->getShadowType();
    return WorldObj::ShadowNone;
}

void GroupObj::setAnimated(bool val){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setAnimated(val);
    }
}

void GroupObj::setTerrainObj(bool val){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setTerrainObj(val);
    }
}

void GroupObj::setShadowType(WorldObj::ShadowType val){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setShadowType(val);
    }
}
