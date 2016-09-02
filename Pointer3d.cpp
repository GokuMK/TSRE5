/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Pointer3d.h"

Pointer3d::Pointer3d() : OglObj() {
    float *punkty = new float[3 * 6];
    int ptr = 0;
    int i = 0;

        punkty[ptr++] = -2;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 2;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;

        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = -2;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 2;

        punkty[ptr++] = 0;
        punkty[ptr++] = -2;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 2;
        punkty[ptr++] = 0;
    this->setMaterial(1.0, 1.0, 0.0);
    this->init(punkty, ptr, this->V, GL_LINES);
    delete[] punkty;
}

Pointer3d::Pointer3d(const Pointer3d& orig) {
}

Pointer3d::~Pointer3d() {
}
