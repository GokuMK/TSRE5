/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <QFile>
#include "ForestObj.h"
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
#include "TDB.h"
#include "Vector4f.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

QVector<ForestObj::ForestList> ForestObj::forestList;
float ForestObj::ForestClearDistance = 0;

ForestObj::ForestObj() {
    this->loaded = false;
}

ForestObj::ForestObj(const ForestObj& o) : WorldObj(o) {
    treeTexture = o.treeTexture;
    scaleRangeX = o.scaleRangeX;
    scaleRangeZ = o.scaleRangeZ;
    areaX = o.areaX;
    areaZ = o.areaZ;
    treeSizeX = o.treeSizeX;
    treeSizeZ = o.treeSizeZ;
    population = o.population;
    tex = o.tex;
    init = false;
    if(o.texturePath != NULL){
        texturePath = new QString();
        *texturePath = *o.texturePath;
    }

}

WorldObj* ForestObj::clone(){
    return new ForestObj(*this);
}

ForestObj::~ForestObj() {
}

void ForestObj::LoadForestList(){
    QString path = Game::root + "/routes/" + Game::route + "/forests.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    ParserX::NextLine(data);

    QString sh = "";
    //ParserX::szukajsekcji1(sh, data);
    //ParserX::GetNumber(data);
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("forest")) {
            ForestObj::forestList.push_back(ForestObj::ForestList());
            ForestObj::forestList.back().name = ParserX::GetString(data);
            ForestObj::forestList.back().texture = ParserX::GetString(data);
            ForestObj::forestList.back().treeSizeX = ParserX::GetNumber(data);
            ForestObj::forestList.back().treeSizeZ = ParserX::GetNumber(data);
            ForestObj::forestList.back().scaleRangeX = ParserX::GetNumber(data);
            ForestObj::forestList.back().scaleRangeZ = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }
}

int ForestObj::GetListIdByTexture(QString texture){
    for(int i = 0; i < forestList.size(); i++){
        if(forestList[i].texture.toLower() == texture.toLower())
            return i;
    }
    return 0;
}

void ForestObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->loaded = true;
    this->size = -1;
    this->tex = -1;
    this->init = false;
    this->skipLevel = 3;
    this->modified = false;
}

