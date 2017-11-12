/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <QString>

#ifndef TERRAININFO_H
#define	TERRAININFO_H

class Terrain;

class TerrainInfo {
public:
    int cx = 0;
    int cy = 0;
    int level = 0;
    bool rendered = false;
    bool low = false;
    QString name = "";
    Terrain* t = NULL;
    TerrainInfo();
    TerrainInfo(const TerrainInfo& o);
    virtual ~TerrainInfo();

private:

};

#endif	/* TERRAININFO_H */

