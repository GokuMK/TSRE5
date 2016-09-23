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
    
    this->pivot.set = objList->pivot.set;
    Vec3::copy(this->pivot.position, (float*)objList->pivot.position);
    Vec3::copy(this->pivot.qDirection, (float*)objList->pivot.qDirection);
    float tp[3];
    float tpos[3];
    int pid = 0;
    if (pivot.set == -2 ) {
        tp[0] = pivot.position[0];
        tp[1] = pivot.position[1];
        tp[2] = pivot.position[2];
    } else {
        pid = pivot.set;
        if(pid < 0 ||pid >= objList->objects.size())
            pid = 0;
        tp[0] = objList->objects[pid]->position[0];
        tp[1] = objList->objects[pid]->position[1];
        tp[2] = objList->objects[pid]->position[2];
    }  
    
    for(int i = 0; i < objList->objects.size(); i++){
        if(objList->objects[i] != NULL){
            tpos[0] = p[0] - (tp[0] - objList->objects[i]->position[0]);
            tpos[1] = p[1] - (tp[1] - objList->objects[i]->position[1]);
            tpos[2] = p[2] - (tp[2] - objList->objects[i]->position[2]);
            q = Quat::create();
            Quat::copy(q, objList->objects[i]->qDirection);
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
    for(int i = 0; i < this->objects.size(); i++){
        this->objects[i]->rotate(x, y, z);
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
        tp[0] = pivot.position[0];
        tp[1] = pivot.position[1];
        tp[2] = pivot.position[2];
    } else {
        pid = pivot.set;
        if(pid < 0 ||pid >= objects.size())
            pid = 0;
        tp[0] = objects[pid]->position[0];
        tp[1] = objects[pid]->position[1];
        tp[2] = objects[pid]->position[2];
    }  
    
    for(int i = 0; i < objects.size(); i++){
        tpos[0] = p[0] - (tp[0] - objects[i]->position[0]);
        tpos[1] = p[1] - (tp[1] - objects[i]->position[1]);
        tpos[2] = p[2] - (tp[2] - objects[i]->position[2]);
        objects[i]->setPosition(x, z, tpos);
    }
}

void GroupObj::setPosition(float* p){
    float tpos[3];
    for(int i = 0; i < objects.size(); i++){
        tpos[0] = p[0] + (pivot.position[0] - objects[i]->position[0]);
        tpos[1] = p[1] + (pivot.position[1] - objects[i]->position[1]);
        tpos[2] = p[2] + (pivot.position[2] - objects[i]->position[2]);
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