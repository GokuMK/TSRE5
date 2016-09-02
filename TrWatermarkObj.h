/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRWATERMARKOBJ_H
#define	TRWATERMARKOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrWatermarkObj : public WorldObj  {
public:
    TrWatermarkObj();
    TrWatermarkObj(int level);
    TrWatermarkObj(const TrWatermarkObj& orig);
    virtual ~TrWatermarkObj();
    void load(int x, int y);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    int dstLevel = 0;
};
#endif	/* TRWATERMARKOBJ_H */

