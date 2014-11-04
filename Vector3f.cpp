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

Vector3f Vector3f::add(Vector3f out, Vector3f v1, Vector3f v2) {
    out.x = v1.x + v2.x; out.y = v1.y + v2.y; out.z = v1.z + v2.z;
    return out;
}

Vector3f Vector3f::sub(Vector3f out, Vector3f v1, Vector3f v2) {
    out.x = v1.x - v2.x; out.y = v1.y - v2.y; out.z = v1.z - v2.z;
    return out;
}

Vector3f Vector3f::cross(Vector3f out, Vector3f v1, Vector3f v2) {
    out.x = v1.y * v2.z - v1.z * v2.y;
    out.y = v1.z * v2.x - v1.x * v2.z;
    out.z = v1.x * v2.y - v1.y * v2.x;
    return out;
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
/*
Vector3f Vector3f::add(Vector3f p) {
    return new Vector3f(this.x + p.x, this.y + p.y, this.z + p.z);
}*/

