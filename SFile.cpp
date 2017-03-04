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

void SFile::load() {
    
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << "S Shape: not exist "<<pathid;
        file->close();
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
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        sh = "shader_names";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        SFileX::odczytajshaders(bufor, this);
        
        sh = "points";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajpunkty(bufor, this);
        getSize();
        
        sh = "uv_points";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje "<< sh <<" na " << bufor->off;
        SFileX::odczytajuvpunkty(bufor, this);

        sh = "normals";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje "<< sh <<" na " << bufor->off;
        SFileX::odczytajnormalne(bufor, this);

        sh = "matrices";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajmatrices(bufor, this);

        sh = "images";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajimages(bufor, this);

        sh = "textures";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
        //qDebug() << "znaleziono sekcje " << sh << " na " << bufor->off;
        SFileX::odczytajtextures(bufor, this);

        sh = "vtx_states";
        if(!ParserX::FindTokenDomIgnore(sh, bufor)) return;
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

    file->close();
    loadSd();
    return;
}

void SFile::loadSd() {
    if(loadedSd == true)
        return;
    QFile file(pathid+"d");
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << "Sd Shape: not exist "<<pathid+"d";
        return;
    }
    FileBuffer* data = ReadFile::read(&file);
    file.close();
    data->toUtf16();
    data->skipBOM();
    QString sh;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("simisa@@@@@@@@@@jinx0t1t______")) {
            continue;
        }
        if (sh == ("shape")) {
            sdName = ParserX::GetString(data).trimmed();
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("esd_detail_level")) {
                    esdDetailLevel = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_alternative_texture")) {
                    esdAlternativeTexture = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_bounding_box")) {
                    esdBoundingBox << EsdBoundingBox();
                    bool ok = false;
                    for(int i = 0; i < 6; i++){
                        esdBoundingBox.back().shape[i] = ParserX::GetNumberInside(data, &ok);
                        if(!ok) break;
                    }
                    if(ok == false)
                        esdBoundingBox.pop_back();
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_complex")) {
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("esd_complex_box")) {
                            esdBoundingBox << EsdBoundingBox();
                            for(int i = 0; i < 3; i++){
                                esdBoundingBox.back().rotation[i] = ParserX::GetNumber(data);
                            }
                            for(int i = 0; i < 3; i++){
                                esdBoundingBox.back().translation[i] = ParserX::GetNumber(data);
                            }
                            for(int i = 0; i < 6; i++){
                                esdBoundingBox.back().shape[i] = ParserX::GetNumber(data);
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("esd_snapable")) {
                    snapable = true;
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
    }
    
    loadedSd = true;
    delete data;
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

bool SFile::getBoxPoints(QVector<float>& points){
    if(this->esdBoundingBox.size() == 0){
        for(int i=0; i<2; i++)
            for(int j=4; j<6; j++){
                points.push_back(-bound[i]);
                points.push_back(bound[2]);
                points.push_back(bound[j]);
                points.push_back(-bound[i]);
                points.push_back(bound[3]);
                points.push_back(bound[j]);
            }
        for(int i=0; i<2; i++)
            for(int j=2; j<4; j++){
                points.push_back(-bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[4]);
                points.push_back(-bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[5]);
            }
        for(int i=4; i<6; i++)
            for(int j=2; j<4; j++){
                points.push_back(-bound[0]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
                points.push_back(-bound[1]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
            }
    } else {
        float tbound[6];
        for(int u = 0; u < this->esdBoundingBox.size(); u++ ){
            tbound[0] = this->esdBoundingBox[u].shape[0];
            tbound[1] = this->esdBoundingBox[u].shape[3];
            tbound[2] = this->esdBoundingBox[u].shape[1];
            tbound[3] = this->esdBoundingBox[u].shape[4];
            tbound[4] = this->esdBoundingBox[u].shape[2];
            tbound[5] = this->esdBoundingBox[u].shape[5];
            
            //int pointsBeg = points.size();
            ///////////
            for(int i=0; i<2; i++)
                for(int j=4; j<6; j++){
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[2]);
                    points.push_back(tbound[j]);
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[3]);
                    points.push_back(tbound[j]);
                }
            for(int i=0; i<2; i++)
                for(int j=2; j<4; j++){
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[4]);
                    points.push_back(-tbound[i]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[5]);
                }
            for(int i=4; i<6; i++)
                for(int j=2; j<4; j++){
                    points.push_back(-tbound[0]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[i]);
                    points.push_back(-tbound[1]);
                    points.push_back(tbound[j]);
                    points.push_back(tbound[i]);
                }
            //int pointsEnd = points.size();
            //for(int i = pointsBeg; i < pointsEnd; i++){
            //    //Vec3::add(&points[i], &points[i], this->esdBoundingBox[u].translation);
            //}
        }
    }
    return true;
}

