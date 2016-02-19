#include "CoordsMkr.h"
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

CoordsMkr::CoordsMkr(QString path) {
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    ParserX::nextLine(data);

    QString sh = "";
    //ParserX::szukajsekcji1(sh, data);
    //ParserX::parsujr(data);

    IghCoordinate* igh;
    PreciseTileCoordinate* ppp;

    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("marker")) {
            markerList.emplace_back();
            markerList.back().lon = ParserX::parsujr(data);
            markerList.back().lat = ParserX::parsujr(data);
            markerList.back().name = ParserX::odczytajtc(data);
            markerList.back().type = ParserX::parsujr(data);

            //IghCoordinate* igh = MstsCoordinates::ConvertToIgh(x, -z, 0, 0);
            //qDebug() << igh->Line << " === " << igh->Sample;
            //LatitudeLongitudeCoordinate* latlon = MstsCoordinates::ConvertToLatLon(igh);
            //qDebug() << latlon->Latitude << " === " << latlon->Longitude;
            igh = MstsCoordinates::ConvertToIgh(markerList.back().lat, markerList.back().lon);
            //qDebug() << igh->Line << " === " << igh->Sample;
            ppp = MstsCoordinates::ConvertToTile(igh);
            //qDebug() << ppp->TileX << " === " << ppp->TileZ << " " << ppp->X << " === " << ppp->Z;
            //igh = MstsCoordinates::ConvertToIgh(ppp);
            //qDebug() << igh->Line << " === " << igh->Sample;
            //latlon = MstsCoordinates::ConvertToLatLon(igh);
            //qDebug() << latlon->Latitude << " === " << latlon->Longitude;
            markerList.back().tileX = ppp->TileX;
            markerList.back().tileZ = ppp->TileZ;
            markerList.back().x = ppp->X*2048-1024;
            markerList.back().z = ppp->Z*2048-1024;

            ParserX::pominsekcje(data);
            continue;
        }
        ParserX::pominsekcje(data);
    }

    if (markerList.size() > 0)
        loaded = true;
}

CoordsMkr::~CoordsMkr() {
}

void CoordsMkr::render(GLUU* gluu, float * playerT, float* playerW) {
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
        punkty[ptr++] = 50;
        punkty[ptr++] = 0;

        simpleMarkerObj->setMaterial(1.0, 0.0, 1.0);
        simpleMarkerObj->init(punkty, ptr, simpleMarkerObj->V, GL_LINES);
        delete[] punkty;
    }

    for (int i = 0; i < markerList.size(); i++) {
        if (fabs(markerList[i].tileX - playerT[0]) + fabs(-markerList[i].tileZ - playerT[1]) > 2) {
            continue;
        }

        gluu->mvPushMatrix();
        //if(pos == NULL) return;
        float h = TerrainLib::getHeight(markerList[i].tileX, -markerList[i].tileZ, markerList[i].x, markerList[i].z);
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, markerList[i].x + 2048 * (markerList[i].tileX - playerT[0]), h, markerList[i].z + 2048 * (-markerList[i].tileZ - playerT[1]));
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
        gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        simpleMarkerObj->render();
        gluu->mvPopMatrix();
    }
};