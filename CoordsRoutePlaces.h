/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef COORDSROUTEPLACES_H
#define	COORDSROUTEPLACES_H
#include "Coords.h"

class TDB;

class CoordsRoutePlaces : public Coords {
public:
    CoordsRoutePlaces(TDB *tdb, QString place);
    virtual ~CoordsRoutePlaces();
private:

};

#endif	/* COORDSROUTEPLACES_H */

