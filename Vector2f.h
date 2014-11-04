#ifndef VECTOR2F_H
#define	VECTOR2F_H

class Vector2f {
public:
    float x;
    float y;
    Vector2f();
    Vector2f(float x, float y);
    Vector2f(float x, float y, float a, float r);
    Vector2f(const Vector2f& orig);
    void set(float x, float y);
    virtual ~Vector2f();
    float getDlugosc();
    Vector2f add(Vector2f r);
    void rotate(float a, float r);
    float* toFloat();
    static Vector2f add(Vector2f v1, Vector2f v2);
    static Vector2f sub(Vector2f v1, Vector2f v2);
    Vector2f subv(Vector2f v);
    Vector2f divf(float d);
    void div(float d);

private:

};

#endif	/* VECTOR2F_H */

