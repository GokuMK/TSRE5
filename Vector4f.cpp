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