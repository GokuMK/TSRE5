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

class Route;

class GroupObj : public WorldObj{
public:
    struct Pivot {
        int set = -1;
        float position[3];
        float qDirection[4];
    };
    
    GroupObj();
    GroupObj(const GroupObj* orig);
    void translate(float px, float py, float pz);
    void rotate(float x, float y, float z);
    void resize(float x, float y, float z);
    void setPosition(float* p);
    void setPosition(int x, int z, float* p);
    void initPQ(float* p, float* q);
    void setQdirection(float* q);
    void setNewQdirection();
    void setMartix();
    void addObject(WorldObj* obj);
    bool select();
    bool select(int val);
    bool unselect();
    unsigned int count();
    void fromNewObjects(GroupObj *objList, Route *route, int x, int z, float *p);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
    virtual ~GroupObj();
private:
    QVector<WorldObj*> objects;
    Pivot pivot;
};

#endif	/* GROUPOBJ_H */

