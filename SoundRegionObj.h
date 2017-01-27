/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SOUNDREGION_H
#define	SOUNDREGION_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class SoundRegionObj : public WorldObj{
public:
    SoundRegionObj();
    SoundRegionObj(const SoundRegionObj& o);
    WorldObj* clone();
    virtual ~SoundRegionObj();
    bool allowNew();
    bool isTrackItem();
    bool isSoundItem();
    void initTrItems(float* tpos);
    void deleteTrItems();
    void load(int x, int y);
    void set(QString sh, long long int val);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void flip();
    int getDefaultDetailLevel();
    int getSoundregionTrackType();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);
private:
    float soundregionRoty;
    int soundregionTrackType;
    QVector<int> trItemId;
    //int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL; 
    TrackItemObj* pointer3dSelected = NULL; 
    float* drawPositionB = NULL;
    float* drawPositionE = NULL;
    OglObj* drawLine = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
    float* drawPositions = NULL;
    int selectionValue = 0;
    float angle;
};

#endif	/* SOUNDREGION_H */

