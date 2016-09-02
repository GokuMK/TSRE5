/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef POINTER3D_H
#define	POINTER3D_H

#include "OglObj.h"

class Pointer3d : public  OglObj {
public:
    Pointer3d();
    Pointer3d(const Pointer3d& orig);
    virtual ~Pointer3d();
private:

};

#endif	/* POINTER3D_H */

