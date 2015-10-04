#include "Intersections.h"
#include "GLMatrix.h"

float Intersections::pointSegmentDistance(float* p1, float* p2, float* p, float* intersectionPoint){
    float v[3];
    float w[3];
    float Pb[3];
    Vec3::sub(v, p2, p1);
    Vec3::sub(w, p, p1);

    float c1 = Vec3::dot(w,v);
    if ( c1 <= 0 ){
        if(intersectionPoint != 0){
            intersectionPoint[0] = p1[0];
            intersectionPoint[1] = p1[1];
            intersectionPoint[2] = p1[2];
        }
        return Vec3::distance(p, p1);
    }

    float c2 = Vec3::dot(v,v);
    if ( c2 <= c1 ){
        if(intersectionPoint != 0){
            intersectionPoint[0] = p2[0];
            intersectionPoint[1] = p2[1];
            intersectionPoint[2] = p2[2];
        }
        return Vec3::distance(p, p2);
    }

    float b = c1 / c2;
    
    Vec3::add(Pb, p1, Vec3::scale(v, v, b));
    if(intersectionPoint != 0){
        intersectionPoint[0] = Pb[0];
        intersectionPoint[1] = Pb[1];
        intersectionPoint[2] = Pb[2];
    }
    return Vec3::distance(p, Pb);
}