/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Vector2i.h"

Vector2i::Vector2i() {
    this->x = 0;
    this->z = 0;
}

Vector2i::Vector2i(int x, int z){
        this->x = x;
        this->z = z;
    }    

Vector2i::Vector2i(const Vector2i& orig) {
    this->x = orig.x;
    this->z = orig.z;
}

Vector2i::~Vector2i() {
}

bool Vector2i::equals(Vector2i other) {
         return (x == other.x) && (z == other.z);
     }

int Vector2i::hashCode() {
        return x*10000+z;
    }

