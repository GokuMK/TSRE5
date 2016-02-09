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
    Camera(float* pt);
    Camera(const Camera& orig);
    virtual ~Camera();
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
    float * pozT;
    int starex, starey;
private:
    //int aktwx = 0, aktwz = 0;
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
    bool moveF, moveR, moveB, moveL;
    PreciseTileCoordinate* currentPos = NULL;
};

#endif	/* CAMERA_H */

