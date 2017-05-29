/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PICKUPOBJ_H
#define	PICKUPOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class TrackItemObj;

class PickupObj : public WorldObj {
public:
    PickupObj();
    PickupObj(const PickupObj& o);
    WorldObj* clone();
    virtual ~PickupObj();
    bool allowNew();
    bool isTrackItem();
    void deleteTrItems();
    void initTrItems(float* tpos);
    void load(int x, int y);
    void set(int sh, FileBuffer* data);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void setTypeId(int val);
    void setCapacity(float val);
    void setFillRate(float val);
    void setPickupContent(float val);
    void setSpeedMin(float val);
    void setSpeedMax(float val);
    void setAnimTypeId(int val);
    void setAnimLength(float val);
    void setInfinite(bool val);
    void setBroken(bool val);
    int getTypeId();
    float getCapacity();
    float getFillRate();
    float getPickupContent();
    float getSpeedMin();
    float getSpeedMax();
    int getAnimTypeId();
    float getAnimLength();
    bool isInfinite();
    bool isBroken();
    int getDefaultDetailLevel();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);

private:
    float speedRange[2];
    int pickupType[2];
    int pickupAnimData1;
    float pickupAnimData2;
    float pickupCapacity1;
    float pickupCapacity2;
    int *trItemId = NULL;
    int trItemIdCount = 0;
    TrackItemObj* pointer3d = NULL;
    float* drawPosition = NULL;
    void renderTritems(GLUU* gluu, int selectionColor);
    bool getSimpleBorder(float* border);
    bool getBoxPoints(QVector<float> &points);
};

#endif	/* PICKUPOBJ_H */

