/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "CameraRot.h"
#include <math.h>
#include "GLMatrix.h"
#include "Game.h"
#include "IghCoords.h"

CameraRot::CameraRot(float* pt) {
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
    relativePos[0] = 0;
    relativePos[1] = 0;
    relativePos[2] = 0;
    
    moveF = moveR = moveB = moveL = false;
    przesx = 1; 
    przesy = 1; 
    przesz = 1;
    fov = 61;
}

CameraRot::CameraRot(const CameraRot& orig) {
}

CameraRot::~CameraRot() {
}

PreciseTileCoordinate* CameraRot::getCurrentPos(){
    if(this->currentPos == NULL)
        this->currentPos = new PreciseTileCoordinate();
    return this->currentPos;
}

float* CameraRot::getMatrix() {
    float* lookAt = Mat4::create();
    Mat4::lookAt(lookAt, playerPos, getTarget(), up);
    return lookAt;
};

float* CameraRot::getTarget() {
    target[0] = playerPos[0] + (float) sin(playerRot[0] + relativeRot[0]) * cos(playerRot[1] + relativeRot[1]); 
    target[1] = playerPos[1] + (float) sin(playerRot[1] + relativeRot[1]) * 1;
    target[2] = playerPos[2] + (float) cos(playerRot[0] + relativeRot[0]) * cos(playerRot[1] + relativeRot[1]);
    return target;
}

void CameraRot::setPos(float* pos){
    playerPos[0] = pos[0];
    playerPos[1] = pos[1];
    playerPos[2] = pos[2];
}

void CameraRot::setPos(float x, float y, float z){
    playerPos[0] = x;
    playerPos[1] = y;
    playerPos[2] = z;
}

void CameraRot::setPozT(int x, int y){
    pozT[0] = x;
    pozT[1] = y;
}

void CameraRot::setPlayerRot(float x, float y){
    playerRot[0] = x;
    playerRot[1] = y;
}

float* CameraRot::getPos() {
    pos[0] = relativePos[0] + (playerPos[0] * (float) cos(-relativeRot[0]) - playerPos[2] * (float) sin(-relativeRot[0]));
    pos[1] = relativePos[1] + playerPos[1];
    pos[2] = relativePos[2] + (playerPos[0] * (float) sin(-relativeRot[0]) + playerPos[2] * (float) cos(-relativeRot[0]));
    return pos;
}

float CameraRot::getRotX() {
    return playerRot[0] + relativeRot[0];
}
float CameraRot::getRotY() {
    return playerRot[1] + relativeRot[1];
}

void CameraRot::update(float fps) {
        if(moveF){
            if (jestcontrol == 1) {
                moveUp();
            } else {
                moveForward(fps);
            }
        }
        if(moveB){
            if (jestcontrol == 1) {
                moveDown();
            } else {
                moveBackward(fps);
            }
        }
        if(moveR)
            moveRight(fps);
        if(moveL)
            moveLeft(fps);
        
        //this.patrzX((this.moveX)/(this.sensitivity));
        //this.patrzY((this.moveY)/(this.sensitivity));
        //this.moveX = 0;
        //this.moveY = 0;

    }

void CameraRot::moveForward(float fps) {
    playerPos[2] = playerPos[2] + (30.0 / fps) * przesz * cos(playerRot[0]) * cos(playerRot[1]);
    playerPos[0] = playerPos[0] + (30.0 / fps) * przesz * sin(playerRot[0]) * cos(playerRot[1]);
    playerPos[1] = playerPos[1] + (30.0 / fps) * przesz * sin(playerRot[1]);

    //playerPos[2] = playerPos[2] + przesz * (float) cos(playerRot[0]);
    // playerPos[0] = playerPos[0] + przesz * (float) sin(playerRot[0]);
    check_coords();
}

void CameraRot::moveBackward(float fps) {
    playerPos[2] = playerPos[2] - (30.0 / fps) * przesz * cos(playerRot[0]) * cos(playerRot[1]);
    playerPos[0] = playerPos[0] - (30.0 / fps) * przesz * sin(playerRot[0]) * cos(playerRot[1]);
    playerPos[1] = playerPos[1] - (30.0 / fps) * przesz * sin(playerRot[1]);
    //playerPos[2] = playerPos[2] - przesz * (float) cos(playerRot[0]);
    //playerPos[0] = playerPos[0] - przesz * (float) sin(playerRot[0]);
    check_coords();
}

