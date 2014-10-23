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
    virtual ~Vector3f();
    
private:

};

#endif	/* VECTOR3F_H */

