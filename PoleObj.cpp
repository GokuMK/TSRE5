/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PoleObj.h"

PoleObj::PoleObj(float height) {
    float punkty[36 * 3]{
        -0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, 0.5f,
        -0.5f, height, 0.5f,
        -0.5f, 0.0f, -0.5f,
        -0.5f, height, 0.5f,
        -0.5f, height, -0.5f,
        0.5f, height, 0.5f,
        0.5f, 0.0f, -0.5f,
        0.5f, height, -0.5f,
        0.5f, 0.0f, -0.5f,
        0.5f, height, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, height, -0.5f,
        -0.5f, 0.0f, -0.5f,
        -0.5f, height, -0.5f,
        0.5f, height, -0.5f,
        0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, -0.5f,
        -0.5f, height, 0.5f,
        -0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, height, 0.5f,
        -0.5f, height, 0.5f,
        0.5f, 0.0f, 0.5f,
        0.5f, height, 0.5f,
        0.5f, height, -0.5f,
        -0.5f, height, -0.5f,
        0.5f, height, 0.5f,
        -0.5f, height, -0.5f,
        -0.5f, height, 0.5f,
        0.5f, 0.0f, 0.5f,
        -0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, -0.5f,
        0.5f, 0.0f, 0.5f,
        -0.5f, 0.0f, 0.5f,
        -0.5f, 0.0f, -0.5f
    };
    int ptr = 36*3;
    this->setMaterial(1.0, 1.0, 0.0);
    this->init(punkty, ptr, RenderItem::V, GL_TRIANGLES);
}

PoleObj::~PoleObj() {
}

