/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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

float Intersections::pointSegmentSquaredDistanceXZ(float* p1, float* p2, float* p){
    float v1, v2;
    float w1, w2;
    float o1, o2;

    v1 = p2[0] - p1[0];
    v2 = p2[2] - p1[2];
    w1 = p[0] - p1[0];
    w2 = p[2] - p1[2];

    float c1 = w1 * v1 + w2 * v2;
    
    if ( c1 <= 0 ){
        return w1*w1 + w2*w2;
    }

    float c2 = v1 * v1 + v2 * v2;
    if ( c2 <= c1 ){
        o1 = p[0] - p2[0];
        o2 = p[2] - p2[2];
        return o1*o1 + o2*o2;
    }

    c1 = c1 / c2;
    
    o1 = p[0] - (p1[0] + v1*c1);
    o2 = p[2] - (p1[2] + v2*c1);
    return o1*o1 + o2*o2;
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines 
// intersect the intersection point may be stored in the floats i_x and i_y.
bool Intersections::segmentIntersection(float &p0_x, float &p0_y, float &p1_x, float &p1_y, 
    float &p2_x, float &p2_y, float &p3_x, float &p3_y, float &i_x, float &i_y)
{
    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        //if (i_x != 0)
            i_x = p0_x + (t * s1_x);
        //if (i_y != 0)
            i_y = p0_y + (t * s1_y);
        return true;
    }

    return false; // No collision
}