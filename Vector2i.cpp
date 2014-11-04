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

