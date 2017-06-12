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

QMap<QString, ObjFile*> ProceduralShape::Files;
float ProceduralShape::Alpha = 0;

void ProceduralShape::GenShape(QVector<OglObj*> &shape, QVector<TSection> &sections) {
    QString path1 = "resources/tracks/inbk3.obj";
    QString path2 = "resources/tracks/uic60.obj";
    QString path3 = "resources/tracks/ballast1.obj";
    if(Files[path1] == NULL)
        Files[path1] = new ObjFile(path1);
    if(Files[path2] == NULL)
        Files[path2] = new ObjFile(path2);
    if(Files[path3] == NULL)
        Files[path3] = new ObjFile(path3);
    
    GLUU *gluu = GLUU::get();
    Alpha = -gluu->alphaTest;
    
    float* p = new float[2000000];
    float* ptr = p;
    
    
    
    float q[4];
    float posRot[6];
    float matrix1[16];
    float matrix2[16];
    QString resPath = Game::root + "/routes/" + Game::route + "/textures";
    QString* texturePath;
    
    ComplexLine line;
    line.init(sections);
    qDebug() << line.length << "length";
    
    ObjFile *tFile = Files[path1];
    for(float i = 0; i < line.length; i += 0.65){
        line.getDrawPosition(posRot, i);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix1, q, posRot);
        PushShapePart(ptr, tFile, 0.155, matrix1);
    }

    tFile = Files[path2];
    float step = 3;
    for(float i = 0; i < line.length; i += step){
        line.getDrawPosition(posRot, i);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix1, q, posRot);
        line.getDrawPosition(posRot, i + step);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix2, q, posRot);
        PushShapePart(ptr, tFile, 0.325, matrix1, matrix2, i, i+step);
    }    
    
    texturePath = new QString(resPath.toLower()+"/rails1.png");
    shape.push_back(new OglObj());
    shape.back()->setMaterial(texturePath);
    shape.back()->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );
    
    ptr = p;
    
    tFile = Files[path3];
    step = 4;
    for(float i = 0; i < line.length; i += step){
        line.getDrawPosition(posRot, i);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix1, q, posRot);
        line.getDrawPosition(posRot, i + step);
        Quat::fromRotationXYZ(q, (float*)(posRot+3));
        Mat4::fromRotationTranslation(matrix2, q, posRot);
        PushShapePart(ptr, tFile, 0.05, matrix1, matrix2, i, i+step);
    }   

    texturePath = new QString(resPath.toLower()+"/ballast1.png");
    shape.push_back(new OglObj());
    shape.back()->setMaterial(texturePath);
    shape.back()->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );
}

void ProceduralShape::PushShapePart(float* &ptr, ObjFile* tFile, float offsetY, float* matrix){
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
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = Alpha;
    }
}

void ProceduralShape::PushShapePart(float* &ptr, ObjFile* tFile, float offsetY, float* matrix1, float* matrix2, float dist1, float dist2){
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
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
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