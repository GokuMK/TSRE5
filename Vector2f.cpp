#include "Vector2f.h"
#include <math.h>

Vector2f::Vector2f() {
    x = y = 0;
}

Vector2f::Vector2f(const Vector2f& orig) {
    this->x = orig.x;
    this->y = orig.y;
}

Vector2f::~Vector2f() {
}

Vector2f::Vector2f(float x, float y) {
    this->x = x;
    this->y = y;
}

float* Vector2f::toFloat() {
    float *f = new float[2];
    f[0] = this->x;
    f[1] = this->y;
    return f;
}

Vector2f Vector2f::add(Vector2f v1, Vector2f v2) {
    return Vector2f(v1.x + v2.x, v1.y + v2.y);
}

Vector2f Vector2f::sub(Vector2f v1, Vector2f v2) {
    return Vector2f(v1.x - v2.x, v1.y - v2.y);
}

Vector2f Vector2f::subv(Vector2f v) {
    return Vector2f(x - v.x, y - v.y);
}

Vector2f Vector2f::divf(float d) {
    return Vector2f(x / d, y / d);
}

void Vector2f::div(float d) {
    this->x /= d;
    this->y /= d;
}

void Vector2f::rotate(float a, float r) { //angle, radius
    this->x -= r;
    float ny = (float) (y * cos(-a) - x * sin(-a));
    float nx = (float) (y * sin(-a) + x * cos(-a));
    this->x = nx + r;
    this->y = ny;
    //x = nx-r;
}

Vector2f::Vector2f(float x, float y, float a, float r) { //x, y, angle, radius
    x -= r;
    float ny = (float) (y * cos(-a) - x * sin(-a));
    float nx = (float) (y * sin(-a) + x * cos(-a));
    this->x = nx + r;
    this->y = ny;
}

float Vector2f::getDlugosc() {
    return (float) sqrt(this->x * this->x + this->y * this->y);
}

Vector2f Vector2f::add(Vector2f r) {
    return Vector2f(this->x + r.x, this->y + r.y);
}
