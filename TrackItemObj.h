/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRACKITEMOBJ_H
#define	TRACKITEMOBJ_H

#include "OglObj.h"

class TrackItemObj : public OglObj {
public:
    TrackItemObj(int type = 0);
    TrackItemObj(const TrackItemObj& orig);
    virtual ~TrackItemObj();
private:

};

#endif	/* TRACKITEMOBJ_H */

