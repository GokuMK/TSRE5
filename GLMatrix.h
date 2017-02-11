/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GLMATRIX_H
#define	GLMATRIX_H

class Vec2 {
public:
    static float* create();
    static float* clone(float* a);
    static float* fromValues(float x, float y);
    static float* copy(float* out, float* a);
    static float* set(float* out, float x, float y);
    static float* add(float *out, float* a, float* b);
    static float* subtract(float *out, float *a, float *b);
    static float* sub(float *out, float *a, float *b);
    static float* multiply(float *out, float *a, float *b);
    static float* mul(float *out, float *a, float *b);
    static float* divide(float *out, float *a, float *b);
    static float* div(float *out, float *a, float *b);
    static float* min(float *out, float *a, float *b);
    static float* max(float *out, float *a, float *b);
    static float* scale(float *out, float *a, float b);
    static float* scaleAndAdd(float *out, float *a, float *b, float scale);
    static float distance(float *a, float *b);
    static float dist(float *a, float *b);
    static float squaredDistance(float *a, float *b);
    static float sqrDist(float *a, float *b);
    static float length(float *a);
    static float len(float *a);
    static float squaredLength(float *a);
    static float sqrLen(float *a);
    static float* negate(float *out, float *a);
    static float* normalize(float *out, float *a);
    static float dot(float *a, float *b);
    static float* cross(float *out, float *a, float *b);
    static float* lerp(float *out, float *a, float *b, float t);
    static float* transformMat2(float *out, float *a, float *m);
    static float* transformMat2d(float *out, float *a, float *m);
    static float* transformMat3(float *out, float *a, float *m);
    static float* transformMat4(float *out, float *a, float *m);
};

class Vec3 {
public:
    static float* create();
    static float* clone(float* a);
    static float* fromValues(float x, float y, float z);
    static float* copy(float* out, float* a);
    static float* set(float* out, float x, float y, float z);
    static float* add(float* out, float* a, float b);
    static float* add(float* out, float* a, float* b);
    static float* subtract(float* out, float* a, float* b);
    static float* sub(float* out, float* a, float* b);
    static float* multiply(float* out, float* a, float* b);
    static float* mul(float* out, float* a, float* b);
    static float* divide(float* out, float* a, float* b);
    static float* div(float* out, float* a, float* b);
    static float* min(float* out, float* a, float* b);
    static float* max(float* out, float* a, float* b);
    static float* scale(float* out, float* a, float b);
    static float* scaleAndAdd(float* out, float* a, float* b, float scale);
    static float distance(float* a, float* b);
    static float dist(float* a, float* b);
    static float squaredDistance(float* a, float* b);
    static float sqrDist(float* a, float* b);
    static float length (float* a);
    static float len (float* a);
    static float squaredLength(float* a);
    static float sqrLen(float* a);
    static float* negate(float* out, float* a);
    static float* normalize(float* out, float* a);
    static float dot (float* a, float* b);
    static float* cross(float* out, float* a, float* b);
    static float* lerp (float* out, float* a, float* b, float t);
    static float* transformMat3(float* out, float* a, float* m);
    static float* transformMat4(float* out, float* a, float* m);
    static float* transformQuat(float* out, float* a, float* q);
};

class Vec4 {
public:
    static float* create();
    static float* normalize(float* out, float* a);

};

class Quat {
public:
    static float* create();
    static float* fill(float *out);
    static float* copy(float* out, float* a);
    static float* fromMat3(float *out, float *m);
    static float* fromRotationXYZ(float *out, float *a);
    static float* multiply(float *out, float *a, float *b);
    static float* invert(float* out, float* a);
    static float* setAxisAngle(float *out, float *axis, float rad);
    static float getAxisAngle(float *out_axis, float *q);
    static float* rotateX(float *out, float *a, float rad);
    static float* rotateY(float *out, float *a, float rad);
    static float* rotateZ(float *out, float *a, float rad);
};

class Mat4 {
public:
    static float GLMAT_EPSILON;
    static float* clone(float* a);
    static float* create();
    static float* identity(float *out);
    static float* fromRotationTranslation(float* out, float* q, float* v);
    static float* lookAt(float *out, float *eye, float *center, float *up);
    static float* multiply(float *out, float *a, float *b);
    static float* perspective(float* out, float fovy, float aspect, float near, float far);
    static float* rotate(float* out, float* a, float rad, float* axis);
    static float* rotate(float* out, float* a, float rad, float x, float y, float z);
    static float* rotateX(float* out, float* a, float rad);
    static float* rotateY(float* out, float* a, float rad);
    static float* translate(float* out, float* a, float* v);
    static float* translate(float* out, float* a,float x,float y,float z);
    static float* ortho(float *out, float left, float right, float bottom, float top, float near, float far);
private:

};

#endif	/* GLMATRIX_H */

