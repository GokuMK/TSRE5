/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ProceduralShape.h"
#include "ObjFile.h"
#include "Game.h"
#include "GLMatrix.h"
#include "TrackShape.h"
#include "Route.h"
#include "TSectionDAT.h"
#include <QDateTime>

bool ProceduralShape::Loaded = false;
QMap<QString, ObjFile*> ProceduralShape::Files;
float ProceduralShape::Alpha = 0;

ObjFile* ProceduralShape::GetObjFile(QString name){
    QString path = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/"+name;
    return Files[path];
}

void ProceduralShape::Load(){
    QString path1 = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/inbk3.obj";
    QString path2 = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/uic60.obj";
    //QString path3 = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/ballast1.obj";
    QString path3 = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/ballast2.obj";
    if(Files[path1] == NULL)
        Files[path1] = new ObjFile(path1);
    if(Files[path2] == NULL)
        Files[path2] = new ObjFile(path2);
    if(Files[path3] == NULL)
        Files[path3] = new ObjFile(path3);
    
    Alpha = -0.3;
    Loaded = true;
}

void ProceduralShape::GenShape(QVector<OglObj*>& shape, TrackShape* tsh, QMap<int, float> &angles){
    if(!Loaded)
        Load();
    float matrixS[16];
    
    if(tsh == NULL)
        return;
        
    for(int j = 0; j < tsh->numpaths; j++){
        TrackShape::SectionIdx *section = &tsh->path[j];

        QVector<TSection> sections;
        for(int i = 0; i < section->n; i++){
            if(Game::currentRoute->tsection->sekcja[(int)section->sect[i]] != NULL)
            sections.push_back(*Game::currentRoute->tsection->sekcja[(int)section->sect[i]]);
        }

        float* p = new float[4000000];
        float* ptr = p;

        float q[4];    
        float qr[4];
        float posRot[6];
        float matrix1[16];
        float matrix2[16];
        ObjFile *tFile;

        QString resPath = Game::root + "/routes/" + Game::route + "/textures";
        QString* texturePath;

        ComplexLine line;
        line.init(sections);
        //qDebug() << line.length << "length";
        float pp[3];
        float zangle;
        
        Quat::fill(q);
        Quat::rotateY(q, q, -tsh->path[j].rotDeg*M_PI/180.0);
        Vec3::set(pp, -tsh->path[j].pos[0], tsh->path[j].pos[1], tsh->path[j].pos[2]);
        Mat4::fromRotationTranslation(matrixS, q, pp);
        
        
        tFile = GetObjFile("inbk3.obj");
        for(float i = 0; i < line.length; i += 0.65){
            line.getDrawPosition(posRot, i);
            Quat::fill(qr);
            Quat::rotateY(qr, qr, -tsh->path[j].rotDeg*M_PI/180.0);
            Quat::fromRotationXYZ(q, (float*)(posRot+3));
            zangle = angles[j*2]*(1.0-i/line.length) + angles[j*2+1]*(i/line.length);
            Quat::rotateZ(q, q, zangle);
            Mat4::fromRotationTranslation(matrix1, q, posRot);
            Quat::multiply(qr, qr, q);
            Mat4::multiply(matrix1, matrixS, matrix1);
            PushShapePart(ptr, tFile, 0.155, matrix1, qr);
        }

        tFile = GetObjFile("uic60.obj");
        float step = 3;
        for(float i = 0; i < line.length; i += step){
            line.getDrawPosition(posRot, i);
            Quat::fill(qr);
            Quat::rotateY(qr, qr, -tsh->path[j].rotDeg*M_PI/180.0);
            Quat::fromRotationXYZ(q, (float*)(posRot+3));
            zangle = angles[j*2]*(1.0-i/line.length) + angles[j*2+1]*(i/line.length);
            Quat::rotateZ(q, q, zangle);
            Mat4::fromRotationTranslation(matrix1, q, posRot);
            Mat4::multiply(matrix1, matrixS, matrix1);
            line.getDrawPosition(posRot, i + step);
            Quat::fromRotationXYZ(q, (float*)(posRot+3));
            zangle = angles[j*2]*(1.0-(i+step)/line.length) + angles[j*2+1]*((i+step)/line.length);
            Quat::rotateZ(q, q, zangle);
            Mat4::fromRotationTranslation(matrix2, q, posRot);
            Mat4::multiply(matrix2, matrixS, matrix2);
            Quat::multiply(qr, qr, q);
            PushShapePart(ptr, tFile, 0.325, matrix1, matrix2, qr, i, i+step);
        }

        texturePath = new QString(resPath.toLower()+"/rails1.png");
        shape.push_back(new OglObj());
        shape.back()->setMaterial(texturePath);
        shape.back()->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );

        ptr = p;

        tFile = GetObjFile("ballast2.obj");
        step = 4;
        for(float i = 0; i < line.length; i += step){
            line.getDrawPosition(posRot, i);
            Quat::fill(qr);
            Quat::rotateY(qr, qr, -tsh->path[j].rotDeg*M_PI/180.0);
            Quat::fromRotationXYZ(q, (float*)(posRot+3));
            zangle = angles[j*2]*(1.0-i/line.length) + angles[j*2+1]*(i/line.length);
            Quat::rotateZ(q, q, zangle);
            Mat4::fromRotationTranslation(matrix1, q, posRot);
            Mat4::multiply(matrix1, matrixS, matrix1);
            line.getDrawPosition(posRot, i + step);
            Quat::fromRotationXYZ(q, (float*)(posRot+3));
            zangle = angles[j*2]*(1.0-(i+step)/line.length) + angles[j*2+1]*((i+step)/line.length);
            Quat::rotateZ(q, q, zangle);
            Mat4::fromRotationTranslation(matrix2, q, posRot);
            Mat4::multiply(matrix2, matrixS, matrix2);
            Quat::multiply(qr, qr, q);
            PushShapePart(ptr, tFile, 0.05, matrix1, matrix2, qr, i, i+step);
        }   

        texturePath = new QString(resPath.toLower()+"/ballast1.png");
        //texturePath = new QString(resPath.toLower()+"/linijka.png");
        shape.push_back(new OglObj());
        shape.back()->setMaterial(texturePath);
        shape.back()->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );
        delete[] p;
    }
    return;
}

