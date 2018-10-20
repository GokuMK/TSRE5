/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GROUPOBJ_H
#define	GROUPOBJ_H

#include "WorldObj.h"
#include <QString>
#include <QVector>

class Route;

class GroupObj : public WorldObj {
public:
    struct Pivot {
        int set = -1;
        float position[3];
        float qDirection[4];
        int x;
        int z;
    };
    QVector<WorldObj*> objects;
    
    GroupObj();
    GroupObj(const GroupObj& o);
    WorldObj* clone();
    float *getPosition();
    float *getQuatRotation();
    void translate(float px, float py, float pz);
    void rotate(float x, float y, float z);
    void resize(float x, float y, float z);
    void setPosition(float* p);
    void setPosition(int x, int z, float* p);
    void setPositionYValue(float val);
    void initPQ(float* p, float* q);
    void setQdirection(float* q);
    void setNewQdirection();
    void setMartix();
    int getDefaultDetailLevel();
    int getCustomDetailLevel();
    int getCurrentDetailLevel();
    void setCustomDetailLevel(int val);
    bool customDetailLevelEnabled();
    bool isAnimated();
    bool isTerrainObj();
    ShadowType getShadowType();
    void setAnimated(bool val);
    void setTerrainObj(bool val);
    void setShadowType(ShadowType val);
    void adjustPositionToTerrain();
    void adjustRotationToTerrain();
    void addObject(WorldObj* obj);
    bool select();
    bool select(int val);
    bool unselect();
    void setIndividualRotation(bool val);
    bool isIndividualRotation();
    unsigned int count();
    void fromNewObjects(GroupObj *objList, Route *route, int x, int z, float *p);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);
    virtual ~GroupObj();
private:
    Pivot pivot;
    bool individualRotation = false;
};

#endif	/* GROUPOBJ_H */

