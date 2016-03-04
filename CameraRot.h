#ifndef CAMERAROT_H
#define	CAMERAROT_H

#include "Camera.h"

class CameraRot : public Camera {
public:
    CameraRot(float* pt = 0);
    CameraRot(const CameraRot& orig);
    virtual ~CameraRot();
    float* getTarget();
    float* getMatrix();
    float* getPos();
    void setPos(float* pos);
    void setPos(float x, float y, float z);
    float getRotX();
    float getRotY();
    void setPozT(int x, int y);
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

#endif	/* CAMERAROT_H */

