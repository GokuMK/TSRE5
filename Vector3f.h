/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    void rotateX(float a, float r);
    void rotate(Vector3f o);
    void setFromAdd(Vector3f v1, Vector3f v2);
    void add(Vector3f v);
    void setFromSub(Vector3f v1, Vector3f v2);
    void setFromCross(Vector3f v1, Vector3f v2) ;
    void div(float d);
    void normalize();
    //Vector3f add(Vector3f p);
private:

};

#endif	/* VECTOR3F_H */

