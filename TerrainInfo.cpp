/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */
#include "TerrainInfo.h"
#include "Terrain.h"

TerrainInfo::TerrainInfo() {
}

TerrainInfo::TerrainInfo(const TerrainInfo& o) {
    cx = o.cx;
    cy = o.cy;
    level = o.level;
    name = o.name;
    t = o.t;
}

TerrainInfo::~TerrainInfo() {
}

