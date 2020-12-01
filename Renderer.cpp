/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Renderer.h"
#include "GLMatrix.h"

Renderer::Renderer() {
    mvMatrix = new float[16];
}

Renderer::Renderer(const Renderer& orig) {
}

Renderer::~Renderer() {
}

void Renderer::pushItem(RenderItem* r, float* mvmatrix){
}

void Renderer::pushItemsVNTA(QVector<RenderItem*>& r, float* mvmatrix){
    
}

void Renderer::pushItemVNTA(RenderItem* r, float* mvmatrix){
}

void Renderer::mvPushMatrix() {
    mvMatrixStack[imvMatrixStack++] = Mat4::clone(mvMatrix);
}

void Renderer::mvPopMatrix() {
    if (--imvMatrixStack < 0) return;
    //delete[] mvMatrix;
    mvMatrixDelete.push_back(mvMatrix);
    mvMatrix = mvMatrixStack[imvMatrixStack];
}

void Renderer::renderFrame(){
    
}
