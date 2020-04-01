/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRACKOBJ_H
#define	TRACKOBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"
#include "Ref.h"

class OglObj;

class TrackObj : public WorldObj  {
public:
    TrackObj();
    TrackObj(const TrackObj& o);
    WorldObj* clone();
    virtual ~TrackObj();
    bool allowNew();
    void setTemplate(QString val);
    void reload();
    void loadingFixes();
    void load(int x, int y);
    void loadInit();
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void set(QString sh, long long int val);
    void rotate(float x, float y, float z);
    Ref::RefItem* getRefInfo();
    void save(QTextStream* out);
    QString getShapePath();
    int getDefaultDetailLevel();
    bool isSimilar(WorldObj* obj);
    int updateTrackSectionInfo(QHash<int, int> shapes, QHash<int, int> sect);
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);
    QVector<std::array<float, 5>> jNodePosn;
    void fillJNodePosn();
    void setElevation(float prom);
    float getElevation();
    int getCollisionType();
private:
    float elevation;
    //unsigned int collideFunction;
    bool getSimpleBorder(float* border);
    bool getBoxPoints(QVector<float> &points);
    bool proceduralShapeInit = false;
    QVector<OglObj*> procShape;
    bool roadShape = false;
    bool templateDisabled = false;
};

#endif	/* TRACKOBJ_H */

