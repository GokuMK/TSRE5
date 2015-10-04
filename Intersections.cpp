#include "Intersections.h"
#include "GLMatrix.h"

float Intersections::pointSegmentDistance(float* p1, float* p2, float* p){
    float v[3];
    float w[3];
    float Pb[3];
    Vec3::sub(v, p2, p1);
    Vec3::sub(w, p, p1);

    float c1 = Vec3::dot(w,v);
    if ( c1 <= 0 )
        return Vec3::distance(p, p1);

    float c2 = Vec3::dot(v,v);
    if ( c2 <= c1 )
        return Vec3::distance(p, p2);

    float b = c1 / c2;
    
    Vec3::add(Pb, p1, Vec3::scale(v, v, b));
    return Vec3::distance(p, Pb);
}