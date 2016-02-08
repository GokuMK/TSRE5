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
    int ptr = 36 * 3;
    this->setMaterial(1.0, 1.0, 0.0);
    this->init(punkty, ptr, this->V, GL_TRIANGLES);
}

PoleObj::~PoleObj() {
}

