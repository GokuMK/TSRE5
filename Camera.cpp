/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Camera.h"
#include <math.h>
#include "GLMatrix.h"
#include "Game.h"
#include "GeoCoordinates.h"
#include "GameObj.h"
#include "GLUU.h"

Camera::Camera(float* pt) {
    pozT = pt;
    
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
    relativeRot[2] = 0;
    relativePos[0] = 0;
    relativePos[1] = 0;
    relativePos[2] = 0;
    
    Mat4::identity(lookAt);
    
    moveF = moveR = moveB = moveL = false;
    fov = Game::cameraFov;
}

Camera::Camera(const Camera& orig) {
}

Camera::~Camera() {
}

PreciseTileCoordinate* Camera::getCurrentPos(){
    if(this->currentPos == NULL)
        this->currentPos = new PreciseTileCoordinate();
    return this->currentPos;
}

float* Camera::getMatrix() {
    return lookAt;
};

float* Camera::getTarget() {
    return target;
}

float* Camera::getUp() {
    return up;
}

void Camera::setPos(float* pos){
}

void Camera::setPos(float x, float y, float z){
}

void Camera::setPlayerRot(float x, float y){
    this->playerRot[0] = x;
    this->playerRot[1] = y;
}

void Camera::setPozT(int x, int y){
}

float* Camera::getPos() {
    return pos;
}

float Camera::getRotX() {
    return 0;
}
float Camera::getRotY() {
    return 0;
}

void Camera::update(float fps) {
}

void Camera::renderHud(GLUU *gluu){
    
    if(cameraObject != NULL)
        cameraObject->renderHud();
}

void Camera::setCameraObject(GameObj* o){
    cameraObject = o;
    playerRot[0] = M_PI;
    playerRot[1] = 0.0;
    playerPos[0] = 0;
    playerPos[1] = 3;
    playerPos[2] = 0;
}

void Camera::moveForward(float fps) {
}

void Camera::moveBackward(float fps) {
}

void Camera::moveLeft(float fps) {
}

void Camera::moveRight(float fps) {
}

void Camera::moveUp() {
}

void Camera::moveDown() {
}

void Camera::patrzX(float f) {
}

void Camera::patrzY(float f) {
}

void Camera::check_coords() {
}

void Camera::MouseWheel(QWheelEvent* e) {
}

void Camera::MouseMove(QMouseEvent* e) {
}

void Camera::MouseDown(QMouseEvent* e) {
}

void Camera::MouseUp(QMouseEvent* e) {
}

void Camera::keyUp(QKeyEvent * e) {
}

void Camera::keyDown(QKeyEvent * e) {
};