void ProceduralShape::GenShape(QVector<OglObj*> &shape, QVector<TSection> &sections) {
    if(!Loaded)
        Load();

    //unsigned long long int timeNow = QDateTime::currentMSecsSinceEpoch();
    Alpha = -0.3;
    
    float* p = new float[2000000];
    float* ptr = p;
    
    float q[4];
    float posRot[6];
    float matrix1[16];
    float matrix2[16];
    ObjFile *tFile;
    
    QString resPath = Game::root + "/routes/" + Game::route + "/textures";
    QString* texturePath;
    
    ComplexLine line;
    line.init(sections);
    //qDebug() << line.length << "length";

    tFile = GetObjFile("inbk3.obj");
    for(float i = 0; i < line.length; i += 0.65){
        line.getDrawPosition(posRot, i);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix1, q, posRot);
        PushShapePart(ptr, tFile, 0.155, matrix1, q);
    }

    tFile = GetObjFile("uic60.obj");
    float step = 3;
    for(float i = 0; i < line.length; i += step){
        line.getDrawPosition(posRot, i);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix1, q, posRot);
        line.getDrawPosition(posRot, i + step);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix2, q, posRot);
        PushShapePart(ptr, tFile, 0.325, matrix1, matrix2, q, i, i+step);
    }
    
    texturePath = new QString(resPath.toLower()+"/rails1.png");
    shape.push_back(new OglObj());
    shape.back()->setMaterial(texturePath);
    shape.back()->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );
    
    ptr = p;
    
    tFile = GetObjFile("ballast2.obj");
    step = 4;
    for(float i = 0; i < line.length; i += step){
        line.getDrawPosition(posRot, i);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix1, q, posRot);
        line.getDrawPosition(posRot, i + step);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix2, q, posRot);
        PushShapePart(ptr, tFile, 0.05, matrix1, matrix2, q, i, i+step);
    }   

    texturePath = new QString(resPath.toLower()+"/ballast1.png");
    //texturePath = new QString(resPath.toLower()+"/linijka.png");
    shape.push_back(new OglObj());
    shape.back()->setMaterial(texturePath);
    shape.back()->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );
    
    delete[] p;
    //qDebug() << "shapeTimeGen" << QDateTime::currentMSecsSinceEpoch() - timeNow;
}

