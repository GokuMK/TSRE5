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

GroupObj::GroupObj(const GroupObj* orig) {
    this->typeID = WorldObj::groupobject;
    this->type = "groupobject";
    this->objects.append(orig->objects);
    this->selected = true;
    this->pivot.set = orig->pivot.set;
    Vec3::copy(this->pivot.position, (float*)orig->pivot.position);
    Vec3::copy(this->pivot.qDirection, (float*)orig->pivot.qDirection);
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
    float tp[3];
    float tpos[3];
    int pid = -1;
    float oldQrot[4];
    float newQrot[4];
    if (pivot.set == -2 ) {
        Vec3::copy(tp, pivot.position);
        Quat::copy(oldQrot, pivot.qDirection);
        Quat::copy(newQrot, pivot.qDirection);
    } else {
        pid = pivot.set;
        if(pid < 0 ||pid >= objList->objects.size())
            pid = 0;
        Vec3::copy(tp, objList->objects[pid]->position);
        Quat::copy(oldQrot, objList->objects[pid]->qDirection);
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

unsigned int GroupObj::count(){
    return objects.size();
}

void GroupObj::translate(float px, float py, float pz){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->translate(px, py, pz);
    }
}

void GroupObj::rotate(float x, float y, float z){
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
        this->objects[i]->rotate(x, y, z);
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
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setNewQdirection();
    }
}

void GroupObj::setQdirection(float* q){
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->setQdirection(q);
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