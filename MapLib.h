/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef MAPLIB_H
#define	MAPLIB_H

#include <QThread>
#include "Texture.h"
#include <unordered_map>


class MapLib : public QThread
 {
     Q_OBJECT

public:
    MapLib();
    Texture* texture;

private:
    
protected:
     void run();
};


#endif	/* MAPLIB_H */

