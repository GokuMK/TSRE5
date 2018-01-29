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

void Intersections::vector(float *a, float *b, float *c){
	a[0] = b[0] - c[0];
	a[1] = b[1] - c[1];
	a[2] = b[2] - c[2];
    };
    
float Intersections::dot(float *b, float *c){
        return b[0]*c[0] + b[1]*c[1] + b[2]*c[2];
    };
    
void Intersections::cross(float *a, float *b, float *c){
    a[0] = b[1]*c[2] - b[2]*c[1];
    a[1] = b[2]*c[0] - b[0]*c[2]; 
    a[2] = b[0]*c[1] - b[1]*c[0];
};
    
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

int Intersections::shapeIntersectsShape(float *shape1, float *shape2, int count1, int count2, int size1, int size2, float *pos){
        float v0[3];
        float v1[3];
        float v2[3];
        float p0[3];
        float p1[3];
        float p2[3];
        
        int tak = 0;
        
        for(int i = 0; i < count1; i+=3*size1)
            for(int j = 0; j < count2; j+=3*size2){
                v0[0] = shape1[i];          v0[1] = shape1[i+1];            v0[2] = shape1[i+2];
                v1[0] = shape1[i+size1];    v1[1] = shape1[i+1+size1];      v1[2] = shape1[i+2+size1];
                v2[0] = shape1[i+2*size1];  v2[1] = shape1[i+1+2*size1];    v2[2] = shape1[i+2+2*size1];

                p0[0] = shape2[j] +pos[0];          p0[1] = shape2[j+1] +pos[1];            p0[2] = shape2[j+2] +pos[2];
                p1[0] = shape2[j+size2] +pos[0];    p1[1] = shape2[j+1+size2] +pos[1];      p1[2] = shape2[j+2+size2] +pos[2];
                p2[0] = shape2[j+2*size2] +pos[0];  p2[1] = shape2[j+1+2*size2] +pos[1];    p2[2] = shape2[j+2+2*size2] +pos[2];

                tak += segmentIntersectsTriangle(p0, p1, v0, v1, v2);
                tak += segmentIntersectsTriangle(p0, p2, v0, v1, v2);
                tak += segmentIntersectsTriangle(p1, p2, v0, v1, v2);

                tak += segmentIntersectsTriangle(v0, v1, p0, p1, p2);
                tak += segmentIntersectsTriangle(v0, v2, p0, p1, p2);
                tak += segmentIntersectsTriangle(v1, v2, p0, p1, p2);
            }
        return tak;
};

int Intersections::segmentIntersectsTriangle(float *p, float *p2, float *v0, float *v1, float *v2) {
        float a,f,u,v;
        float d[3];
        float e1[3];
        float e2[3];
        float h[3];
        float q[3];
        float s[3];
        d[0] = p2[0] - p[0];
        d[1] = p2[1] - p[1];
        d[2] = p2[2] - p[2];
	vector(e1,v1,v0);
	vector(e2,v2,v0);
	cross(h,d,e2);

        a = e1[0]*h[0] + e1[1]*h[1] + e1[2]*h[2];

        if (a > -0.00001 && a < 0.00001){
            return 0;
        }
        f = 1/a;
        vector(s,p,v0);

        u = f * (s[0]*h[0] + s[1]*h[1] + s[2]*h[2]);

        if (u < 0.0 || u > 1.0){
            return 0;
        }

        cross(q,s,e1);
        v = f * (d[0]*q[0] + d[1]*q[1] + d[2]*q[2]);
        if (v < 0.0 || u + v > 1.0){
            return 0;
        }

        float t = f * (e2[0]*q[0] + e2[1]*q[1] + e2[2]*q[2]);
        if (t > 0.00001 && t <= 1.0){
            return 1;
        }
           
        return 0;
};