void ProceduralShape::PushShapePart(float* &ptr, ObjFile* tFile, float offsetY, float* matrix, float* qrot){
    int j = 0;
    float p[3];
    for(int i = 0; i < tFile->count; i++ ){
        p[0] = tFile->points[j++];
        p[1] = tFile->points[j++];
        p[2] = tFile->points[j++];
        Vec3::transformMat4(p, p, matrix);
        *ptr++ = p[0];
        *ptr++ = p[1] + offsetY;
        *ptr++ = p[2];
        p[0] = tFile->points[j++];
        p[1] = tFile->points[j++];
        p[2] = tFile->points[j++];
        Vec3::transformQuat(p, p, qrot);
        *ptr++ = p[0];
        *ptr++ = p[1];
        *ptr++ = p[2];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = Alpha;
    }
}

void ProceduralShape::PushShapePart(float* &ptr, ObjFile* tFile, float offsetY, float* matrix1, float* matrix2, float* qrot, float dist1, float dist2){
    int j = 0;
    float p[3];
    float texY = tFile->texYmin;
    float texYstep = tFile->texYmax - tFile->texYmin;
    float itexy = 0;
    for(int i = 0; i < tFile->count; i++ ){
        p[0] = tFile->points[j++];
        p[1] = tFile->points[j++];
        p[2] = tFile->points[j++];
        if(fabs(p[2]) < 0.5){
            p[2] = 0;
            Vec3::transformMat4(p, p, matrix1);
            itexy = texYstep*dist1;
        } else {
            p[2] = 0;
            Vec3::transformMat4(p, p, matrix2);
            itexy = texYstep*dist2;
        }
        *ptr++ = p[0];
        *ptr++ = p[1] + offsetY;
        *ptr++ = p[2];
        
        p[0] = tFile->points[j++];
        p[1] = tFile->points[j++];
        p[2] = tFile->points[j++];
        Vec3::transformQuat(p, p, qrot);
        *ptr++ = p[0];
        *ptr++ = p[1];
        *ptr++ = p[2];
        *ptr++ = tFile->points[j++];
        j++;
        *ptr++ = texY + itexy;
        *ptr++ = Alpha;
    }
}

void ProceduralShape::ComplexLine::init(QVector<TSection> s){
    sections.append(s);
    length = 0;
    for(int i = 0; i < sections.size(); i++){
        length += sections[i].getDlugosc();
    }
}

float ProceduralShape::ComplexLine::getLength(){
    return length;
}

void ProceduralShape::ComplexLine::getDrawPosition(float* posRot, float distance){
    float tLength = 0;
    float sLength = 0;
    float tpos[3];
    float trot[3];
    Vec3::set(tpos, 0, 0, 0);
    Vec3::set(trot, 0, 0, 0);
    Vector3f vPos;
    
    if(distance > length)
        distance = length;
    
    for(int i = 0; i < sections.size(); i++){
        sLength = sections[i].getDlugosc();
        //qDebug() << "sLength" << sLength << distance;
        if(distance > tLength + sLength){
            sections[i].getDrawPosition(&vPos, sLength);
            vPos.rotateY(trot[1], 0);
            Vec3::add(tpos, tpos, (float*)&vPos);
            trot[1] += sections[i].getDrawAngle(sLength);
            tLength += sLength;
            continue;
        }

        sections[i].getDrawPosition(&vPos, distance - tLength);
        vPos.rotateY(trot[1], 0);
        Vec3::set(posRot, vPos.x, vPos.y, vPos.z);
        Vec3::add(posRot, posRot, tpos);
        posRot[3] = M_PI;
        posRot[4] = - trot[1] - sections[i].getDrawAngle(distance - tLength);
        posRot[5] = 0;
        return;
    }
}