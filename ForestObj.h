/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef FORESTOBJ_H
#define	FORESTOBJ_H

#include "WorldObj.h"
#include <QString>

class OglObj;
class Ref::RefItem;

class ForestObj : public WorldObj{
public:
    /*struct Shape{
        int iloscv;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };*/
    struct ForestList{
        QString name;
        QString texture;
        float scaleRangeX = 0;
        float scaleRangeZ = 0;
        float treeSizeX = 0;
        float treeSizeZ = 0;
    };
    static float ForestClearDistance;
    static std::vector<ForestList> forestList;
    QString treeTexture = "";
    float scaleRangeX = 0;
    float scaleRangeZ = 0;
    float areaX = 0;
    float areaZ = 0;
    float treeSizeX = 0;
    float treeSizeZ = 0;
    float population = 0;
    ForestObj();
    ForestObj(const ForestObj& o);
    WorldObj* clone();
    bool allowNew();
    void load(int x, int y);
    void set(int sh, FileBuffer* data);
    void set(QString sh, long long int val);
    void set(QString sh, float val);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    Ref::RefItem* getRefInfo();
    void save(QTextStream* out);
    void deleteVBO();
    void translate(float px, float py, float pz);
    void rotate(float x, float y, float z);
    void resize(float x, float y, float z);
    int getDefaultDetailLevel();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
    static void LoadForestList();
    static int GetListIdByTexture(QString texture);
    virtual ~ForestObj();
private:
    void drawShape();
    bool getBoxPoints(QVector<float>& points);
    int tex;
    bool init;
    OglObj shape;
    QString * texturePath = NULL;
};

#endif	/* FORESTOBJ_H */

