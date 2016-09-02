/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    float dot(Vector2f b,Vector2f c);
    float cross(Vector2f a,Vector2f b);

private:

};

#endif	/* VECTOR2F_H */

