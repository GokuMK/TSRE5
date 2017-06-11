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
    QString path = "resources/tracks/inbk3.obj";
    if(Files[path] == NULL)
        Files[path] = new ObjFile(path);
    
    GLUU *gluu = GLUU::get();
    Alpha = -gluu->alphaTest;
    
    float* p = new float[1000000];
    float* ptr = p;
    
    ObjFile *tFile = Files[path];
    
    float length = 0;
    float pos[3];
    float tpos[3];
    float trot[3];
    Vector3f vPos;
    float q[4];
    float rot[3];
    //float rotY;
    float matrix[16];
    for(int i = 0; i < sections.size(); i++){
        length += sections[i].getDlugosc();
    }
    
    int currentSection = 0;
    float tLength = 0;
    float cLength = sections[currentSection].getDlugosc();
    Vec3::set(tpos, 0, 0, 0);
    Vec3::set(trot, 0, 0, 0);
    qDebug() << length << "length";
    for(float i = 0; i < length; i += 0.65){
        if(i > tLength + cLength){
            qDebug() << "t+c" << tLength + cLength;
            sections[currentSection].getDrawPosition(&vPos, cLength);
            vPos.rotateY(trot[1], 0);
            Vec3::add(tpos, tpos, (float*)&vPos);
            trot[1] += sections[currentSection].getDrawAngle(cLength);
            currentSection++;
            if(currentSection > sections.size() - 1)
                break;
            tLength += cLength;
            cLength = sections[currentSection].getDlugosc();
        }
        qDebug() << "length" << i;
        sections[currentSection].getDrawPosition(&vPos, i - tLength);
        vPos.rotateY(trot[1], 0);
        Vec3::set(pos, vPos.x, vPos.y, vPos.z);
        Vec3::add(pos, pos, tpos);
        rot[0] = M_PI;
        rot[1] = - trot[1] - sections[currentSection].getDrawAngle(i - tLength);
        rot[2] = 0;
        Quat::fromRotationXYZ(q, rot);
        Mat4::fromRotationTranslation(matrix, q, pos);
        PushShapePart(ptr, tFile, matrix);
    }
    //PushShapePart(ptr, tFile, pos, rotY);
    qDebug() << "length";
    QString resPath = Game::root + "/routes/" + Game::route + "/textures";
    QString* texturePath = new QString(resPath.toLower()+"/blank.ace");
    shape.push_back(new OglObj());
    shape[0]->setMaterial(texturePath);
    shape[0]->init(p, ptr - p, OglObj::VNT, GL_TRIANGLES );
    //shape.push_back(new OglObj());
    //texturePath = new QString(resPath.toLower()+"/acleantrack2.ace");
    //shape[1]->setMaterial(texturePath);
    //shape[1]->init(sk, str, OglObj::VNT, GL_TRIANGLES );
}

void ProceduralShape::PushShapePart(float* &ptr, ObjFile* tFile, float* matrix){
    int j = 0;
    float p[3];
    for(int i = 0; i < tFile->count; i++ ){
        p[0] = tFile->points[j++];
        p[1] = tFile->points[j++];
        p[2] = tFile->points[j++];
        Vec3::transformMat4(p, p, matrix);
        *ptr++ = p[0];
        *ptr++ = p[1] + 0.3;
        *ptr++ = p[2];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = tFile->points[j++];
        *ptr++ = Alpha;
    }
}