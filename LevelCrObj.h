/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef LEVELCROBJ_H
#define	LEVELCROBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class LevelCrObj : public WorldObj {
public:
    LevelCrObj();
    LevelCrObj(const LevelCrObj& orig);
    virtual ~LevelCrObj();
    void load(int x, int y);
    void set(QString sh, QString val);
    void set(int sh, FileBuffer* data);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    bool getBorder(float* border);
    bool allowNew();
    bool isTrackItem();
    void deleteTrItems();
    void initTrItems(float* tpos);
    int getDefaultDetailLevel();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);

private:
    int levelCrParameters[2];
    int crashProbability = 0;
    unsigned int levelCrData[2];
    int levelCrTiming[3];
    int *trItemId = NULL;
    int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL;
    std::vector<float*> drawPositions;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
};

#endif	/* LEVELCROBJ_H */

