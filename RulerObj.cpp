/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "RulerObj.h"
#include "GLMatrix.h"
#include "TrackObj.h"
#include "GLMatrix.h"
#include "TrackItemObj.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include "Game.h"
#include "DynTrackObj.h"
#include "TDB.h"
#include "TrackShape.h"
#include "TSectionDAT.h"
#include "Route.h"
#include "GeoCoordinates.h"
#include "ProceduralShape.h"

bool RulerObj::TwoPointRuler = false;
bool RulerObj::DrawPoints = false;

RulerObj::RulerObj() {
    this->internalLodControl = true;
    this->shape = -1;
    this->loaded = false;
    this->modified = false;
}

bool RulerObj::allowNew(){
    return true;
}

RulerObj::RulerObj(const RulerObj& o) : WorldObj(o){

    for(int i = 0; i < o.points.size(); i++){
        Point point;
        point.position[0] = o.points[i].position[0];
        point.position[1] = o.points[i].position[1];
        point.position[2] = o.points[i].position[2];
        points.push_back(point);
    }
    selectionValue = o.selectionValue;
    length = o.length;
    internalLodControl = o.internalLodControl;
}

WorldObj* RulerObj::clone(){
    return new RulerObj(*this);
}

RulerObj::~RulerObj() {
}

void RulerObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 1;
    this->box.loaded = false;
    setMartix();
    //this->point3d = new TrackItemObj();
    //this->point3d->setMaterial(1,1,1);
    //this->point3dSelected = new TrackItemObj();
    //this->point3dSelected->setMaterial(0.5,0.5,0.5);

    if(this->points.size() == 0){
        Point point;
        Vec3::copy(point.position, this->position);
        this->points.push_back(point);
        if(TwoPointRuler){
            Point point2;
            Vec3::copy(point2.position, this->position);
            point2.position[2] += 1;
            this->points.push_back(point2);
            selectionValue = 1;
        }
    }
}

void RulerObj::set(QString sh, QString val){
    
    WorldObj::set(sh, val);
    return;
}

void RulerObj::set(QString sh, FileBuffer* data) {
    if (sh == ("points")) {
        int pointCount = ParserX::GetNumber(data);
        for(int i=0; i< pointCount; i++){
            Point point;
            point.position[0] = ParserX::GetNumber(data);
            point.position[1] = ParserX::GetNumber(data);
            point.position[2] = -ParserX::GetNumber(data);
            points.push_back(point);
        }
        ParserX::SkipToken(data);
        return;
    }
    if (sh == ("shapetemplate")) {
        shapeEnabled = true;
        templateName = ParserX::GetStringInside(data);
        return;
    }
    
    WorldObj::set(sh, data);
    return;
}

void RulerObj::reload(){
    proceduralShapeInit = false;
    for(int j = 0; j < points.size() - 1; j++){
        points[j].procShape.clear();
    }
}

void RulerObj::setTemplate(QString name){
    templateName = name;
    shapeEnabled = true;
    modified = true;
    reload();
}

void RulerObj::setPosition(int x, int z, float* p){
    if(selectionValue > 0){
        points[selectionValue].position[0] = -2048*(this->x-x) + p[0];
        points[selectionValue].position[1] = p[1];
        points[selectionValue].position[2] = -2048*(this->y-z) + p[2];
    } else {
        Point point;
        point.position[0] = -2048*(this->x-x) + p[0];
        point.position[1] = p[1];
        point.position[2] = -2048*(this->y-z) + p[2];
        if(Vec3::dist(points.back().position, point.position) > 1)
            points.push_back(point);
    }
    modified = true;
    if(line3d != NULL)
        line3d->deleteVBO();
}

