#include "Vector3f.h"

Vector3f::Vector3f() {
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

/*float* Vector3f::toFloat() {
    return { this->x, this->y, this->z };
}

Vector3f Vector3f::add(Vector3f v1, Vector3f v2) {
    return new Vector3f(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3f Vector3f::sub(Vector3f v1, Vector3f v2) {
    return new Vector3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3f Vector3f::cross(Vector3f v1, Vector3f v2) {
    return new Vector3f(v1.y * v2.z - v1.z * v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x
            );
}

void Vector3f::div(float d) {
    this.x /= d;
    this.y /= d;
    this.z /= d;
}

Vector3f Vector3f::add(Vector3f p) {
    return new Vector3f(this.x + p.x, this.y + p.y, this.z + p.z);
}*/

