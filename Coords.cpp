/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Coords.h"
#include <QFile>
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <cstdlib>
#include "TexLib.h"
#include "Vector2f.h"
#include "TerrainLib.h"
#include "TS.h"
#include "Game.h"
#include "FileFunctions.h"
#include "ReadFile.h"
#include "GeoCoordinates.h"
#include "OglObj.h"
#include "TextObj.h"

Coords::Coords() {
    loaded = false;
}

Coords::Coords(QString path) {
    qDebug() << "undefined coords file";
    loaded = false;
}

Coords::~Coords() {
}

void Coords::render(GLUU* gluu, float * playerT, float* playerW, float playerRot) {
    if (!loaded) return;

    gluu->setMatrixUniforms();

    if (simpleMarkerObjP == NULL) {
        simpleMarkerObjP = new OglObj();
        simpleMarkerObjL = new OglObj();
        float *punkty = new float[3 * 2];
        int ptr = 0;
        int i = 0;

        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 30;
        punkty[ptr++] = 0;

        simpleMarkerObjP->setMaterial(1.0, 0.0, 1.0);
        simpleMarkerObjP->init(punkty, ptr, simpleMarkerObjP->V, GL_LINES);
        simpleMarkerObjL->setMaterial(0.0, 1.0, 0.0);
        simpleMarkerObjL->init(punkty, ptr, simpleMarkerObjL->V, GL_LINES);
        delete[] punkty;
    }
    
    TextObj* txt;
    for (int i = 0; i < markerList.size(); i++ ) {
        if(Game::markerLines){
            if(markerList[i].line3d == NULL){
                markerList[i].line3d = new OglObj();
                markerList[i].line3d->setLineWidth(2);
                qDebug() << markerList[i].style;
                QColor color(style[markerList[i].style].color);
                markerList[i].line3d->setMaterial(color.redF(),color.greenF(),color.blueF());
                //markerList[i].line3d->setMaterial(((float)((i*10)))/255.0,1,1);
            }
            if(!markerList[i].line3d->loaded){
                //qDebug() << markerList[i].x.size();
                float *punkty = new float[markerList[i].x.size()*12]; 
                int ptr = 0;

                for(int j = 0; j < markerList[i].x.size() - 1; j++){
                    if(markerList[i].segmentPtr[j+1] > 0)
                        continue;
                    float h = Game::terrainLib->getHeight(markerList[i].tileX[j], -markerList[i].tileZ[j], markerList[i].x[j], markerList[i].z[j]);
                    punkty[ptr++] = markerList[i].x[j] + 2048 * ( markerList[i].tileX[j] - markerList[i].tileX[0] );
                    punkty[ptr++] = markerList[i].y[j] + h;
                    punkty[ptr++] = markerList[i].z[j] - 2048 * ( markerList[i].tileZ[j] - markerList[i].tileZ[0] );
                    punkty[ptr++] = markerList[i].x[j+1] + 2048 * ( markerList[i].tileX[j+1] - markerList[i].tileX[0] );
                    punkty[ptr++] = markerList[i].y[j+1] + h;
                    punkty[ptr++] = markerList[i].z[j+1] - 2048 * ( markerList[i].tileZ[j+1] - markerList[i].tileZ[0] );
                }
                markerList[i].line3d->init(punkty, ptr, markerList[i].line3d->V, GL_LINES);
                delete[] punkty;
            }
            gluu->mvPushMatrix();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0 + 2048 * (markerList[i].tileX[0] - playerT[0]), 10, 0 + 2048 * (-markerList[i].tileZ[0] - playerT[1]));
            gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));        
            markerList[i].line3d->render();
            gluu->mvPopMatrix();
            /////
        } else {
            for(int j = 0; j < markerList[i].tileX.size(); j++ ){
                if (fabs(markerList[i].tileX[j] - playerT[0]) + fabs(-markerList[i].tileZ[j] - playerT[1]) > 2) {
                    continue;
                }
                gluu->mvPushMatrix();
                //if(pos == NULL) return;
                float h = Game::terrainLib->getHeight(markerList[i].tileX[j], -markerList[i].tileZ[j], markerList[i].x[j], markerList[i].z[j]);
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, markerList[i].x[j] + 2048 * (markerList[i].tileX[j] - playerT[0]), h, markerList[i].z[j] + 2048 * (-markerList[i].tileZ[j] - playerT[1]));
                //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
                //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
                gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
                if(j == 0)
                    simpleMarkerObjP->render();
                else
                    simpleMarkerObjL->render();
                Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 30, 0);
                gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
                txt = nameGl[markerList[i].name.toStdString()];
                if(txt == NULL){
                    txt = new TextObj(markerList[i].name, 16, 1.0);
                    txt->setColor(0,0,0);
                    nameGl[markerList[i].name.toStdString()] = txt;
                } 
                txt->render(playerRot);
                gluu->mvPopMatrix();
            }
        }
        
        /*if (markerList[i].tileX.size() == 1) continue;
        
        if (markerList[i].oglObj == NULL){
            markerList[i].oglObj = new OglObj();
            float *punkty = new float[markerList[i].pointsX.size()*3*2];
            int ptr = 0;
            int j = 0;
            for (j = 0; j < markerList[i].pointsX.size() - 1; j++) {
                punkty[ptr++] = markerList[i].pointsX[j];
                punkty[ptr++] = Game::terrainLib->getHeight(markerList[i].tileX, -markerList[i].tileZ, markerList[i].pointsX[j], markerList[i].pointsZ[j]);
                punkty[ptr++] = markerList[i].pointsZ[j];
                punkty[ptr++] = markerList[i].pointsX[j+1];
                punkty[ptr++] = Game::terrainLib->getHeight(markerList[i].tileX, -markerList[i].tileZ, markerList[i].pointsX[j], markerList[i].pointsZ[j]);
                punkty[ptr++] = markerList[i].pointsZ[j+1];
            }

            markerList[i].oglObj->setMaterial(1.0, 0.0, 1.0);
            markerList[i].oglObj->init(punkty, ptr, markerList[i].oglObj->V, GL_LINES);
            delete[] punkty;
        }
        
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * (markerList[i].tileX - playerT[0]), 0, 2048 * (-markerList[i].tileZ - playerT[1]));
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        markerList[i].oglObj->render();
        gluu->mvPopMatrix();*/
    }
};

void Coords::getTileList(QMap<int, QPair<int, int>*> &tileList, int radius, int step){
    if (!loaded) return;
    
    QMap<int, QPair<int, int>*> tileList2;
    for (int i = 0; i < markerList.size(); i++ ) {
        for(int j = 0; j < markerList[i].tileX.size(); j++ ){
            if(tileList2[markerList[i].tileX[j]*10000 + markerList[i].tileZ[j]] == NULL)
                tileList2[markerList[i].tileX[j]*10000 + markerList[i].tileZ[j]] = new QPair<int, int>(markerList[i].tileX[j], markerList[i].tileZ[j]);
            }
        }
    
    QMapIterator<int, QPair<int, int>*> i(tileList2);
    int x, z;
    radius *= step;
    qDebug() << "radius" << radius;
    while (i.hasNext()) {
        i.next();
        if(i.value() == NULL)
            continue;
        x = i.value()->first;
        z = i.value()->second;
        for(int i = -radius; i <= radius; i+=step)
            for(int j = -radius; j <= radius; j+=step){
                if(tileList[(x+i)*10000+(z+j)] == NULL){
                    tileList[(x+i)*10000+(z+j)] = new QPair<int, int>(x+i, z+j);
                    //qDebug() << i.value()->first << i.value()->second;
                }
            }
    }
}