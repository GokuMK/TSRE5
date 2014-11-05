#ifndef VECTOR4F_H
#define	VECTOR4F_H

class Vector4f {
public:
    float x;
    float y;
    float z;
    float c;
    Vector4f();
    Vector4f(const Vector4f& orig);
    Vector4f(float x, float y, float z);
    Vector4f(float x, float y, float z, float c);
    ~Vector4f();
    void set(float x, float y, float z);
    void set(float x, float y, float z, float c);
private:

};

#endif	/* VECTOR4F_H */

