/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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

