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
#include "Intersections.h"

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

void ProceduralShape::GenTrackShape(QVector<OglObj*>& shape, TrackShape* tsh, QMap<int,float>& angles){
    
    float matrixS[16];
    float matrixS1[16];
    float matrixS2[16];
    QVector<QVector<ShapePrimitive>> primitives;
    
    if(tsh->numpaths == 2){
    //for(int j = 0; j < tsh->numpaths; j++){
        TrackShape::SectionIdx *section = &tsh->path[0];

        QVector<TSection> sections1;
        QVector<TSection> sections2;
        for(int i = 0; i < section->n; i++){
            if(Game::currentRoute->tsection->sekcja[(int)section->sect[i]] != NULL)
            sections1.push_back(*Game::currentRoute->tsection->sekcja[(int)section->sect[i]]);
        }
        section = &tsh->path[1];
        for(int i = 0; i < section->n; i++){
            if(Game::currentRoute->tsection->sekcja[(int)section->sect[i]] != NULL)
            sections2.push_back(*Game::currentRoute->tsection->sekcja[(int)section->sect[i]]);
        }
        //float* p = new float[4000000];
        //float* ptr = p;

        float q[4];   
        float q1[4];   
        float q2[4];   
        float qr[4];
        float posRot[6];
        float posRot1[6];
        float posRot2[6];
        float matrix1[16];
        float matrix2[16];
        ObjFile *tFile;


        ComplexLine line1;
        ComplexLine line2;
        line1.init(sections1);
        line2.init(sections2);
        //qDebug() << line.length << "length";
        float pp[3];
        float zangle;
        
        Quat::fill(q1);
        Quat::rotateY(q1, q1, -tsh->path[0].rotDeg*M_PI/180.0);
        Vec3::set(pp, -tsh->path[0].pos[0], tsh->path[0].pos[1], tsh->path[0].pos[2]);
        Mat4::fromRotationTranslation(matrixS1, q1, pp);
        Quat::fill(q2);
        Quat::rotateY(q2, q2, -tsh->path[1].rotDeg*M_PI/180.0);
        Vec3::set(pp, -tsh->path[1].pos[0], tsh->path[1].pos[1], tsh->path[1].pos[2]);
        Mat4::fromRotationTranslation(matrixS2, q2, pp);
        //Quat::multiply(q, q1, q2);
        
        //Quat
        bool junct = false;
        if(tsh->mainroute > -1)
            junct = true;
        
        primitives.push_back(QVector<ShapePrimitive>());
        tFile = GetObjFile("inbk3.obj");
        float length = line1.length;
        ComplexLine *line3 = &line2;
        int pathidx = 1;
        Mat4::copy(matrixS, matrixS2);
        if(line2.length < line1.length){
            length = line2.length;
            line3 = &line1;
            pathidx = 0;
            Mat4::copy(matrixS, matrixS1);
        }
        float i = 0;
        for(i = 0; i < length; i += 0.65){
            line1.getDrawPosition(posRot1, i);
            line2.getDrawPosition(posRot2, i);
            Vec3::transformMat4(posRot1, posRot1, matrixS1);
            Vec3::transformMat4(posRot2, posRot2, matrixS2);
            float distance = Vec3::dist(posRot1, posRot2)*0.5;
            
            Vec3::add(posRot, posRot1, posRot2);
            Vec3::scale(posRot, posRot, 0.5);
            posRot[3] = posRot1[3]*0.5 + posRot2[3]*0.5;
            posRot[4] = posRot1[4]*0.5 + posRot2[4]*0.5;
            posRot[5] = posRot1[5]*0.5 + posRot2[5]*0.5;
            Quat::fill(qr);
            Quat::rotateY(qr, qr, -tsh->path[0].rotDeg*M_PI/180.0);
            Quat::fromRotationXYZ(q1, (float*)(posRot1+3));
            Quat::multiply(q1, qr, q1);
            Quat::fill(qr);
            Quat::rotateY(qr, qr, -tsh->path[1].rotDeg*M_PI/180.0);
            Quat::fromRotationXYZ(q2, (float*)(posRot2+3));
            Quat::multiply(q2, qr, q2);
            if(!junct)
                Quat::slerp(q, q1, q2, 0.5);
            else if(tsh->mainroute == 0)
                Quat::copy(q, q1);
            else
                Quat::copy(q, q2);
            //Quat::multiply(q, q1, q2);
            zangle = 0;//angles[j*2]*(1.0-i/line1.length) + angles[j*2+1]*(i/line1.length);
            //Quat::rotateZ(q, q, zangle);
            Mat4::fromRotationTranslation(matrix1, q, posRot);
            
            //Quat::multiply(qr, qr, q);
            
            //Mat4::multiply(matrix1, matrixS, matrix1);
            
            primitives[0].push_back(ShapePrimitive());
            primitives[0].back().data = new float[1800];
            float* ptr = primitives[0].back().data;
            primitives[0].back().count = tFile->count;
            qDebug() << "count" << tFile->count;
            //Vec3::copy(primitives[0].back().pos, posRot);
            //Vec3::transformMat4(primitives[0].back().pos, primitives[0].back().pos, matrixS);
            //Mat4::copy(primitives[0].back().matrix, matrix1);
            //Quat::copy(primitives[0].back().quat, qr);
            //primitives[0].back().rotY = -tsh->path[0].rotDeg*M_PI/180.0;
            //primitives[0].back().rotZ = zangle;
            //primitives[0].back().templatePtr = tFile;
            PushShapePart(ptr, tFile, 0.155, matrix1, q, distance);
        }
        
        for(; i < line3->length; i += 0.65){
            line3->getDrawPosition(posRot, i);
            Quat::fill(qr);
            Quat::rotateY(qr, qr, -tsh->path[pathidx].rotDeg*M_PI/180.0);
            Quat::fromRotationXYZ(q, (float*)(posRot+3));
            zangle = 0;
            Quat::rotateZ(q, q, zangle);
            Mat4::fromRotationTranslation(matrix1, q, posRot);
            Quat::multiply(qr, qr, q);
            Mat4::multiply(matrix1, matrixS, matrix1);
            
            primitives[0].push_back(ShapePrimitive());
            primitives[0].back().data = new float[1800];
            float* ptr = primitives[0].back().data;
            primitives[0].back().count = tFile->count;
            qDebug() << "count" << tFile->count;
            Vec3::copy(primitives[0].back().pos, posRot);
            Vec3::transformMat4(primitives[0].back().pos, primitives[0].back().pos, matrixS);
            Mat4::copy(primitives[0].back().matrix, matrix1);
            Quat::copy(primitives[0].back().quat, qr);
            primitives[0].back().rotY = -tsh->path[pathidx].rotDeg*M_PI/180.0;
            primitives[0].back().rotZ = zangle;
            primitives[0].back().templatePtr = tFile;
            PushShapePart(ptr, tFile, 0.155, matrix1, qr);
        }
    }
    
    /*for(int j = 0; j < tsh->numpaths; j++){
        TrackShape::SectionIdx *section = &tsh->path[j];

        QVector<TSection> sections;
        for(int i = 0; i < section->n; i++){
            if(Game::currentRoute->tsection->sekcja[(int)section->sect[i]] != NULL)
            sections.push_back(*Game::currentRoute->tsection->sekcja[(int)section->sect[i]]);
        }

        //float* p = new float[4000000];
        //float* ptr = p;

        float q[4];    
        float qr[4];
        float posRot[6];
        float matrix1[16];
        float matrix2[16];
        ObjFile *tFile;


        ComplexLine line;
        line.init(sections);
        //qDebug() << line.length << "length";
        float pp[3];
        float zangle;
        
        Quat::fill(q);
        Quat::rotateY(q, q, -tsh->path[j].rotDeg*M_PI/180.0);
        Vec3::set(pp, -tsh->path[j].pos[0], tsh->path[j].pos[1], tsh->path[j].pos[2]);
        Mat4::fromRotationTranslation(matrixS, q, pp);
        
        primitives.push_back(QVector<ShapePrimitive>());
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
            
            primitives[j].push_back(ShapePrimitive());
            primitives[j].back().data = new float[1800];
            float* ptr = primitives[j].back().data;
            primitives[j].back().count = tFile->count;
            qDebug() << "count" << tFile->count;
            Vec3::copy(primitives[j].back().pos, posRot);
            Vec3::transformMat4(primitives[j].back().pos, primitives[j].back().pos, matrixS);
            Mat4::copy(primitives[j].back().matrix, matrix1);
            Quat::copy(primitives[j].back().quat, qr);
            primitives[j].back().rotY = -tsh->path[j].rotDeg*M_PI/180.0;
            primitives[j].back().rotZ = zangle;
            primitives[j].back().templatePtr = tFile;
            PushShapePart(ptr, tFile, 0.155, matrix1, qr);
        }
    }
    
    if(primitives.count() == 2){
        primitives.push_back(QVector<ShapePrimitive>());
        ObjFile *tFile;
        
        float pos[3];
        Vec3::set(pos, 0, 0, 0);
        for(int j = 0; j < primitives[0].count(); j++){
            for(int i = 0; i < primitives[1].count(); i++){
                if(primitives[0][j].disabled || primitives[1][i].disabled)
                    continue;
                if(Intersections::shapeIntersectsShape(
                        primitives[0][j].data, 
                        primitives[1][i].data, 
                        primitives[0][j].count*9, 
                        primitives[1][i].count*9, 
                        9,
                        9,
                        pos) > 0
                    ){
                    primitives[0][j].disabled = true;
                    primitives[1][i].disabled = true;
                    
                    primitives[2].push_back(ShapePrimitive());
                    primitives[2].back().data = new float[1800];
                    float* ptr = primitives[2].back().data;
                    tFile = primitives[0][j].templatePtr;
                    primitives[2].back().count = tFile->count;
                    float matrix[16];
                    float q[4];
                    float pos[3];
                    Vec3::add(pos, primitives[0][j].pos, primitives[1][i].pos);
                    Vec3::scale(pos, pos, 0.5);
                    /*
                    Quat::fill(q);
                    float zangle = primitives[0][j].rotZ*0.5 + primitives[1][i].rotZ * 0.5;
                    float yangle = primitives[0][j].rotY*0.5 + primitives[1][i].rotY * 0.5;
                    Quat::rotateY(q, q, yangle);
                    Quat::rotateZ(q, q, zangle);*/
                    //Quat::multiply(q, primitives[0][j].quat, primitives[1][i].quat);
                    //Mat4::fromRotationTranslation(matrix, q, pos);
                    //for(int i = 0; i < 16; i++){
                    //    matrix[i] = primitives[0][j].matrix[i]*0.5 + primitives[1][i].matrix[i]*0.5;
                    //}
                    //Mat4::multiply(matrix, primitives[0][j].matrix, primitives[1][i].matrix);
                    
                    //PushShapePart(ptr, tFile, 0.155, matrix, q);
                    //PushShapePart(ptr, tFile, 0.155, primitives[1][i].matrix, primitives[1][i].quat);
                    
                //}
            //}
        //}
    //}*/
    
    float* p = new float[4000000];
    float* ptr = p;
    QString resPath = Game::root + "/routes/" + Game::route + "/textures";
    QString* texturePath;
    
    for(int i = 0; i < primitives.count(); i++){
        for(int j = 0; j < (primitives[i]).count(); j++){
            if((primitives[i])[j].disabled)
                continue;
            memcpy(ptr, (primitives[i])[j].data, (primitives[i])[j].count * 9 * 4);
            ptr += (primitives[i])[j].count * 9;
        }
    }

    texturePath = new QString(resPath.toLower()+"/rails1.png");
    shape.push_back(new OglObj());
    shape.back()->setMaterial(texturePath);
    shape.back()->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );
        
    // Rails and Ballast - deprecated
    
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
    
    delete[] p;
    
}

void ProceduralShape::GenShape(QVector<OglObj*>& shape, TrackShape* tsh, QMap<int, float> &angles){
    if(!Loaded)
        Load();
    
    if(tsh == NULL)
        return;
    
    if(tsh->numpaths == 2 && tsh->xoverpts > 0){
        return GenTrackShape(shape, tsh, angles);
    }
    if(tsh->numpaths == 2 && tsh->mainroute > -1){
        return GenTrackShape(shape, tsh, angles);
    }
    
    float matrixS[16];
        
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

void ProceduralShape::PushShapePart(float* &ptr, ObjFile* tFile, float offsetY, float* matrix, float* qrot, float distance){
    int j = 0;
    float p[3];
    for(int i = 0; i < tFile->count; i++ ){
        p[0] = tFile->points[j++];
        p[1] = tFile->points[j++];
        p[2] = tFile->points[j++];
        if(distance > 0.01){
            if(p[0] > 0)
                p[0] += distance;
            if(p[0] < 0)
                p[0] -= distance;
        }
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