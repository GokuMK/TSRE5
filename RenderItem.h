/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef RENDERITEM_H
#define RENDERITEM_H

#include <QSharedPointer>
#include <QVector>

class Vector3f;
class Vector4f;
class QOpenGLBuffer;
class QOpenGLVertexArrayObject;

class RenderItem {
public:
    enum VertexAttr {NO_ATTR = 0, V = 3, VT = 6, VNT = 8, VNTA = 9};
    
    VertexAttr vertexAttr = NO_ATTR;
    QOpenGLBuffer *VBO = 0;    
    QOpenGLVertexArrayObject *VAO = 0;    
    bool shared = false;
    float *msMatrix = 0;
    float *mvMatrix = 0;
    QVector<float*> mvMatrixList;
    unsigned int mvMatrixId = -1;
    unsigned char normalsEnabled = 0;
    unsigned char texturesEnabled = 0;
    float brightness = 1.0;
    unsigned int vertOffset = 0;
    unsigned int vertCount = 0;
    unsigned int itemType = 0;
    unsigned int polygonMode = 0;
    
    float colorX, colorY, colorZ, colorA;
    unsigned int texAddr = 0;
    int lineWidth = 0;
    
    RenderItem();
    RenderItem(const RenderItem& orig);
    virtual ~RenderItem();
    void setVertexAttributes(VertexAttr attr);
    void disableTextures(Vector3f* color);
    void disableTextures(Vector4f* color);
    void disableTextures(int color);
    void disableTextures(float x, float y, float z, float a);
    void enableTextures(unsigned int addr);
private:

    
};

#endif /* RENDERITEM_H */

