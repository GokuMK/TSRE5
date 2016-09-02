/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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