void RulerObj::refreshLength(){
    length = 0;
    geoLength = 0;
    
    IghCoordinate igh1, igh2;
    LatitudeLongitudeCoordinate latlon1, latlon2;
    PreciseTileCoordinate coords1, coords2;
    
    for(int i = 0; i < points.size() - 1; i++){
        length += Vec3::distance(points[i].position, points[i+1].position);
        
        coords1.setTWxyz(x, -y, points[i].position[0], points[i].position[1], points[i].position[2]);
        coords2.setTWxyz(x, -y, points[i+1].position[0], points[i+1].position[1], points[i+1].position[2]);
        Game::GeoCoordConverter->ConvertToInternal(&coords1, &igh1);
        Game::GeoCoordConverter->ConvertToInternal(&coords2, &igh2);
        Game::GeoCoordConverter->ConvertToLatLon(&igh1, &latlon1);
        Game::GeoCoordConverter->ConvertToLatLon(&igh2, &latlon2);
        geoLength += latlon1.distanceTo(&latlon2);
        
    }
    
    
}

float RulerObj::getLength(){
    return length;
    
}float RulerObj::getGeoLength(){
    return geoLength;
}

void RulerObj::createRoadPaths(){
    float tlength = 0;
    DynTrackObj* dobj = new DynTrackObj();
    dobj->load(x, y);
    float p[3];
    float q[4];
    for(int i = 0; i < points.size() - 1; i++){
        tlength = Vec3::distance(points[i].position, points[i+1].position);
        Vec3::copy(p, points[i].position);
        
        int someval = (((points[i+1].position[2]-points[i].position[2])+0.00001f)/fabs((points[i+1].position[2]-points[i].position[2])+0.00001f));
        float rotY = ((float)someval+1.0)*(M_PI/2)+(float)(atan((points[i].position[0]-points[i+1].position[0])/(points[i].position[2]-points[i+1].position[2]))); 
        float rotX = -(float)(asin((points[i].position[1]-points[i+1].position[1])/(tlength))); 

        Quat::fill(q);
        Quat::rotateY(q, q, rotY);
        Quat::rotateX(q, q, rotX);

        dobj->sections[0].a = floor((tlength * 10 ) + 0.5) / 10;
        Game::roadDB->fillDynTrack(dobj);
        Game::roadDB->placeTrack(x, y, (float*) &p, (float*) &q, dobj->sectionIdx, UiD);
        
    }
}

void RulerObj::removeRoadPaths(){
    bool ok;
    ok = Game::roadDB->removeTrackFromTDB(x, y, UiD);
    //if(ok)
}

void RulerObj::enableShape(){
    if(points.size() < 2)
        return;
    
    shapeEnabled = true;
    modified = true;

}

void RulerObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if (!loaded) return;
    if (jestPQ < 2) return;
    
    if(Game::showWorldObjPivotPoints){
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.9,0.9,0.7);
        }
        pointer3d->render(selectionColor);
    }
    
    int useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    
    if(shapeEnabled){
        if(proceduralShapeInit){
            for(int j = 0; j < points.size() - 1; j++){
                gluu->mvPushMatrix();
                Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, points[j].matrix);
                gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
                for(int i = 0; i < points[j].procShape.size(); i++){
                    points[j].procShape[i]->render(selectionColor | (i&0xF)*useSC);
                }
                gluu->mvPopMatrix();
            }
        } else {
            //templateName = "Siec1";
            qDebug() << templateName;
            for(int i = 0; i < points.size() - 1; i++){
                float tlength = Vec3::distance(points[i].position, points[i+1].position);
                int someval = (((points[i+1].position[2]-points[i].position[2])+0.00001f)/fabs((points[i+1].position[2]-points[i].position[2])+0.00001f));
                float rotY = ((float)someval+1.0)*(M_PI/2)+(float)(atan((points[i].position[0]-points[i+1].position[0])/(points[i].position[2]-points[i+1].position[2]))); 
                float rotX = (float)(asin((points[i].position[1]-points[i+1].position[1])/(tlength))); 

                Quat::fill(points[i].quat);
                Quat::rotateY(points[i].quat, points[i].quat, rotY + M_PI);
                Quat::rotateX(points[i].quat, points[i].quat, rotX);
                Mat4::fromRotationTranslation(points[i].matrix, points[i].quat, points[i].position);
                QVector<TSection> sections;
                sections.push_back(TSection());
                sections.back().size = floor((tlength * 10 ) + 0.5) / 10;
                ProceduralShape::GetShape(templateName, points[i].procShape, sections, i);
            }
            proceduralShapeInit = true;
        }
    }
    
    if (renderMode == GLUU::RENDER_SHADOWMAP) return;
    if(!Game::viewInteractives) 
        return;
    
    if(point3d == NULL){
        point3d = new OglObj();
        point3d->setMaterial(1,1,1);
        point3d->setLineWidth(8);
        float *punkty = new float[6];
        int ptr = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 10;
        punkty[ptr++] = 0;
        point3d->init(punkty, ptr, point3d->V, GL_LINES);
        
        point3dSelected = new OglObj();
        point3dSelected->setLineWidth(8);
        point3dSelected->setMaterial(0.5,0.5,0.5);
        ptr = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 10;
        punkty[ptr++] = 0;
        point3dSelected->init(punkty, ptr, point3dSelected->V, GL_LINES);
        delete[] punkty;
    }
    if(line3d == NULL){
        line3d = new OglObj();
        line3d->setLineWidth(2);
        line3d->setMaterial(1,1,1);
    }
    if(!line3d->loaded){
        float *punkty = new float[points.size()*6*2]; 
        int ptr = 0;
        
        for(int i = 0; i < points.size() - 1; i++){
            punkty[ptr++] = points[i].position[0];
            punkty[ptr++] = points[i].position[1]+1;
            punkty[ptr++] = points[i].position[2];
            punkty[ptr++] = points[i+1].position[0];
            punkty[ptr++] = points[i+1].position[1]+1;
            punkty[ptr++] = points[i+1].position[2];
        }
        line3d->init(punkty, ptr, line3d->V, GL_LINES);
        delete[] punkty;
        refreshLength();
    }
    
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    line3d->render(selectionColor);
    
    for(int i = 0; i < points.size(); i++){
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, points[i].position[0], points[i].position[1], points[i].position[2]);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(i == 0 || i == points.size() - 1 || DrawPoints)
            if(this->selected && this->selectionValue == i) 
                point3dSelected->render(selectionColor | (i&0xF)*useSC);
            else
                point3d->render(selectionColor | (i&0xF)*useSC);
        //    pointer3d->render(selectionColor + (i+1)*131072*8*useSC);
        gluu->mvPopMatrix();
    }
};

bool RulerObj::hasLinePoints(){
    return true;
}

void RulerObj::getLinePoints(float *&punkty){
    
    float pos[3];
    for(float i = 0; i < this->length; i += 1){
        getPosition(i, pos);
        *punkty++ = pos[0];
        *punkty++ = pos[1];
        *punkty++ = pos[2];
    }
    //for(int i = 0; i < points.size(); i++){
    //    *punkty++ = points[i].position[0];
    //    *punkty++ = points[i].position[1];
    //    *punkty++ = points[i].position[2];
    //}
    return;
}

void RulerObj::getPosition(float len, float* pos){
    float slen = 0;
    float tlen;
    for(int i = 0; i < points.size()-1; i++){
        tlen = Vec3::dist(points[i].position, points[i+1].position);
        slen += tlen;
        if(slen > len){
            float len1 = len - slen + tlen;
            float len2 = slen - len;
            len1 = len1/tlen;
            len2 = len2/tlen;
            pos[0] = len1*points[i].position[0] + len2*points[i+1].position[0];
            pos[1] = len1*points[i].position[1] + len2*points[i+1].position[1];
            pos[2] = len1*points[i].position[2] + len2*points[i+1].position[2];
            return;
        }
    }
    pos[0] = points[points.size()-1].position[0];
    pos[1] = points[points.size()-1].position[1];
    pos[2] = points[points.size()-1].position[2];
    
}

bool RulerObj::select(int value){
    this->selectionValue = value;
    this->selected = true;
}


void RulerObj::save(QTextStream* out){
    if (!loaded) return;
    if (jestPQ < 2) return;
    
*(out) << "	Ruler (\n";

*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		Points ( " << points.size()<<" \n";
for(int i = 0; i < points.size(); i++)
*(out) << "			Point ( "<<points[i].position[0]<<" "<<points[i].position[1]<<" "<<-points[i].position[2]<<" )\n";
*(out) << "		)\n";
if(shapeEnabled){
*(out) << "		ShapeTemplate ( "<<ParserX::AddComIfReq(templateName)<<" )\n";
}
*(out) << "	)\n";
}