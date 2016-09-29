/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Vector3f.h"
#include <math.h>

Vector3f::Vector3f() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3f::Vector3f(const Vector3f& orig) {
    this->x = orig.x;
    this->y = orig.y;
    this->z = orig.z;
}

Vector3f::~Vector3f() {
}

Vector3f::Vector3f(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3f::set(float x, float y, float z) {
    this->x = x; this->y = y; this->z = z;
}

/*float* Vector3f::toFloat() {
    return { this->x, this->y, this->z };
}*/
void Vector3f::setFromAdd(Vector3f v1, Vector3f v2) {
    this->x = v1.x + v2.x; this->y = v1.y + v2.y; this->z = v1.z + v2.z;
}

void Vector3f::setFromSub(Vector3f v1, Vector3f v2) {
    this->x = v1.x - v2.x; this->y = v1.y - v2.y; this->z = v1.z - v2.z;
}

void Vector3f::setFromCross(Vector3f v1, Vector3f v2) {
    float tx = v1.y * v2.z - v1.z * v2.y;
    float ty = v1.z * v2.x - v1.x * v2.z;
    float tz = v1.x * v2.y - v1.y * v2.x;
    this->x = tx;
    this->y = ty;
    this->z = tz;
}

void Vector3f::normalize() {
        float l = (float)sqrt((x * x) + (y * y) + (z * z));
        if(l==0) return;
        x/=l;
        y/=l;
        z/=l;
    }

void Vector3f::div(float d) {
    x /= d;
    y /= d;
    z /= d;
}

void Vector3f::rotateY(float a, float r) { //angle, radius
    this->x -= r;
    float nz = (float) (z * cos(-a) - x * sin(-a));
    float nx = (float) (z * sin(-a) + x * cos(-a));
    this->x = nx + r;
    this->z = nz;
    //x = nx-r;
}

void Vector3f::rotateX(float a, float r) { //angle, radius
    this->y -= r;
    float nz = (float) (z * cos(-a) - y * sin(-a));
    float ny = (float) (z * sin(-a) + y * cos(-a));
    this->y = ny + r;
    this->z = nz;
    //x = nx-r;
}

void Vector3f::rotate(Vector3f o) {
        float ny = (float) (z*cos(-o.y) - x*sin(-o.y));
        float nx = (float) (z*sin(-o.y) + x*cos(-o.y));
        x = nx;
        z = ny;
        ny = (float) (z*cos(-o.z) - x*sin(-o.z));
        nx = (float) (z*sin(-o.z) + x*cos(-o.z));
        x = nx;
        z = ny;
        ny = (float) (sin(-o.x)*sqrt(x*x+z*z));
        y = ny;
    }
/*
Vector3f Vector3f::add(Vector3f p) {
    return new Vector3f(this.x + p.x, this.y + p.y, this.z + p.z);
}*/

