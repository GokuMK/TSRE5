/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ComplexLine.h"
#include "GLMatrix.h"
#include "Vector3f.h"
#include <QDebug>

ComplexLinePoint::ComplexLinePoint(){
    
}

ComplexLinePoint::ComplexLinePoint(const ComplexLinePoint& o){
    this->selected = o.selected;
    this->shapeType = o.shapeType;
    Vec3::copy(this->position, o.position);
}

ComplexLine::ComplexLine() {
}

ComplexLine::~ComplexLine() {
}

void ComplexLine::init(QVector<TSection> s){
    sections.append(s);
    length = 0;
    for(int i = 0; i < sections.size(); i++){
        length += sections[i].getDlugosc();
    }
}

void ComplexLine::init(QVector<ComplexLinePoint> s){
    points.append(s);
    length = 0;
    for(int i = 1; i < points.size(); i++){
        length += Vec3::distance(points[i].position, points[i-1].position);
        Vec3::sub(points[i].position, points[i].position, points[0].position);
    }
    Vec3::sub(points[0].position, points[0].position, points[0].position);
}

float ComplexLine::getLength(){
    return length;
}

void ComplexLine::getDrawPosition(float* posRot, float distance, float xOffset){
    if(sections.size() > 0)
        return getDrawPositionFromTSection(posRot, distance, xOffset);
    if(points.size() > 0)
        return getDrawPositionFromPoints(posRot, distance, xOffset);
}

void ComplexLine::getDrawPositionFromPoints(float* posRot, float distance, float xOffset){
    float tLength = 0;
    float sLength = 0;
    float tpos[3];
    float trot[3];
    Vec3::set(tpos, 0, 0, 0);
    Vec3::set(trot, 0, 0, 0);
    Vector3f vPos;
    Vector3f off;
    
    
    for(int i = 0; i < points.size() - 1; i++){
        sLength = Vec3::dist(points[i].position, points[i+1].position);
        //qDebug() << "sLength" << sLength << distance;
        if(distance > tLength + sLength){
            //sections[i].getDrawPosition(&vPos, sLength);
            //vPos.rotateY(trot[1], 0);
            //Vec3::add(tpos, tpos, (float*)&vPos);
            //trot[1] += sections[i].getDrawAngle(sLength);
            tLength += sLength;
            continue;
        }

        float ttLength = distance - tLength;
        
        posRot[0] = points[i].position[0]*(1.0 - ttLength/sLength) + points[i+1].position[0]*(ttLength/sLength);
        posRot[1] = points[i].position[1]*(1.0 - ttLength/sLength) + points[i+1].position[1]*(ttLength/sLength);
        posRot[2] = points[i].position[2]*(1.0 - ttLength/sLength) + points[i+1].position[2]*(ttLength/sLength);
        //vPos.rotateY(trot[1], 0);
        //Vec3::set(posRot, vPos.x, vPos.y, vPos.z);
        //Vec3::add(posRot, posRot, tpos);

        int someval = (((points[i+1].position[2]-points[i].position[2])+0.00001f)/fabs((points[i+1].position[2]-points[i].position[2])+0.00001f));
        float rotY = ((float)someval+1.0)*(M_PI/2)+(float)(atan((points[i].position[0]-points[i+1].position[0])/(points[i].position[2]-points[i+1].position[2]))); 
        float rotX = -(float)(asin((points[i].position[1]-points[i+1].position[1])/(sLength))); 
        
        posRot[3] = M_PI;//-rotX;
        posRot[4] = rotY;//- trot[1] - sections[i].getDrawAngle(distance - tLength);
        posRot[5] = M_PI;
        qDebug() << "distance" << distance << posRot[0] << posRot[1] << posRot[2] << posRot[3] << posRot[4];
        return;
    }
}

void ComplexLine::getDrawPositionFromTSection(float* posRot, float distance, float xOffset){
    float tLength = 0;
    float sLength = 0;
    float tpos[3];
    float trot[3];
    Vec3::set(tpos, 0, 0, 0);
    Vec3::set(trot, 0, 0, 0);
    Vector3f vPos;
    Vector3f off;
    
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
        /*off.set(xOffset, 0, 0);
        off.rotateY(sections[i].getDrawAngle(distance - tLength), 0);
        Vec3::set(tpos, off.x, off.y, off.z);
        Vec3::add(posRot, posRot, tpos);*/
        posRot[3] = M_PI;
        posRot[4] = - trot[1] - sections[i].getDrawAngle(distance - tLength);
        posRot[5] = 0;
        return;
    }
}