/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "CameraConsist.h"
#include <math.h>
#include "GLMatrix.h"
#include "Game.h"
#include "IghCoords.h"

CameraConsist::CameraConsist(float* pt) {
    if(pt != NULL)
        pozT = pt;
    else{
        pozT = new float[2];
        pozT[0] = 0; 
        pozT[1] = 1;
    }
        
    
    up[0] = 0;
    up[1] = 1;
    up[2] = 0;
    playerRot[0] = 0.0;
    playerRot[1] = 0.0;
    playerPos[0] = 0;
    playerPos[1] = 10;
    playerPos[2] = 0;
    
    relativeRot[0] = 0;
    relativeRot[1] = 0;
    relativePos[0] = 0;
    relativePos[1] = 0;
    relativePos[2] = 0;
    
    moveF = moveR = moveB = moveL = false;
    fov = 49;
}

CameraConsist::CameraConsist(const CameraConsist& orig) {
}

CameraConsist::~CameraConsist() {
}

PreciseTileCoordinate* CameraConsist::getCurrentPos(){
    if(this->currentPos == NULL)
        this->currentPos = new PreciseTileCoordinate();
    this->currentPos->TileX = this->pozT[0];
    this->currentPos->TileZ = -this->pozT[1];
    float* p = this->getPos();
    this->currentPos->setWxyz(p[0], p[1], -p[2]);
    return this->currentPos;
}

float* CameraConsist::getMatrix() {
    float* lookAt = Mat4::create();
    Mat4::lookAt(lookAt, playerPos, getTarget(), up);
    return lookAt;
};

float* CameraConsist::getTarget() {
    // + trasa.ruch1.pozW.x
    //return new Vector3f(obx+ trasa.ruch1.pozW.x + (float) Math.sin(kato) * 35 , oby + trasa.ruch1.pozW.y + (float) Math.sin(katov) * 20 , obz - trasa.ruch1.pozW.z + (float) Math.cos(kato) * 35 );
    //return new Vector3f(playerPos.x + relativePos.x + (float) Math.sin(playerRot.x + relativeRot.x) * 1 , playerPos.y + relativePos.y + (float) Math.sin(playerRot.y  + relativeRot.y) * 1 , playerPos.z + relativePos.z + (float) Math.cos(playerRot.x + relativeRot.x) * 1 );
    //return new float[3]{getPos().x + (float) sin(playerRot[0] + relativeRot[0]) * 1, playerPos.y + relativePos.y + (float) sin(playerRot.y + relativeRot.y) * 1, getPos().z + (float) cos(playerRot.x + relativeRot.x) * 1};
    target[0] = playerPos[0] + (float) sin(playerRot[0] + relativeRot[0]) * cos(playerRot[1] + relativeRot[1]); 
    target[1] = playerPos[1] + (float) sin(playerRot[1] + relativeRot[1]) * 1;
    target[2] = playerPos[2] + (float) cos(playerRot[0] + relativeRot[0]) * cos(playerRot[1] + relativeRot[1]);
    return target;
    //return new float[3]{playerPos[0], playerPos[1],playerPos[2]+1};
}

void CameraConsist::setPos(float* pos){
    playerPos[0] = pos[0];
    playerPos[1] = pos[1];
    playerPos[2] = pos[2];
}

void CameraConsist::setPos(float x, float y, float z){
    playerPos[0] = x;
    playerPos[1] = y;
    playerPos[2] = z;
}

void CameraConsist::setPozT(int x, int y){
    pozT[0] = x;
    pozT[1] = y;
}

float* CameraConsist::getPos() {
    pos[0] = relativePos[0] + (playerPos[0] * (float) cos(-relativeRot[0]) - playerPos[2] * (float) sin(-relativeRot[0]));
    pos[1] = relativePos[1] + playerPos[1];
    pos[2] = relativePos[2] + (playerPos[0] * (float) sin(-relativeRot[0]) + playerPos[2] * (float) cos(-relativeRot[0]));
    return pos;
}

float CameraConsist::getRotX() {
    return playerRot[0] + relativeRot[0];
}
float CameraConsist::getRotY() {
    return playerRot[1] + relativeRot[1];
}

void CameraConsist::update(float fps) {

}

void CameraConsist::moveForward(float fps) {

}

void CameraConsist::moveBackward(float fps) {

}

void CameraConsist::moveLeft(float fps) {

}

void CameraConsist::moveRight(float fps) {

}

void CameraConsist::moveUp() {

}

void CameraConsist::moveDown() {

}

void CameraConsist::patrzX(float f) {

}

void CameraConsist::patrzY(float f) {

}

void CameraConsist::check_coords() {

}

void CameraConsist::MouseMove(QMouseEvent* e) {

}

void CameraConsist::MouseDown(QMouseEvent* e) {

}

void CameraConsist::MouseUp(QMouseEvent* e) {

}

void CameraConsist::keyUp(QKeyEvent * e) {

}

void CameraConsist::keyDown(QKeyEvent * e) {

};