void CameraRot::moveLeft(float fps) {
    playerPos[0] = playerPos[0] + (30.0 / fps) * przesx * (float) cos(playerRot[0]);
    playerPos[2] = playerPos[2] - (30.0 / fps) * przesx * (float) sin(playerRot[0]);
    check_coords();
}

void CameraRot::moveRight(float fps) {
    playerPos[0] = playerPos[0] - (30.0 / fps) * przesx * (float) cos(playerRot[0]);
    playerPos[2] = playerPos[2] + (30.0 / fps) * przesx * (float) sin(playerRot[0]);
    check_coords();
}

void CameraRot::moveUp() {
    playerPos[1] += przesy;
}

void CameraRot::moveDown() {
    playerPos[1] -= przesy;
}

void CameraRot::patrzX(float f) {
    playerRot[0] = (float) (playerRot[0] + f);
}

void CameraRot::patrzY(float f) {
    playerRot[1] = (float) (playerRot[1] + f);
    if (playerRot[1] > 1.57) {
        playerRot[1] = (float) 1.57;
    }
    if (playerRot[1] < -1.57) {
        playerRot[1] = (float) - 1.57;
    }
}

void CameraRot::check_coords() {
    if (playerPos[0] > 1024) {
        playerPos[0] -= 2048;
        pozT[0]++;
    }
    if (playerPos[0]<-1024) {
        playerPos[0] += 2048;
        pozT[0]--;
    }
    if (playerPos[2] > 1024) {
        playerPos[2] -= 2048;
        pozT[1]++;
    }
    if (playerPos[2]<-1024) {
        playerPos[2] += 2048;
        pozT[1]--;
    }
}

void CameraRot::MouseMove(QMouseEvent* e) {
    if (lpm != 0) {
        patrzX((float) (float) (starex - e->x()) / 30*(fov/45.0));
        patrzY((float) (float) (starey - e->y()) / 30*(fov/45.0));
    }
    starex = e->x();
    starey = e->y();
}

void CameraRot::MouseDown(QMouseEvent* e) {
    lpm = 1;
    starex = e->x();
    starey = e->y();
}

void CameraRot::MouseUp(QMouseEvent* e) {
    lpm = 0;
    starex = e->x();
    starey = e->y();
}

void CameraRot::keyUp(QKeyEvent * e) {
    int code = e->key();
    switch (code) {
        case Qt::Key_Q: // Q
        case Qt::Key_Space:
            jestcontrol = 0;
            break;
        case Qt::Key_E: // E
        case Qt::Key_Shift:
            przesx = przesz = 3;
            break;
        case Qt::Key_Left:    
            if(!Game::usenNumPad) break;
        case Qt::Key_A: // A 
            moveL = false;
            break;
        case Qt::Key_Up:    
            if(!Game::usenNumPad) break;
        case Qt::Key_W: // W
            moveF = false;
            break;
        case Qt::Key_Right:   
            if(!Game::usenNumPad) break;
        case Qt::Key_D: // D
            moveR = false;
            break;
        case Qt::Key_Down:    
            if(!Game::usenNumPad) break;
        case Qt::Key_S: // D
            moveB = false;
            break;
        default:
            break;
    }
}

void CameraRot::keyDown(QKeyEvent * e) {
    int code = e->key();
    switch (code) {
            //case 17:
            //    jestcontrol = 1;
            //     break;
        //case Qt::Key_Q: // Q
        case Qt::Key_Space:
            jestcontrol = 1;
            break;
        case Qt::Key_Left:    
            if(!Game::usenNumPad) break;
        case Qt::Key_A: // A 
            moveL = true;
            break;
        case Qt::Key_Up:    
            if(!Game::usenNumPad) break;
        case Qt::Key_W: // W
            moveF = true;
            break;
        case Qt::Key_Right:   
            if(!Game::usenNumPad) break;
        case Qt::Key_D: // D
            moveR = true;
            break;
        case Qt::Key_Down:    
            if(!Game::usenNumPad) break;
        case Qt::Key_S: // D
            moveB = true;
            break;
        //case Qt::Key_E: // E
        case Qt::Key_Shift:
            przesx = przesz = 40;
            break;
        default:
            break;
    }
};
