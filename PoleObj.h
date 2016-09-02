/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef POLEOBJ_H
#define	POLEOBJ_H

#include "OglObj.h"

class PoleObj : public OglObj {
public:
    PoleObj(float height = 10.0);
    virtual ~PoleObj();
private:

};

#endif	/* POLEOBJ_H */

