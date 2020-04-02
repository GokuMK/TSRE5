/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef HAZARDOBJ_H
#define	HAZARDOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class HazardObj : public WorldObj {
public:
    HazardObj();
    HazardObj(const HazardObj& o);
    WorldObj* clone();
    virtual ~HazardObj();
    bool allowNew();
    bool isTrackItem();
    bool containsTrackItem(int tdbId, int id);
    void getTrackItemIds(QVector<int> &ids, int tdbId);
    void deleteTrItems();
    void initTrItems(float* tpos);
    void load(int x, int y);
    void set(QString sh, QString val);
    void set(int sh, FileBuffer* data);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    int getDefaultDetailLevel();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);

private:
    int trItemIdCount = 0;
    int *trItemId = NULL;
    TrackItemObj* pointer3d = NULL;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
    bool getSimpleBorder(float* border);
    bool getBoxPoints(QVector<float> &points);
};

#endif	/* HAZARDOBJ_H */

