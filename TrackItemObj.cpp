#include "TrackItemObj.h"

TrackItemObj::TrackItemObj() : OglObj() {
    float *punkty = new float[18*3];
    int ptr = 0;
    int i = 0;

        punkty[ptr++] = -0.5;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0;
        
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0;
        
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 1;
        
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 1;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0;
        
        punkty[ptr++] = 0.5;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 1;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0;
        
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = -0.5;
        punkty[ptr++] = 0.5;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 1;
        
    this->setMaterial(0.0, 1.0, 0.0);
    this->init(punkty, ptr, this->V, GL_TRIANGLES);
    delete[] punkty;
}

TrackItemObj::TrackItemObj(const TrackItemObj& orig) {
}

TrackItemObj::~TrackItemObj() {
}

