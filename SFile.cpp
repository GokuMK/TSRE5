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

void SFile::Load(QString path) {
    
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* bufor = ReadFile::read(file);
    //qDebug() << "--" << path << "--" << bufor->length;

    bufor->off = 16;
    //qDebug() << bufor->getInt() << " - ";
    bufor->off = 16;
    if (bufor->getInt() == 71) {
        bufor->off = 16;
        int val = bufor->getInt();
        //qDebug() << val << " plik binarny ";
        //wczytanie binarnego
        bufor->off += 5;
        SFileC::szukajsekcjic(7, bufor);
        //qDebug() << "znaleziono sekcje 7 na " << bufor->off;
        SFileC::odczytajpunktyc(bufor, this);
        getSize();
        
        SFileC::szukajsekcjic(9, bufor);
        //qDebug() << "znaleziono sekcje 9 na " << bufor->off;
        SFileC::odczytajuvpunktyc(bufor, this);

        SFileC::szukajsekcjic(5, bufor);
        //qDebug() << "znaleziono sekcje 5 na " << bufor->off;
        SFileC::odczytajnormalnec(bufor, this);

        SFileC::szukajsekcjic(66, bufor);
        //qDebug() << "znaleziono sekcje 66 na " << bufor->off;
        SFileC::odczytajmatricesc(bufor, this);

        SFileC::szukajsekcjic(14, bufor);
        //qDebug() << "znaleziono sekcje 14 na " << bufor->off;
        SFileC::odczytajimagesc(bufor, this);

        SFileC::szukajsekcjic(16, bufor);
        //qDebug() << "znaleziono sekcje 16 na " << bufor->off;
        SFileC::odczytajtexturesc(bufor, this);

        SFileC::szukajsekcjic(47, bufor);
        //qDebug() << "znaleziono sekcje 47 na " << bufor->off;
        SFileC::odczytajvtx_statesc(bufor, this);

        SFileC::szukajsekcjic(55, bufor);
        //qDebug() << "znaleziono sekcje 55 na " << bufor->off;
        SFileC::odczytajprim_statesc(bufor, this);

        SFileC::szukajsekcjic(31, bufor);
        //qDebug() << "znaleziono sekcje 31 na " << bufor->off;
        SFileC::odczytajloddc(bufor, this);
        this->loaded = 1;
    } else {
        //qDebug() << "plik xml:";
        //wczytanie plku xml
        //for (int i = 0; i < 100; i++)
        //qDebug() << ":" << (char)bufor->data[i];
        QString sh = "shape";
        ParserX::szukajsekcji1(sh, bufor);
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;

        sh = "points";
        ParserX::szukajsekcji1(sh, bufor);
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajpunkty(bufor, this);
        getSize();
        
        sh = "uv_points";
        ParserX::szukajsekcji1(sh, bufor);
        //qDebug() << "znaleziono sekcje "<< sh <<" na " << bufor->off;
        SFileX::odczytajuvpunkty(bufor, this);

        sh = "normals";
        ParserX::szukajsekcji1(sh, bufor);
        //qDebug() << "znaleziono sekcje "<< sh <<" na " << bufor->off;
        SFileX::odczytajnormalne(bufor, this);

        sh = "matrices";
        ParserX::szukajsekcji1(sh, bufor);
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajmatrices(bufor, this);

        sh = "images";
        ParserX::szukajsekcji1(sh, bufor);
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajimages(bufor, this);

        sh = "textures";
        ParserX::szukajsekcji1(sh, bufor);
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajtextures(bufor, this);

        sh = "vtx_states";
        ParserX::szukajsekcji1(sh, bufor);
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
        //if(GameSettings::allowObjLag < 1)  return;
        //GameSettings::allowObjLag--;
        loaded = 2;
        Load(pathid);
        return;
    }

    int oldmatrix = -2;
    float m[16];
    //var tex;
    //var oldtex = -3;

    GLUU *gluu = GLUU::get();

    //for(int iii = 0; iii < 200; iii++)
    for (int i = 0; i < distancelevel[0].iloscs; i++) {
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
            if(primstate[prim_state].arg4 == -1)
                glDisable(GL_TEXTURE_2D);
            else if (image[texture[primstate[prim_state].arg4].image].texAddr > 0) {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, image[texture[primstate[prim_state].arg4].image].texAddr);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -2) {
                glDisable(GL_TEXTURE_2D);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -1) {
                //image[texture[primstate[prim_state].arg4].image].tex = -2;
                image[texture[primstate[prim_state].arg4].image].tex = TexLib::addTex(
                        texPath,
                        image[texture[primstate[prim_state].arg4].image].name
                        );
                glDisable(GL_TEXTURE_2D);
            } else if (TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->glLoaded) {
                image[texture[primstate[prim_state].arg4].image].texAddr = TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->tex[0];
                glDisable(GL_TEXTURE_2D);
            } else if (TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->loaded) {
                //if(allowLag) {
                //    allowLag = false;
                TexLib::mtex[image[texture[primstate[prim_state].arg4].image].tex]->GLTextures();
                glDisable(GL_TEXTURE_2D);
                //}
            } else {
                glDisable(GL_TEXTURE_2D);
            }/**/

            QOpenGLVertexArrayObject::Binder vaoBinder(&distancelevel[0].subobiekty[i].czesci[j].VAO);
            glDrawArrays(GL_TRIANGLES, 0, distancelevel[0].subobiekty[i].czesci[j].iloscv);/**/
        }
    }
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


