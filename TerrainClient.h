/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef TERRAINCLIENT_H
#define TERRAINCLIENT_H

#include "Terrain.h"

class TerrainClient : public Terrain {
    Q_OBJECT
public:
    TerrainClient();
    TerrainClient(TerrainInfo *ti);
    TerrainClient(float x, float y);
    TerrainClient(const TerrainClient& orig);
    virtual ~TerrainClient();
    void load();
    void loadTFile(FileBuffer *data);
    void loadRAWFile(FileBuffer *data);
    void loadFFile(FileBuffer *data);
    void updateTFile();
private:
    int loadingProgress = 0;
};

#endif /* TERRAINCLIENT_H */

