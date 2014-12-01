#include "Camera.h"
#include <math.h>
#include "GLMatrix.h"

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
    relativePos[0] = 0;
    relativePos[1] = 0;
    relativePos[2] = 0;
    
    moveF = moveR = moveB = moveL = false;
}

Camera::Camera(const Camera& orig) {
}

Camera::~Camera() {
}

/*
Vector2i Camera::getTilePos() {
    return Vector2i(aktwx, aktwz);
}

void Camera::setTilePos(Vector2i s) {
    aktwx = s.x;
    aktwz = s.z;
}

void Camera::setPlayerPos(float x, float y, float z) {
    playerPos[0] = x;
    playerPos[1] = y;
    playerPos[2] = z;

}

void Camera::setPlayerRot(float x, float y) {
    playerRot[0] = x;
    playerRot[1] = y;
}*/

float* Camera::getMatrix() {
    float* lookAt = Mat4::create();
    Mat4::lookAt(lookAt, playerPos, getTarget(), up);
    return lookAt;
};

/*
Vector3f Camera::getEye() {
    //return new Vector3f(obx + trasa.ruch1.pozW.x, oby + trasa.ruch1.pozW.y , obz - trasa.ruch1.pozW.z);
    //return relativePos.add(playerPos);
    return getPos();
}
 */
float* Camera::getTarget() {
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

/*
Vector3f Camera::getTarget0() {
    return Vector3f((float) sin(playerRot.x + relativeRot.x) * 35, (float) sin(playerRot.y + relativeRot.y) * 20, (float) cos(playerRot.x + relativeRot.x) * 35);
}

void Camera::setRelativePos(Vector3f p) {
    relativePos = p;
}

void Camera::setRelativeRot(Vector2f p) {
    relativeRot = p;
}

void Camera::scalPosRot() {
    //playerPos = playerPos.add(relativePos);
    playerPos = getPos();
    playerRot = playerRot.add(relativeRot);
    relativePos = Vector3f();
    relativeRot = Vector2f();
}
*/

void Camera::setPozT(int x, int y){
    pozT[0] = x;
    pozT[1] = y;
}

float* Camera::getPos() {
    pos[0] = relativePos[0] + (playerPos[0] * (float) cos(-relativeRot[0]) - playerPos[2] * (float) sin(-relativeRot[0]));
    pos[1] = relativePos[1] + playerPos[1];
    pos[2] = relativePos[2] + (playerPos[0] * (float) sin(-relativeRot[0]) + playerPos[2] * (float) cos(-relativeRot[0]));
    return pos;
}

float Camera::getRotX() {
    return playerRot[0] + relativeRot[0];
}
float Camera::getRotY() {
    return playerRot[1] + relativeRot[1];
}
/*
Vector3f Camera::getUp() {
    return Vector3f(0, 1, 0);
}*/

void Camera::update(float fps) {
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

void Camera::moveForward(float fps) {
    playerPos[2] = playerPos[2] + (30.0 / fps) * przesz * cos(playerRot[0]) * cos(playerRot[1]);
    playerPos[0] = playerPos[0] + (30.0 / fps) * przesz * sin(playerRot[0]) * cos(playerRot[1]);
    playerPos[1] = playerPos[1] + (30.0 / fps) * przesz * sin(playerRot[1]);

    //playerPos[2] = playerPos[2] + przesz * (float) cos(playerRot[0]);
    // playerPos[0] = playerPos[0] + przesz * (float) sin(playerRot[0]);
    check_coords();
}

void Camera::moveBackward(float fps) {
    playerPos[2] = playerPos[2] - (30.0 / fps) * przesz * cos(playerRot[0]) * cos(playerRot[1]);
    playerPos[0] = playerPos[0] - (30.0 / fps) * przesz * sin(playerRot[0]) * cos(playerRot[1]);
    playerPos[1] = playerPos[1] - (30.0 / fps) * przesz * sin(playerRot[1]);
    //playerPos[2] = playerPos[2] - przesz * (float) cos(playerRot[0]);
    //playerPos[0] = playerPos[0] - przesz * (float) sin(playerRot[0]);
    check_coords();
}

void Camera::moveLeft(float fps) {
    playerPos[0] = playerPos[0] + (30.0 / fps) * przesx * (float) cos(playerRot[0]);
    playerPos[2] = playerPos[2] - (30.0 / fps) * przesx * (float) sin(playerRot[0]);
    check_coords();
}

void Camera::moveRight(float fps) {
    playerPos[0] = playerPos[0] - (30.0 / fps) * przesx * (float) cos(playerRot[0]);
    playerPos[2] = playerPos[2] + (30.0 / fps) * przesx * (float) sin(playerRot[0]);
    check_coords();
}

void Camera::moveUp() {
    playerPos[1] += przesy;
}

void Camera::moveDown() {
    playerPos[1] -= przesy;
}

void Camera::patrzX(float f) {
    playerRot[0] = (float) (playerRot[0] + f);
}

void Camera::patrzY(float f) {
    playerRot[1] = (float) (playerRot[1] + f);
    if (playerRot[1] > 1.57) {
        playerRot[1] = (float) 1.57;
    }
    if (playerRot[1] < -1.57) {
        playerRot[1] = (float) - 1.57;
    }
}

void Camera::check_coords() {
    if (playerPos[0] > 1024) {
        playerPos[0] = -1024;
        pozT[0]++;
    }
    if (playerPos[0]<-1024) {
        playerPos[0] = 1024;
        pozT[0]--;
    }
    if (playerPos[2] > 1024) {
        playerPos[2] = -1024;
        pozT[1]++;
    }
    if (playerPos[2]<-1024) {
        playerPos[2] = 1024;
        pozT[1]--;
    }
}

void Camera::MouseMove(QMouseEvent* e) {
    if (lpm != 0) {
        patrzX((float) (float) (starex - e->x()) / 30);
        patrzY((float) (float) (starey - e->y()) / 30);
    }
    starex = e->x();
    starey = e->y();
}

void Camera::MouseDown(QMouseEvent* e) {
    lpm = 1;
    starex = e->x();
    starey = e->y();
}

void Camera::MouseUp(QMouseEvent* e) {
    lpm = 0;
    starex = e->x();
    starey = e->y();
}

void Camera::keyUp(QKeyEvent * e) {
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
        case Qt::Key_A: // A 
        case Qt::Key_Left:
            moveL = false;
            break;
        case Qt::Key_W: // W
        case Qt::Key_Up:
            moveF = false;
            break;
        case Qt::Key_D: // D
        case Qt::Key_Right:
            moveR = false;
            break;
        case Qt::Key_S: // D
        case Qt::Key_Down:
            moveB = false;
            break;
        default:
            break;
    }
}

void Camera::keyDown(QKeyEvent * e) {
    int code = e->key();
    switch (code) {
            //case 17:
            //    jestcontrol = 1;
            //     break;
        case Qt::Key_Q: // Q
        case Qt::Key_Space:
            jestcontrol = 1;
            break;
        case Qt::Key_A: // A 
        case Qt::Key_Left:
            moveL = true;
            break;
        case Qt::Key_W: // W
        case Qt::Key_Up:
            moveF = true;
            break;
        case Qt::Key_D: // D
        case Qt::Key_Right:
            moveR = true;
            break;
        case Qt::Key_S: // D
        case Qt::Key_Down:
            moveB = true;
            break;
        case Qt::Key_E: // E
        case Qt::Key_Shift:
            przesx = przesz = 40;
            break;
        default:
            break;
    }
};
