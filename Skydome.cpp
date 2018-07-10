/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Skydome.h"
#include "GLUU.h"
#include "SFile.h"
#include "ShapeLib.h"

Skydome::Skydome() {
    QString resPath = Game::root + "/routes/" + Game::route + "/shapes";  
    int shape = Game::currentShapeLib->addShape(resPath +"/skydome.s");
    this->shapePointer = Game::currentShapeLib->shape[shape];
    if(this->shapePointer == NULL)
        return;
    loaded = true;
}

Skydome::Skydome(const Skydome& orig) {
}

Skydome::~Skydome() {
}

void Skydome::render(GLUU* gluu, int renderMode) {
    if (!loaded) return;
    if (renderMode == gluu->RENDER_SHADOWMAP) {
        return;
    }
    
    gluu->enableTextures();
    gluu->mvPushMatrix();
    if(shapePointer != NULL)
        shapePointer->render();
    gluu->mvPopMatrix();
};