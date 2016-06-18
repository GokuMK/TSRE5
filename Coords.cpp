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
#include "IghCoords.h"
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
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();

    if (simpleMarkerObj == NULL) {
        simpleMarkerObj = new OglObj();
        float *punkty = new float[3 * 2];
        int ptr = 0;
        int i = 0;

        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 30;
        punkty[ptr++] = 0;

        simpleMarkerObj->setMaterial(1.0, 0.0, 1.0);
        simpleMarkerObj->init(punkty, ptr, simpleMarkerObj->V, GL_LINES);
        delete[] punkty;
    }
    
    TextObj* txt;
    for (int i = 0; i < markerList.size(); i++ ) {
        for(int j = 0; j < markerList[i].tileX.size(); j++ ){
            if (fabs(markerList[i].tileX[j] - playerT[0]) + fabs(-markerList[i].tileZ[j] - playerT[1]) > 2) {
                continue;
            }
            gluu->mvPushMatrix();
            //if(pos == NULL) return;
            float h = TerrainLib::getHeight(markerList[i].tileX[j], -markerList[i].tileZ[j], markerList[i].x[j], markerList[i].z[j]);
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, markerList[i].x[j] + 2048 * (markerList[i].tileX[j] - playerT[0]), h, markerList[i].z[j] + 2048 * (-markerList[i].tileZ[j] - playerT[1]));
            //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
            //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
            gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            simpleMarkerObj->render();
            Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 0, 30, 0);
            gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
            txt = nameGl[markerList[i].name.toStdString()];
            if(txt == NULL){
                txt = new TextObj(markerList[i].name, 16, 1.0);
                txt->setColor(0,0,0);
                nameGl[markerList[i].name.toStdString()] = txt;
            } 
            txt->render(playerRot);
            gluu->mvPopMatrix();
        }
        
        /*if (markerList[i].tileX.size() == 1) continue;
        
        if (markerList[i].oglObj == NULL){
            markerList[i].oglObj = new OglObj();
            float *punkty = new float[markerList[i].pointsX.size()*3*2];
            int ptr = 0;
            int j = 0;
            for (j = 0; j < markerList[i].pointsX.size() - 1; j++) {
                punkty[ptr++] = markerList[i].pointsX[j];
                punkty[ptr++] = TerrainLib::getHeight(markerList[i].tileX, -markerList[i].tileZ, markerList[i].pointsX[j], markerList[i].pointsZ[j]);
                punkty[ptr++] = markerList[i].pointsZ[j];
                punkty[ptr++] = markerList[i].pointsX[j+1];
                punkty[ptr++] = TerrainLib::getHeight(markerList[i].tileX, -markerList[i].tileZ, markerList[i].pointsX[j], markerList[i].pointsZ[j]);
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