/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#include <cmath>

#include "OpenGL3Renderer.h"
#include "RenderItem.h"
#include "GLMatrix.h"
#include <QOpenGLFunctions>
#include "GLUU.h"

OpenGL3Renderer::OpenGL3Renderer() {
    mvMatrix = new float[16];
    objStrMatrix = new float[16];
    Mat4::identity(objStrMatrix);

    //VAO.create();
    //QOpenGLVertexArrayObject::Binder vaoBinder(&VAO);
    //f = QOpenGLContext::currentContext()->functions();
    /*f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glEnableVertexAttribArray(2);
    f->glEnableVertexAttribArray(3);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    f->glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));
*/
}

OpenGL3Renderer::OpenGL3Renderer(const OpenGL3Renderer& orig) {
}

OpenGL3Renderer::~OpenGL3Renderer() {
}

void OpenGL3Renderer::pushItem(RenderItem* r, float* mvmatrix){
    //r->mvMatrixId = 
    //r->mvMatrix = mvmatrix;
    //items.push_back(r);
}

void OpenGL3Renderer::pushItemsVNTA(QVector<RenderItem*>& r, float* mvmatrix){
    for(int i = 0; i < r.size(); i++){
        //r[i]->mvMatrix = Mat4::clone(mvmatrix);
        //itemsVNTA[r[i]->texAddr].push_back(r[i]);
        if(itemsVNTA[r[i]->texAddr][(unsigned long long int)r[i]] == NULL){
            itemsVNTA[r[i]->texAddr][(unsigned long long int)r[i]] = r[i];
            r[i]->mvMatrixList.clear();
        }
        r[i]->mvMatrixList.push_back(mvmatrix);
        /*RenderItem *rr = new RenderItem();
        rr->VBO = r[i]->VBO;
        rr->VAO = r[i]->VAO;
        rr->mvMatrix = Mat4::clone(mvmatrix);
        rr->vertOffset = r[i]->vertOffset;
        rr->vertCount = r[i]->vertCount;
        rr->itemType = r[i]->itemType;
        rr->vertexAttr = r[i]->vertexAttr;
        rr->msMatrix = r[i]->msMatrix;
        rr->texAddr = r[i]->texAddr;
        rr->texturesEnabled = r[i]->texturesEnabled;
        rr->shared = false;
        itemsVNTA[r[i]->texAddr].push_back(rr);*/
    }
}

void OpenGL3Renderer::pushItemVNTA(RenderItem* r, float* mvmatrix){
    //r->mvMatrix = Mat4::clone(mvmatrix);
    //if(itemsVNTA[r->texAddr][(unsigned long long int)r] == NULL){
    //    itemsVNTA[r->texAddr][(unsigned long long int)r] = r;
    //    r->mvMatrixList.clear();
    //}
    //r->mvMatrixList.push_back(mvmatrix);
    //itemsVNTA[r->texAddr].push_back(r);
}

void OpenGL3Renderer::renderFrame(){
    //return;
    for (int i = 0; i < items.size(); ++i) {
        if(items[i] == NULL)
            continue;
        if(!items[i]->shared)
            delete items[i];
    }
    items.clear();
    
    GLUU *gluu = GLUU::get();
    gluu->enableTextures();
    f = QOpenGLContext::currentContext()->functions();
    
    QHashIterator<unsigned int, QHash<unsigned long long int, RenderItem*>> it(itemsVNTA);
    //QHashIterator<unsigned int, QVector<RenderItem*>> it(itemsVNTA);
    while (it.hasNext()) {
        it.next();
        f->glBindTexture(GL_TEXTURE_2D, it.key());
        
        QHashIterator<unsigned long long int, RenderItem*> it2(itemsVNTA[it.key()]);
        //for (int i = 0; i < it.value().size(); ++i) {
        while (it2.hasNext()) {
            it2.next();
            if(it2.value() == NULL)
                continue;
            QOpenGLVertexArrayObject::Binder vaoBinder(it2.value()->VAO);
            //itemsVNTA[i]->VBO->bind();
            //f->glBindTexture(GL_TEXTURE_2D, it2.value()->texAddr);
            gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]>(it2.value()->msMatrix));
            //gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]>(it2.value()->mvMatrix));

            for(int i = 0; i < it2.value()->mvMatrixList.size(); i++){
                gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]>(it2.value()->mvMatrixList[i]));
                f->glDrawArrays(GL_TRIANGLES, it2.value()->vertOffset, it2.value()->vertCount);
            }
            //if(!itemsVNTA[it.key()]->shared)
            //    delete itemsVNTA[it.key()];
        }
        /*for (int i = 0; i < it.value().size(); ++i) {
            QOpenGLVertexArrayObject::Binder vaoBinder(it.value()[i]->VAO);
            gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]>(it.value()[i]->msMatrix));
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]>(it.value()[i]->mvMatrix));
            f->glDrawArrays(GL_TRIANGLES, it.value()[i]->vertOffset, it.value()[i]->vertCount);
            if(!itemsVNTA[it.key()][i]->shared)
                delete itemsVNTA[it.key()][i];
        }*/
        itemsVNTA[it.key()].clear();
    }
    
    itemsVNTA.clear();
    mvMatrixs.clear();
    
    for(int i=0; i<mvMatrixDelete.size(); i++){
        delete[] mvMatrixDelete[i];
    }
    mvMatrixDelete.clear();
}



