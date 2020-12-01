/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef RENDERER_H
#define RENDERER_H

#include <QVector>
#include <QHash>

class RenderItem;

class Renderer {
public:
    enum RenderMode {
        RENDER_DEFAULT = 0,
        RENDER_SELECTION = 1,
        RENDER_SHADOWMAP = 2
    };
    float* objStrMatrix = NULL;
    float* mvMatrix = NULL;
    float* mvMatrixStack[1000];
    QVector<float*> mvMatrixDelete;
    int imvMatrixStack = 0;
    Renderer();
    Renderer(const Renderer& orig);
    virtual ~Renderer();
    virtual void pushItem(RenderItem *r, float* mvmatrix);
    virtual void pushItemVNTA(RenderItem *r, float* mvmatrix);
    virtual void pushItemsVNTA(QVector<RenderItem*> &r, float* mvmatrix);
    void mvPushMatrix();
    void mvPopMatrix();
    QVector<RenderItem*> items; 
    QVector<float*> mvMatrixs;
    //QHash<unsigned int, QVector<RenderItem*>> itemsVNTA;
    QHash<unsigned int, QHash<unsigned long long int, RenderItem*>> itemsVNTA; 
    virtual void renderFrame();
private:
   
};

#endif /* RENDERER_H */

