/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CAMERA_H
#define	CAMERA_H

#include "Vector2i.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include <QMatrix4x4>
#include <QVector3D>
#include <QMouseEvent>

class PreciseTileCoordinate;

class Camera {
public:
    Camera(float* pt = 0);
    Camera(const Camera& orig);
    virtual ~Camera();
    virtual float* getTarget();
    virtual float* getMatrix();
    virtual float* getPos();
    virtual void setPos(float* pos);
    virtual void setPos(float x, float y, float z);
    virtual void setPlayerRot(float x, float y);
    virtual float getRotX();
    virtual float getRotY();
    virtual void setPozT(int x, int y);
    virtual void moveForward(float fps);
    virtual void moveBackward(float fps);
    virtual void moveLeft(float fps);
    virtual void moveRight(float fps);
    virtual void moveUp();
    virtual void moveDown();
    virtual void patrzX(float f);
    virtual void patrzY(float f);
    virtual void check_coords();
    virtual void MouseMove(QMouseEvent* e);
    virtual void MouseDown(QMouseEvent* e);
    virtual void MouseUp(QMouseEvent* e);
    virtual void keyDown(QKeyEvent * e);
    virtual void keyUp(QKeyEvent * e);
    virtual void update(float fps);
    virtual PreciseTileCoordinate* getCurrentPos();
    float * pozT;
    int starex, starey;
    float fov;
protected:
    float pos[3];
    float target[3];
    float up[3];
    float playerPos[3];
    float playerRot[2];
    float relativePos[3];
    float relativeRot[2];
    int przesx = 3, przesy = 1, przesz = 3;
    int jestcontrol;
    int lpm = 0;
    int rpm = 0;
    bool moveF, moveR, moveB, moveL;
    PreciseTileCoordinate* currentPos = 0;

};

#endif	/* CAMERA_H */

