/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef COORDSKML_H
#define	COORDSKML_H

#include "Coords.h"

class CoordsKml : public Coords {
public:
    CoordsKml(QString path);
    ~CoordsKml();
private:
};

#endif	/* COORDSKML_H */

