/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Vector4f.h"

Vector4f::Vector4f() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->c = 0;
}

Vector4f::Vector4f(const Vector4f& orig) {
}

Vector4f::~Vector4f() {
}

Vector4f::Vector4f(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->c = 1;
}

Vector4f::Vector4f(float x, float y, float z, float c) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->c = c;
}

void Vector4f::set(float x, float y, float z) {
    this->x = x; this->y = y; this->z = z; this->c = 1;
}

void Vector4f::set(float x, float y, float z, float c) {
    this->x = x; this->y = y; this->z = z; this->c = c;
}