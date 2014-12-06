#ifndef VECTOR3F_H
#define	VECTOR3F_H

class Vector3f {
public:
    float x;
    float y;
    float z;
    Vector3f();
    Vector3f(const Vector3f& orig);
    Vector3f(float x, float y, float z);
    ~Vector3f();
    void set(float x, float y, float z);
    void rotateY(float a, float r);
    static Vector3f add(Vector3f out, Vector3f v1, Vector3f v2);
    static Vector3f sub(Vector3f out, Vector3f v1, Vector3f v2);
    static Vector3f cross(Vector3f out, Vector3f v1, Vector3f v2) ;
    void div(float d);
    void normalize();
    //Vector3f add(Vector3f p);
private:

};

#endif	/* VECTOR3F_H */

