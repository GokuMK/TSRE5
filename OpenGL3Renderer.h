/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef OPENGL3RENDERER_H
#define OPENGL3RENDERER_H

#include "Renderer.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

class QOpenGLFunctions;

class OpenGL3Renderer : public Renderer {
public:
    OpenGL3Renderer();
    OpenGL3Renderer(const OpenGL3Renderer& orig);
    virtual ~OpenGL3Renderer();
    void renderFrame();
    void pushItem(RenderItem *r, float* mvmatrix);
    void pushItemsVNTA(QVector<RenderItem*>& r, float* mvmatrix);
    void pushItemVNTA(RenderItem *r, float* mvmatrix);
private:
    QOpenGLVertexArrayObject VAO;
    QOpenGLFunctions *f;
};

#endif /* OPENGL3RENDERER_H */