void ForestObj::set(QString sh, QString val){
    if (sh == ("ref_filename")) {
        treeTexture = val;
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void ForestObj::set(QString sh, long long int val) {
    if (sh == ("ref_value")) {
        qDebug() << val;
        int val2        = val & 0xFFFF;
        int tareax      = (val >> 16) & 0xFFF;
        int tareaz      = (val >> 28) & 0xFFF;
        int tpopulation = (val >> 40) & 0xFFFFFF;    
        treeTexture = ForestObj::forestList[val2].texture;
        scaleRangeX = ForestObj::forestList[val2].scaleRangeX;
        scaleRangeZ = ForestObj::forestList[val2].scaleRangeZ;
        areaX = 100;
        if(tareax != 0)
            areaX = tareax;
        areaZ = 100;
        if(tareaz != 0)
            areaZ = tareaz;
        treeSizeX = ForestObj::forestList[val2].treeSizeX;
        treeSizeZ = ForestObj::forestList[val2].treeSizeZ;
        population = 10;
        if(tpopulation != 0)
            population = tpopulation;
        return;
    }
    if (sh == ("population")) {
        population = val;
        return;
    }
    WorldObj::set(sh, val);
    this->modified = true;
}

void ForestObj::set(QString sh, float val) {
    if (sh == ("areaX")) {
        areaX = val;
        return;
    }
    if (sh == ("areaZ")) {
        areaZ = val;
        return;
    }
    WorldObj::set(sh, val);
    this->modified = true;
}

void ForestObj::set(int sh, FileBuffer* data) {
    if (sh == TS::TreeTexture) {
        data->off++;
        int slen = data->getShort()*2;
        treeTexture = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    if (sh == TS::ScaleRange) {
        data->off++;
        scaleRangeX = data->getFloat();
        scaleRangeZ = data->getFloat();
        return;
    }
    if (sh == TS::Area) {
        data->off++;
        areaX = data->getFloat();
        areaZ = data->getFloat();
        return;
    }
    if (sh == TS::TreeSize) {
        data->off++;
        treeSizeX = data->getFloat();
        treeSizeZ = data->getFloat();
        return;
    }
    if (sh == TS::Population) {
        data->off++;
        population = data->getUint();
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void ForestObj::set(QString sh, FileBuffer* data) {
    if (sh == ("treetexture")) {
        treeTexture = ParserX::GetString(data);
        return;
    }
    if (sh == ("scalerange")) {
        scaleRangeX = ParserX::GetNumber(data);
        scaleRangeZ = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("area")) {
        areaX = ParserX::GetNumber(data);
        areaZ = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("treesize")) {
        treeSizeX = ParserX::GetNumber(data);
        treeSizeZ = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("population")) {
        population = ParserX::GetNumber(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

Ref::RefItem* ForestObj::getRefInfo(){
    Ref::RefItem* r = new Ref::RefItem();
    r->type = type;
    r->filename.push_back(treeTexture);
    
    r->value       = (long long int)this->GetListIdByTexture(treeTexture) & 0xFFFF;
    r->value      |= ((long long int)areaX & 0xFFF) << 16;
    r->value      |= ((long long int)areaZ & 0xFFF) << 28;
    r->value |= ((long long int)population & 0xFFFFFF) << 40;    
    return r;
}

void ForestObj::translate(float px, float py, float pz){
    this->position[0]+=px;
    //this->position[1]+=py;
    this->position[2]+=pz;
    this->modified = true;
    deleteVBO();
}

void ForestObj::rotate(float x, float y, float z){
    if(matrix3x3 != NULL) matrix3x3 = NULL;
    if(x!=0) Quat::rotateX(this->qDirection, this->qDirection, x);
    if(y!=0) Quat::rotateY(this->qDirection, this->qDirection, y);
    if(z!=0) Quat::rotateZ(this->qDirection, this->qDirection, z);
    this->modified = true;
    deleteVBO();
}

void ForestObj::resize(float x, float y, float z){
    if(x == 0 && y == 0) return;
    this->areaX += x;
    this->areaZ += y;
    this->modified = true;
    deleteVBO();
}

void ForestObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if (!loaded) return;
    //if (jestPQ < 2) return;
    //GLUU* gluu = GLUU::get();
    //if((this.position===undefined)||this.qDirection===undefined) return;

    /*if (size > 0) {
        if ((lod > size)) {
            float v1[2];
            v1[0] = pos[0] - (target[0]);
            v1[1] = pos[2] - (target[2]);
            float v2[2];
            v2[0] = posx;
            v2[1] = posz;
            float iloczyn = v1[0] * v2[0] + v1[1] * v2[1];
            float d1 = sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
            float d2 = sqrt(v2[0] * v2[0] + v2[1] * v2[1]);
            float zz = iloczyn / (d1 * d2);
            if (zz > 0) return;

            float ccos = cos(fov) + zz;
            float xxx = sqrt(2 * d2 * d2 * (1 - ccos));
            //if((ccos > 0) && (xxx > 200+50)) return;
            if ((ccos > 0) && (xxx > size + 150) && (skipLevel == 1)) return;
        }
    } else {
        if (ShapeLib::shape[shape]->loaded)
            size = ShapeLib::shape[shape]->size;
    }
*/
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, position[0], 0, position[2]);
    //float scale = sqrt(qDirection[0] * qDirection[0] + qDirection[1] * qDirection[1] + qDirection[2] * qDirection[2]);
    //float angle = ((acos(qDirection[3])*360) / M_PI);
    //Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, gluu->degToRad(-angle), -qDirection[0] * scale, -qDirection[1] * scale, qDirection[2] * scale);
    //Mat4::rotate(gluu->mvMatrix, gluu->mvMatrix, gluu->degToRad(180), 0, -1, 0);

    //if(selected){
    //    selected = !selected;
    //    selectionColor = 155;
    //}
    //gluu.setMatrixUniforms();

    //
    //var z = this.position[0]*mmm[9] + this.position[1]*mmm[7] + this.position[2]*mmm[9];

    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
   
    if(selectionColor != 0){
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        shape.setMaterial((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f);
    } else {
        shape.setMaterial(texturePath);
    }
    
    drawShape();
};

void ForestObj::drawShape(){
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    /*if (tex == -2) {
        f->glDisable(GL_TEXTURE_2D);
    } else {
        f->glEnable(GL_TEXTURE_2D);
        if (tex == -1) {
            tex = TexLib::addTex(resPath, treeTexture);
            f->glDisable(GL_TEXTURE_2D);
        }
    }*/

    if (!init) {
        if(!Game::ignoreLoadLimits){
            if(Game::allowObjLag < 1)  return;
            Game::allowObjLag-=2;
        }
        
        //qint64 timeNow = QDateTime::currentMSecsSinceEpoch();
            
            int iloscv = population*24;
            float* punkty = new float[iloscv*9];
            int ptr = 0;
            
            GLUU* gluu = GLUU::get();
            float alpha = -gluu->alphaTest;

            int seed = (int)(position[0] + position[1] + position[2]);
            //Random random = new Random(seed);
            std::srand(seed);
            float treeSizeXt = treeSizeX*0.7;
            float posT[2];
            posT[0] = x;
            posT[1] = y;
            //qDebug() << x << y;
            float tposx, tposz;
            
            QVector<Vector4f> fpoints;
            float off = ((qDirection[1]+0.00001f)/fabs(qDirection[1]+0.00001f))*(float)-acos(qDirection[3])*2.0;
            for(int uu = 0; uu < population; uu++){
                
                tposx = ((float)((std::rand()%1000))/1000)*areaX-areaX/2.0;
                tposz = ((float)((std::rand()%1000))/1000)*areaZ-areaZ/2.0;
                
                Vector2f uuu(tposx,tposz);
                uuu.rotate(off, 0);
                tposx = uuu.x;
                tposz = uuu.y;
                                
                fpoints.push_back(Vector4f(tposx+position[0], 0, tposz+position[2], 9999));
            }
            
            if(ForestClearDistance > 0){
                float bBox[4];
                bBox[0] = bBox[2] = 9999;
                bBox[1] = bBox[3] = -9999;
                Vector2f v;
                for(int u = -1; u < 2; u+=2)
                    for(int y = -1; y < 2; y+=2){
                        v.set(areaX*u/2,areaZ*y/2);
                        v.rotate(off, 0);
                        if(v.x < bBox[0])
                            bBox[0] = v.x;
                        if(v.x > bBox[1])
                            bBox[1] = v.x;
                        if(v.y < bBox[2])
                            bBox[2] = v.y;
                        if(v.y > bBox[3])
                            bBox[3] = v.y;
                    }
                bBox[0] += position[0];
                bBox[1] += position[0];
                bBox[2] += position[2];
                bBox[3] += position[2];            
                qDebug() << bBox[0] << bBox[1] << bBox[2] << bBox[3];
            
                if(Game::trackDB != NULL)
                    Game::trackDB->fillNearestSquaredDistanceToTDBXZ(posT, fpoints, bBox);
                if(Game::roadDB != NULL)
                    Game::roadDB->fillNearestSquaredDistanceToTDBXZ(posT, fpoints, bBox);
            }
            for(int uu = 0; uu < population; uu++){
                if(ForestClearDistance > 0){
                    if(fpoints[uu].c < ForestClearDistance*ForestClearDistance)
                        continue;
                }

                float wysokosc = Game::terrainLib->getHeight(x, y, fpoints[uu].x, fpoints[uu].z);
                
                tposx = fpoints[uu].x - position[0];
                tposz = fpoints[uu].z - position[2];
                
                for(int j = -1; j < 2; j+=2){
                    for(int i = -1; i<2; i+=2){
                            punkty[ptr++] = -treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc+treeSizeZ;
                            punkty[ptr++] = -treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 0; punkty[ptr++] = 0;
                            punkty[ptr++] = alpha;
                            
                            punkty[ptr++] = treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc+treeSizeZ;
                            punkty[ptr++] = treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = alpha;
                            
                            punkty[ptr++] = treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc;
                            punkty[ptr++] = treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 1; punkty[ptr++] = 1;
                            punkty[ptr++] = alpha;
                            
                            punkty[ptr++] = -treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc;
                            punkty[ptr++] = -treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 0; punkty[ptr++] = 1;
                            punkty[ptr++] = alpha;
                            
                            punkty[ptr++] = -treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc+treeSizeZ;
                            punkty[ptr++] = -treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 0; punkty[ptr++] = 0;
                            punkty[ptr++] = alpha;
                            
                            punkty[ptr++] = treeSizeXt*i*j/2.0 + tposx;
                            punkty[ptr++] = wysokosc;
                            punkty[ptr++] = treeSizeXt*i/2.0 + tposz;
                            punkty[ptr++] = 0; punkty[ptr++] = 1; punkty[ptr++] = 0;
                            punkty[ptr++] = 1; punkty[ptr++] = 1;
                            punkty[ptr++] = alpha;
                    }
                }
            }
            
        texturePath = new QString(resPath.toLower()+"/"+treeTexture.toLower());
        shape.setMaterial(texturePath);
        shape.init(punkty, ptr, RenderItem::VNTA, GL_TRIANGLES);
        /*shape.VAO.create();
        QOpenGLVertexArrayObject::Binder vaoBinder(&shape.VAO);

        shape.VBO.create();
        shape.VBO.bind();
        shape.VBO.allocate(punkty, ptr * sizeof (GLfloat));
        f->glEnableVertexAttribArray(0);
        f->glEnableVertexAttribArray(1);
        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);
        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (6 * sizeof (GLfloat)));
        shape.VBO.release();*/
        //shape.iloscv = ptr/8;
        //qint64 timeNow2 = QDateTime::currentMSecsSinceEpoch();
        //qDebug() << "forest gen time: " << (timeNow2 - timeNow);
        delete[] punkty;
        init = true;
    }
    shape.render();
    if(selected){
        drawBox();
    }
}

bool ForestObj::getBoxPoints(QVector<float>& points){
    if (!loaded) 
        return false;
    float scale = (float) sqrt(qDirection[0] * qDirection[0] + qDirection[1] * qDirection[1] + qDirection[2] * qDirection[2]);
    float off = ((qDirection[1]+0.000001f)/fabs(scale+0.000001f))*(float)-acos(qDirection[3])*2;
        
            Vector2f x1y1(-areaX/2,-areaZ/2, off, 0);
            Vector2f x1y2(-areaX/2,areaZ/2, off, 0);
            Vector2f x2y1(areaX/2,-areaZ/2, off, 0);
            Vector2f x1y12 = x1y2.subv(x1y1);
            Vector2f x12y1 = x2y1.subv(x1y1);
            float x1y12d = x1y12.getDlugosc();
            float x12y1d = x12y1.getDlugosc();
            float wysokosc = 0;
            float step = 2;
            
            for(float i = 0; i <= x1y12d; i+=x1y12d){
                for(float j = 0; j < x12y1d; j+=step){
                    float jj = j+step;
                    if(jj>x12y1d) jj = x12y1d;          

                    wysokosc = Game::terrainLib->getHeight(x, y, x1y1.x + x1y12.divf(x1y12d/i).x + x12y1.divf(x12y1d/j).x + position[0], ( x1y1.y + x1y12.divf(x1y12d/i).y + x12y1.divf(x12y1d/j).y + position[2]));
                    points << x1y1.x + x1y12.divf(x1y12d/i).x + x12y1.divf(x12y1d/j).x;
                    points << wysokosc+0.5f;
                    points << x1y1.y + x1y12.divf(x1y12d/i).y + x12y1.divf(x12y1d/j).y;
                    wysokosc = Game::terrainLib->getHeight(x, y, x1y1.x + x1y12.divf(x1y12d/i).x + x12y1.divf(x12y1d/jj).x + position[0], ( x1y1.y + x1y12.divf(x1y12d/i).y + x12y1.divf(x12y1d/jj).y + position[2]));
                    points << x1y1.x + x1y12.divf(x1y12d/i).x + x12y1.divf(x12y1d/jj).x;
                    points << wysokosc+0.5f;
                    points << x1y1.y + x1y12.divf(x1y12d/i).y + x12y1.divf(x12y1d/jj).y;
                }
            }
            for(float j = 0; j <= x12y1d; j+=x12y1d){
                for(float i = 0; i < x1y12d; i+=step){
                    float ii = i+step;
                    if(ii>x1y12d) ii = x1y12d;          

                    wysokosc = Game::terrainLib->getHeight(x, y, x1y1.x + x1y12.divf(x1y12d/i).x + x12y1.divf(x12y1d/j).x + position[0], ( x1y1.y + x1y12.divf(x1y12d/i).y + x12y1.divf(x12y1d/j).y + position[2]));
                    points << x1y1.x + x1y12.divf(x1y12d/i).x + x12y1.divf(x12y1d/j).x;
                    points << wysokosc+0.5f;
                    points << x1y1.y + x1y12.divf(x1y12d/i).y + x12y1.divf(x12y1d/j).y;
                    wysokosc = Game::terrainLib->getHeight(x, y, x1y1.x + x1y12.divf(x1y12d/ii).x + x12y1.divf(x12y1d/j).x + position[0], ( x1y1.y + x1y12.divf(x1y12d/ii).y + x12y1.divf(x12y1d/j).y + position[2]));
                    points << x1y1.x + x1y12.divf(x1y12d/ii).x + x12y1.divf(x12y1d/j).x;
                    points << wysokosc+0.5f;
                    points << x1y1.y + x1y12.divf(x1y12d/ii).y + x12y1.divf(x12y1d/j).y;
                }
            }
            return true;
}

int ForestObj::getDefaultDetailLevel(){
    return -11;
}

void ForestObj::save(QTextStream* out){
    if (!loaded) return;
    if(Game::useOnlyPositiveQuaternions)
        Quat::makePositive(this->qDirection);
    
*(out) << "	Forest (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
if(this->treeTexture != "")
*(out) << "		TreeTexture ( "<<ParserX::AddComIfReq(this->treeTexture)<<" )\n";
*(out) << "		ScaleRange ( "<<this->scaleRangeX<<" "<<this->scaleRangeZ<<" )\n";
*(out) << "		Area ( "<<this->areaX<<" "<<this->areaZ<<" )\n";
*(out) << "		Population ( "<<this->population<<" )\n";
*(out) << "		TreeSize ( "<<this->treeSizeX<<" "<<this->treeSizeZ<<" )\n";
*(out) << "		StaticFlags ( "<<ParserX::MakeFlagsString(this->staticFlags)<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel >= 0)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}

bool ForestObj::allowNew(){
    return true;
}

void ForestObj::deleteVBO(){
    //this->shape.deleteVBO();
    this->init = false;
    this->box.deleteVBO();
}