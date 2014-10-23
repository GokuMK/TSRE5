#ifndef GLMATRIX_H
#define	GLMATRIX_H

class Mat4 {
public:
    static float GLMAT_EPSILON;
    static float* clone(float* a);
    static float* create();
    static float* identity(float *out);
    static float* lookAt(float *out, float *eye, float *center, float *up);
    static float* multiply(float *out, float *a, float *b);
    static float* perspective(float* out, float fovy, float aspect, float near, float far);
    static float* rotate(float* out, float* a, float rad, float* axis);
    static float* rotate(float* out, float* a, float rad, float x, float y, float z);
    static float* translate(float* out, float* a, float* v);
    static float* translate(float* out, float* a,float x,float y,float z);
private:

};

#endif	/* GLMATRIX_H */

