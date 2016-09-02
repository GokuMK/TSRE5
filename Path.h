/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

/* 
 * File:   Path.h
 * Author: Goku
 *
 * Created on 14 maja 2015, 00:36
 */

#ifndef PATH_H
#define	PATH_H

#include "TDB.h"

class Path {
public:
    Path();
    Path(const Path& orig);
    virtual ~Path();
    static void CreatePaths(TDB * tdb);
private:

};

#endif	/* PATH_H */

