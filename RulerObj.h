/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef RULEROBJ_H
#define	RULEROBJ_H

#include "WorldObj.h"
#include <QString>
#include "FileBuffer.h"

class OglObj;

class RulerObj : public WorldObj {
public:
    static bool TwoPointRuler;
    RulerObj();
    RulerObj(const RulerObj& orig);
    virtual ~RulerObj();
    bool allowNew();
    void load(int x, int y);
    void set(QString sh, QString val);
    void set(QString sh, FileBuffer* data);
    void setPosition(int x, int z, float* p);
    bool select(int value);
    void save(QTextStream* out);
    float getLength();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor, int renderMode);

private:
    struct Point {
        bool selected = false;
        float position[3];
    };
    QVector<Point> points;
    OglObj* point3d = NULL;
    OglObj* line3d = NULL;
    OglObj* point3dSelected = NULL;
    int selectionValue = 0;
    float length = 0;
    
    void refreshLength();
};

#endif	/* RULEROBJ_H */

