/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef OGLOBJ_H
#define	OGLOBJ_H

#include <QString>
#include "GLUU.h"
#include "Vector4f.h"

class OglObj {
public:
    enum VertexAttr {V = 3, VT = 5, VNT = 8};
    enum MaterialType {NONE, TEXTURE, COLOR};
    bool loaded;
    
    OglObj();
    OglObj(const OglObj& orig);
    virtual ~OglObj();
    void init(float* punkty, int ptr, enum VertexAttr v, int type);
    virtual void render();
    virtual void render(int selectionColor);
    void deleteVBO();
    void setMaterial(float r, float g, float b);
    void setMaterial(QString* path);
    int getTexId();
    void setLineWidth(int val);
private:
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
    int length; 
    int shapeType;
    int texId;
    int materialType;
    int lineWidth = 1;
    QString *res;
    Vector4f *color = NULL;
};

#endif	/* OGLOBJ_H */

