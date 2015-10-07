#include "TrackItemObj.h"

TrackItemObj::TrackItemObj(int type) : OglObj() {
    if(type == 0){
        float punkty[18*3]{
            -0.5,-0.5,0,
            -0.5,0.5,0,
            0.5,0.5,0,
            0.5,0.5,0,
            0.5,-0.5,0,
            -0.5,-0.5,0,
            -0.5,0.5,0,
            -0.5,-0.5,0,
            0,0,1,
            0.5,0.5,0,
            0,0,1,
            0.5,-0.5,0,
            0.5,-0.5,0,
            0,0,1,
            -0.5,-0.5,0,
            0.5,0.5,0,
            -0.5,0.5,0,
            0,0,1
            };
        int ptr = 18*3;
        this->setMaterial(0.0, 1.0, 0.0);
        this->init(punkty, ptr, this->V, GL_TRIANGLES);
        //delete[] punkty;
    } else {
        float punkty[36*3]{
            -0.5f,-0.5f,-0.5f, 
            -0.5f,-0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f, 
            0.5f, 0.5f,-0.5f, 
            -0.5f,-0.5f,-0.5f,
            -0.5f, 0.5f,-0.5f,
            0.5f,-0.5f, 0.5f,
            -0.5f,-0.5f,-0.5f,
            0.5f,-0.5f,-0.5f,
            0.5f, 0.5f,-0.5f,
            0.5f,-0.5f,-0.5f,
            -0.5f,-0.5f,-0.5f,
            -0.5f,-0.5f,-0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f,-0.5f,
            0.5f,-0.5f, 0.5f,
            -0.5f,-0.5f, 0.5f,
            -0.5f,-0.5f,-0.5f,
            -0.5f, 0.5f, 0.5f,
            -0.5f,-0.5f, 0.5f,
            0.5f,-0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f,-0.5f,-0.5f,
            0.5f, 0.5f,-0.5f,
            0.5f,-0.5f,-0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f,-0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            0.5f, 0.5f,-0.5f,
            -0.5f, 0.5f,-0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f,-0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f, 0.5f, 0.5f,
            -0.5f, 0.5f, 0.5f,
            0.5f,-0.5f, 0.5f
        };
        int ptr = 36*3;
        this->setMaterial(0.0, 1.0, 0.0);
        this->init(punkty, ptr, this->V, GL_TRIANGLES);
        //delete[] punkty;
    }
}

TrackItemObj::TrackItemObj(const TrackItemObj& orig) {
}

TrackItemObj::~TrackItemObj() {
}

