/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef STATICOBJ_H
#define	STATICOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class StaticObj : public WorldObj  {
public:
    StaticObj();
    StaticObj(const StaticObj& o);
    WorldObj* clone();
    virtual ~StaticObj();
    bool allowNew();
    void load(int x, int y);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    QString getShapePath();
    int getDefaultDetailLevel();
    int getCollisionType();
    void setCollisionType(int val);
    void removeCollisions();
    void snapped(int side);
    void insertSnapablePoints(QVector<float>& points);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    void loadSnapablePoints();
    bool getSimpleBorder(float* border);
    bool getBoxPoints(QVector<float> &points);
    void renderSnapableEndpoints(GLUU* gluu);
    QVector<float> snapablePoints;
};

#endif	/* STATICOBJ_H */