void SFile::getFloorBorderLinePoints(float *&punkty){
    if(this->esdBoundingBox.size() == 0){
        float h = bound[2];
        if(bound[3] < bound[2])
            h = bound[3];
//0 1 / 4 5
        float vec1[3], vec2[3], dist;
        Vec3::set(vec1, bound[0], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[0], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
    } else {
        float tbound[6];
        for(int u = 0; u < this->esdBoundingBox.size(); u++ ){
            tbound[0] = this->esdBoundingBox[u].shape[0];
            tbound[1] = this->esdBoundingBox[u].shape[3];
            tbound[2] = this->esdBoundingBox[u].shape[1];
            tbound[3] = this->esdBoundingBox[u].shape[4];
            tbound[4] = this->esdBoundingBox[u].shape[2];
            tbound[5] = this->esdBoundingBox[u].shape[5];
            
        float h = tbound[2];
        if(tbound[3] < tbound[2])
            h = tbound[3];
        float vec1[3], vec2[3], dist;
        Vec3::set(vec1, bound[0], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[0], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[5]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[4]);
        Vec3::set(vec2, bound[0], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        Vec3::set(vec1, bound[1], h, bound[5]);
        Vec3::set(vec2, bound[1], h, bound[4]);
        dist = Vec3::distance(vec1, vec2);
        Vec3::sub(vec2, vec2, vec1);
        Vec3::normalize(vec2, vec2);
        for(float i = 0; i < dist + 2; i+=4 ){
            *punkty++ = vec1[0] + i*vec2[0];
            *punkty++ = h;
            *punkty++ = vec1[2] + i*vec2[2];
        }
        }
    }
}

bool SFile::isSnapable(){
    return snapable;
}

void SFile::addSnapablePoints(QVector<float> &out){
    //if(!this->loadedSd)
    //    loadSd();
    if(esdBoundingBox.size() == 0) return;
    float avgZ = (esdBoundingBox[0].shape[2] + esdBoundingBox[0].shape[5])/2.0;
    out.push_back(esdBoundingBox[0].shape[0]);
    out.push_back(0);
    out.push_back(avgZ);
    out.push_back(esdBoundingBox[0].shape[3]);
    out.push_back(0);
    out.push_back(avgZ);
}

void SFile::reload() {
    loaded = 0;
}

void SFile::render() {

    if (isinit != 1 || loaded == 2)
        return;
    if (loaded == 0) {
        if(Game::allowObjLag < 1)  return;
        
        Game::allowObjLag-=2;
        loaded = 2;
        load();
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
    //float talpha = gluu->alpha;
    //float talphatest = gluu->alphaTest;
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
                    macierz[matrix].hash = gluu->getMatrixHash(macierz[matrix].fixed);
                }
                if(macierz[matrix].hash != gluu->currentMsMatrinxHash){
                    gluu->currentMsMatrinxHash = macierz[matrix].hash;
                    gluu->currentShader->setUniformValue(gluu->currentShader->msMatrixUniform, *reinterpret_cast<float(*)[4][4]>(&macierz[matrix].fixed));
                }
            }
            
            if( vtxstate[vtx_state].arg2 < -7 )
                gluu->disableNormals();
            else
                gluu->enableNormals();
            
            if( vtxstate[vtx_state].arg2 == -12 )
                gluu->setBrightness(0.5);
            else
                gluu->setBrightness(1.0);
            
            /*if(primstate[prim_state].arg2 < this->ishaders)
                gluu->alpha = shader[primstate[prim_state].arg2].alpha;
            else 
                gluu->alpha = 0;
            gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlpha, gluu->alpha);

            if(primstate[prim_state].arg6 == 1)
                gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlphaTest, 0.51f);
            else 
                gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlphaTest, gluu->alphaTest);
            */
            //if(gluu->textureEnabled)
            if(primstate[prim_state].arg4 == -1){
                //glDisable(GL_TEXTURE_2D);
            } else if (image[texture[primstate[prim_state].arg4].image].texAddr > 0) {
                //glEnable(GL_TEXTURE_2D);
                gluu->bindTexture(f, image[texture[primstate[prim_state].arg4].image].texAddr);
                //f->glBindTexture(GL_TEXTURE_2D, image[texture[primstate[prim_state].arg4].image].texAddr);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -2) {
                //glDisable(GL_TEXTURE_2D);
            } else if (image[texture[primstate[prim_state].arg4].image].tex == -1) {
                //image[texture[primstate[prim_state].arg4].image].tex = -2;
                //qDebug() << this->nazwa;
                //if(this->nazwa.contains("pared1_I.s", Qt::CaseInsensitive)){
                //    qDebug() << vtxstate[vtx_state].arg2;
                //    qDebug() << "=========" << image[texture[primstate[prim_state].arg4].image].name;
                //}
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
    //gluu->currentShader->setUniformValue(gluu->currentShader->shaderAlphaTest, gluu->alphaTest);
    gluu->setBrightness(1.0);
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


