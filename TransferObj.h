/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRANSFEROBJ_H
#define	TRANSFEROBJ_H

#include "WorldObj.h"
#include "OglObj.h"
#include <QString>

class TransferObj : public WorldObj {
public:
    /*struct Shape{
        int iloscv;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };*/
    //Shape shape;
    OglObj shape;
    QString texture;
    float width = 0;
    float height = 0;
    
    TransferObj();
    TransferObj(const TransferObj& orig);
    virtual ~TransferObj();
    bool allowNew();
    void load(int x, int y);
    void set(int sh, FileBuffer* val);
    void set(QString sh, QString val);
    void set(QString sh, float val);
    void set(QString sh, FileBuffer* data);
    void save(QTextStream* out);
    void deleteVBO();
    int getTexId();
    virtual Ref::RefItem* getRefInfo();
    void translate(float px, float py, float pz);
    void rotate(float x, float y, float z);
    void resize(float x, float y, float z);
    int getDefaultDetailLevel();
    void render(GLUU* gluu, float lod, float posx, float posz, float* playerW, float* target, float fov, int selectionColor);
private:
    void drawShape();
    int tex;
    bool init;
    float bound[6];
    QString *texturePath;
};

#endif	/* TRANSFEROBJ_H */

