/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef VECTOR2I_H
#define	VECTOR2I_H

class Vector2i {
public:
    int x;
    int z;
    Vector2i();
    Vector2i(int x, int z);
    Vector2i(const Vector2i& orig);
    virtual ~Vector2i();
    bool equals(Vector2i other);
    int hashCode();
private:
    
};

#endif	/* VECTOR2I_H */

