/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SFile.h"
#include "SFileC.h"
#include "SFileX.h"
#include "ReadFile.h"
#include "ParserX.h"
#include "TexLib.h"
#include <QDebug>
#include <QtCore>
#include <iostream>
#include <QOpenGLShaderProgram>
#include "GLUU.h"
#include "GLMatrix.h"
#include <QString>
#include "Game.h"

SFile::SFile() {
    pathid = "";
    this->loaded = 2;
}

SFile::SFile(QString pathid, QString name, QString texp ) {
    this->pathid = pathid;
    this->nazwa = name;
    this->isinit = 1;
    this->loaded = 0;
    this->texPath = texp;
}

SFile::SFile(const SFile& orig) {
}

SFile::~SFile() {
}

void SFile::Load() {
    
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << "S Shape: not exist "<<pathid;
        return;
    }
    FileBuffer* bufor = ReadFile::read(file);
    //qDebug() << "--" << path << "--" << bufor->length;
    
    bufor->off = 32;
    //qDebug() << bufor->getInt() << " - ";
    bufor->off = 32;
    if (bufor->getInt() == 71) {
        bufor->off = 32;
        int val = bufor->getInt();
        //qDebug() << val << " plik binarny ";
        //wczytanie binarnego
        bufor->off += 5;
        bufor->findToken(72);
        SFileC::odczytajshaders(bufor, this);
        
        bufor->findToken(7);
        //qDebug() << "znaleziono sekcje 7 na " << bufor->off;
        SFileC::odczytajpunktyc(bufor, this);
        getSize();
        
        bufor->findToken(9);
        //qDebug() << "znaleziono sekcje 9 na " << bufor->off;
        SFileC::odczytajuvpunktyc(bufor, this);

        bufor->findToken(5);
        //qDebug() << "znaleziono sekcje 5 na " << bufor->off;
        SFileC::odczytajnormalnec(bufor, this);

        bufor->findToken(66);
        //qDebug() << "znaleziono sekcje 66 na " << bufor->off;
        SFileC::odczytajmatricesc(bufor, this);

        bufor->findToken(14);
        //qDebug() << "znaleziono sekcje 14 na " << bufor->off;
        SFileC::odczytajimagesc(bufor, this);

        bufor->findToken(16);
        //qDebug() << "znaleziono sekcje 16 na " << bufor->off;
        SFileC::odczytajtexturesc(bufor, this);

        bufor->findToken(47);
        //qDebug() << "znaleziono sekcje 47 na " << bufor->off;
        SFileC::odczytajvtx_statesc(bufor, this);

        bufor->findToken(55);
        //qDebug() << "znaleziono sekcje 55 na " << bufor->off;
        SFileC::odczytajprim_statesc(bufor, this);

        bufor->findToken(31);
        //qDebug() << "znaleziono sekcje 31 na " << bufor->off;
        SFileC::odczytajloddc(bufor, this);
        this->loaded = 1;
    } else {
        //qDebug() << "plik xml:";
        //wczytanie plku xml
        //for (int i = 0; i < 100; i++)
        //qDebug() << ":" << (char)bufor->data[i];
        QString sh = "shape";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        sh = "shader_names";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        SFileX::odczytajshaders(bufor, this);
        
        sh = "points";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajpunkty(bufor, this);
        getSize();
        
        sh = "uv_points";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje "<< sh <<" na " << bufor->off;
        SFileX::odczytajuvpunkty(bufor, this);

        sh = "normals";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje "<< sh <<" na " << bufor->off;
        SFileX::odczytajnormalne(bufor, this);

        sh = "matrices";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajmatrices(bufor, this);

        sh = "images";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajimages(bufor, this);

        sh = "textures";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajtextures(bufor, this);

        sh = "vtx_states";
        if(!ParserX::szukajsekcji1(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajvtx_states(bufor, this);

        sh = "prim_states";
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajprim_states(bufor, this);
        
        sh = "lod_controls";
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajlodd(bufor, this);
        this->loaded = 1;
    }
    delete bufor;

    for (int i = 0; i < ishaders; i++) {
        if(shader[i].name == "texdiff") shader[i].alpha = 1;
        else shader[i].alpha = 0;
    }
    file->close();
    return;
}

void SFile::getSize() {
        float tx[2]{-9999, 9999}, ty[2]{-9999, 9999}, tz[2]{-9999, 9999};
        for (int i = 0; i < tpoints.ipoints ; i++) {
            if(tpoints.points[i].x < tx[1]) tx[1] = tpoints.points[i].x;
            if(tpoints.points[i].y < ty[1]) ty[1] = tpoints.points[i].y;
            if(tpoints.points[i].z < tz[1]) tz[1] = tpoints.points[i].z;
            if(tpoints.points[i].x > tx[0]) tx[0] = tpoints.points[i].x;
            if(tpoints.points[i].y > ty[0]) ty[0] = tpoints.points[i].y;
            if(tpoints.points[i].z > tz[0]) tz[0] = tpoints.points[i].z;
        }
        bound[0] = tx[0];
        bound[1] = tx[1];
        bound[2] = ty[0];
        bound[3] = ty[1];
        bound[4] = tz[0];
        bound[5] = tz[1];
        tx[0] = tx[0] - tx[1];
        ty[0] = ty[0] - ty[1];
        tz[0] = tz[0] - tz[1];
        size = sqrt(tx[0]*tx[0] + ty[0]*ty[0] + tz[0]*tz[0]);
        //console.log(this.size);
    }

void SFile::render() {
    
    if (isinit != 1 || loaded == 2)
        return;
    if (loaded == 0) {
        if(Game::allowObjLag < 1)  return;
        
        Game::allowObjLag-=2;
        loaded = 2;
        Load();
        return;
    }

    int oldmatrix = -2;
    float m[16];
    //var tex;
    //var oldtex = -3;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    GLUU *gluu = GLUU::get();
    //gluu->enableTextures();
    //for(int iii = 0; iii < 200; iii++)
    for (int i = 0; i < distancelevel[0].iloscs; i++) {
        QOpenGLVertexArrayObject::Binder vaoBinder(&distancelevel[0].subobiekty[i].VAO);
        
        for (int j = 0; j < distancelevel[0].subobiekty[i].iloscc; j++) {

            int prim_state = distancelevel[0].subobiekty[i].czesci[j].prim_state_idx;
            int vtx_state = primstate[prim_state].vtx_state;
            int matrix = vtxstate[vtx_state].matrix;

            if (oldmatrix != matrix) {
                oldmatrix = matrix;
                if (!macierz[matrix].isFixed) {
                    Mat4::identity(m);
                    memcpy(macierz[matrix].fixed, getPmatrix(m, matrix), sizeof (float) * 16);
                    macierz[matrix].isFixed = true;
                }
                gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]>(&macierz[matrix].fixed));
            }
            
            //qDebug() << "2";
            if(primstate[prim_state].arg2 < this->ishaders)
                gluu->alpha = shader[primstate[prim_state].arg2].alpha;
            else 
                gluu->alpha = 0;
            gluu->m_program->setUniformValue(gluu->shaderAlpha, gluu->alpha);
            
            if(primstate[prim_state].arg6 == 1)
                gluu->m_program->setUniformValue(gluu->shaderAlphaTest, 0.51f);
            else 
                gluu->m_program->setUniformValue(gluu->shaderAlphaTest, gluu->alphaTest);
            
            if(primstate[prim_state].arg4 == -1){
                //glDisable(GL_TEXTURE_2D);
            } else if (image[texture[primstate[prim_state].arg4].image].texAddr > 0) {
                //glEnable(GL_TEXTURE_2D);
                f->glBindTexture(GL_TEXTURE_2D, image[texture[primstate[prim_state].arg4].image].texAddr);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -2) {
                //glDisable(GL_TEXTURE_2D);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -1) {
                //image[texture[primstate[prim_state].arg4].image].tex = -2;
                image[texture[primstate[prim_state].arg4].image].tex = TexLib::addTex(
                        texPath,
                        image[texture[primstate[prim_state].arg4].image].name
                        );
                //glDisable(GL_TEXTURE_2D);
            } else if (TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->glLoaded) {
                image[texture[primstate[prim_state].arg4].image].texAddr = TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->tex[0];
                //glDisable(GL_TEXTURE_2D);
            } else if (TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->loaded) {
                //if(allowLag) {
                //    allowLag = false;
                TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->GLTextures();
                //glDisable(GL_TEXTURE_2D);
                //}
            } else {
                //glDisable(GL_TEXTURE_2D);
            }/**/
            
            //QOpenGLVertexArrayObject::Binder vaoBinder(&distancelevel[0].subobiekty[i].czesci[j].VAO);
            f->glDrawArrays(GL_TRIANGLES, distancelevel[0].subobiekty[i].czesci[j].offset, distancelevel[0].subobiekty[i].czesci[j].iloscv);/**/
        }
    }
    gluu->m_program->setUniformValue(gluu->shaderAlphaTest, gluu->alphaTest);
}

/*======================================================
===== Przekształcenia takie tam
=======================================================*/
float* SFile::getPmatrix(float* pmatrix, int matrix) {
    if (matrix == -1 || matrix == 0) {
        Mat4::identity(pmatrix);
        pmatrix[0] = -1;
        return pmatrix;
    } else {
        pmatrix = getPmatrix(pmatrix, distancelevel[0].hierarchia[matrix]);
    }
    Mat4::multiply(pmatrix, pmatrix, macierz[matrix].param);
    return pmatrix;
}


