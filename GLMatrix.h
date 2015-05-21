#ifndef GLMATRIX_H
#define	GLMATRIX_H

class Vec3 {
public:
    static float* transformMat4(float* out, float* a, float* m);
    static  float* transformQuat(float* out, float* a, float* q);
};

class Vec4 {
public:
    static float* normalize(float* out, float* a);

};

class Quat {
public:
    static float* fromMat3(float *out, float *m);
    static float* fromRotationXYZ(float *out, float *a);
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
    static float* rotateY(float* out, float* a, float rad);
    static float* translate(float* out, float* a, float* v);
    static float* translate(float* out, float* a,float x,float y,float z);
private:

};

#endif	/* GLMATRIX_H */

