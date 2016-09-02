/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CAMERACONSIST_H
#define	CAMERACONSIST_H

#include "Camera.h"

class CameraConsist : public Camera {
public:
    CameraConsist(float* pt = 0);
    CameraConsist(const CameraConsist& orig);
    virtual ~CameraConsist();
    float* getTarget();
    /*Vector2i getTilePos();
    void setTilePos(Vector2i s);
    void setPlayerPos(float x, float y, float z);
    void setPlayerRot(float x, float y);
    Vector3f getEye();
    Vector3f getTarget0();*/
    float* getMatrix();
    /*void setRelativePos(Vector3f p);
    void setRelativeRot(Vector2f p);
    void scalPosRot();*/
    float* getPos();
    void setPos(float* pos);
    void setPos(float x, float y, float z);
    float getRotX();
    float getRotY();
    void setPozT(int x, int y);
    //Vector3f getUp();
    void moveForward(float fps);
    void moveBackward(float fps);
    void moveLeft(float fps);
    void moveRight(float fps);
    void moveUp();
    void moveDown();
    void patrzX(float f);
    void patrzY(float f);
    void check_coords();
    void MouseMove(QMouseEvent* e);
    void MouseDown(QMouseEvent* e);
    void MouseUp(QMouseEvent* e);
    void keyDown(QKeyEvent * e);
    void keyUp(QKeyEvent * e);
    void update(float fps);
    PreciseTileCoordinate* getCurrentPos();

private:

};
#endif	/* CAMERACONSIST_H */

