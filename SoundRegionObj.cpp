/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SoundRegionObj.h"
#include "TS.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include "TDB.h"
#include "TRitem.h"
#include "Game.h"
#include "TrackItemObj.h"
#include "TS.h"
#include "SoundList.h"
#include <QDebug>

SoundRegionObj::SoundRegionObj() {
}

SoundRegionObj::~SoundRegionObj() {
}

bool SoundRegionObj::allowNew(){
    return true;
}

bool SoundRegionObj::isTrackItem(){
    return true;
}

bool SoundRegionObj::isSoundItem(){
    return true;
}

void SoundRegionObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->fileName = "IMRegionPoint.s";
    this->staticFlags = 1048576;
    this->loaded = true;
    setMartix();
}

void SoundRegionObj::deleteTrItems(){
    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    for(int i = 0; i<this->trItemId.size()/2; i++){
        if(this->trItemId[i*2] == 0)
            tdb->deleteTrItem(this->trItemId[i*2+1]);
        else if(this->trItemId[i*2] == 1)
            rdb->deleteTrItem(this->trItemId[i*2+1]);
        this->trItemId[i*2+1] = -1;
    }
}

void SoundRegionObj::initTrItems(float* tpos){
    if(tpos == NULL)
        return;
    int trNodeId = tpos[0];
    float metry = tpos[1];
    
    TDB* tdb = Game::trackDB;
    qDebug() <<"new soundregion  "<<this->fileName;

    tdb->newSoundRegionObject(this->soundregionTrackType, trItemId, trNodeId, metry, this->typeID);
    float *srd = tdb->trackItems[this->trItemId[1]]->trItemSRData;
    this->soundregionRoty = srd[2];
    Quat::fill(this->qDirection);
    Quat::rotateY(this->qDirection, this->qDirection, this->soundregionRoty);
    this->position[1] += 1;
    this->drawPositionB = NULL;
}

void SoundRegionObj::flip(){

    TDB* tdb = Game::trackDB;
    for(int j = 0; j < this->trItemId.size()/2; j++){
        if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
            continue;
        tdb->trackItems[this->trItemId[j*2+1]]->flipSoundRegion();
    }
    float *srd = tdb->trackItems[this->trItemId[1]]->trItemSRData;
    this->soundregionRoty = srd[2];
    Quat::rotateY(this->qDirection, this->qDirection, M_PI);
    this->modified = true; 
    delete this->drawPositionB;
    this->drawPositionB = NULL;
}

void SoundRegionObj::set(QString sh, long long int val){
    if (sh == ("ref_value")) {
        //this->fileName = "IMRegionPoint.s";
        //for (auto it = Game::soundList->regions.begin(); it != Game::soundList->sources.end(); ++it ){
        //    if(it->second->id == val){
        this->soundregionTrackType = val;
        ///    }
        //}
        return;
    }
    if (sh == ("update_type")) {
        this->soundregionTrackType = val;
        TDB* tdb = Game::trackDB;
        for(int j = 0; j < this->trItemId.size()/2; j++){
            if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
                continue;
            tdb->trackItems[this->trItemId[j*2+1]]->trItemSRData[1] = val;
        }
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void SoundRegionObj::set(QString sh, QString val){
    if (sh == ("filename")) {
        fileName = val;
        return;
    }
    if (sh == ("ref_filename")) {
        fileName = val;
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void SoundRegionObj::set(int sh, FileBuffer* data) {
    if (sh == TS::FileName) {
        data->off++;
        int slen = data->getShort()*2;
        fileName = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SoundRegionObj::set(QString sh, FileBuffer* data) {
    if (sh == ("filename")) {
        fileName = ParserX::GetString(data);
        return;
    }
    if (sh == ("soundregionroty")) {
        soundregionRoty = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("soundregiontracktype")) {
        soundregionTrackType = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("tritemid")) {
        //this->trItemId = new int[2];
        //his->trItemId[0] = ParserX::GetUInt(data);
        //this->trItemId[1] = ParserX::GetUInt(data);
        trItemId.push_back(ParserX::GetUInt(data));
        trItemId.push_back(ParserX::GetUInt(data));
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SoundRegionObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if (!loaded) return;

    if(Game::viewInteractives && renderMode != gluu->RENDER_SHADOWMAP) 
        this->renderTritems(gluu, selectionColor);
};

void SoundRegionObj::renderTritems(GLUU* gluu, int selectionColor){
    
    ///////////////////////////////
    TDB* tdb = Game::trackDB;
    if(drawPositionB == NULL){
        if(this->trItemId.size() < 2){
            qDebug() << "SoundRegionObj: fail trItemId";
            loaded = false;
            return;
        }
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 0) {
            qDebug() << "SoundRegionObj: fail id";
            loaded = false;
            return;
        }
        float *srd = tdb->trackItems[this->trItemId[1]]->trItemSRData;
        if(srd != NULL)
            angle = srd[2];
        drawPositionB = new float[7];
        drawPositionE = new float[7];
        //qDebug() << this->trItemId[1];
        bool ok = tdb->getDrawPositionOnTrNode(drawPositionB, id, tdb->trackItems[this->trItemId[1]]->trItemSData1);
        if(!ok){
            this->loaded = false;
            return;
        }
        drawPositionB[0] += 2048 * (drawPositionB[5] - this->x);
        drawPositionB[2] -= 2048 * (-drawPositionB[6] - this->y);
        id = tdb->findTrItemNodeId(this->trItemId[this->trItemId.size()-1]);
        ok = tdb->getDrawPositionOnTrNode(drawPositionE, id, tdb->trackItems[this->trItemId[this->trItemId.size()-1]]->trItemSData1);
        if(!ok){
            this->loaded = false;
            return;
        }
        drawPositionE[0] += 2048 * (drawPositionE[5] - this->x);
        drawPositionE[2] -= 2048 * (-drawPositionE[6] - this->y);
        
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(2);
            pointer3d->setMaterial(1.0,1.0,0.0);
        }
        if(pointer3dSelected == NULL){
            pointer3dSelected = new TrackItemObj(2);
            pointer3dSelected->setMaterial(1.0,1.0,0.5);
        }
        // line
        drawLine = new OglObj();
        float* tpoints = new float[7];
        float* lpoints = new float[trItemId.size()*6];
        int ptr = 0;

        for(int i = 0; i < trItemId.size()/2; i++){
            if(this->trItemId[i*2] != 0)
                continue;
            int id = tdb->findTrItemNodeId(this->trItemId[i*2+1]);
            if (id < 0) {
                qDebug() << "SoundRegionObj: fail id";
                loaded = false;
                return;
            }
            //qDebug() << "id: "<< this->trItemId[i*2+1] << " "<< id;
            bool ok = tdb->getDrawPositionOnTrNode(tpoints, id, tdb->trackItems[this->trItemId[i*2+1]]->trItemSData1);
            if(!ok){
                this->loaded = false;
                return;
            }
            tpoints[0] += 2048 * (tpoints[5] - this->x);
            tpoints[2] -= 2048 * (-tpoints[6] - this->y);
            //lpoints[ptr++] = tpoints[0];
            //lpoints[ptr++] = tpoints[1]+1.0;
            //lpoints[ptr++] = -tpoints[2];
            //if((i > 0 && i < trItemId.size()/2-1) || trItemId.size() == 2){
            if(i == 0 || i == trItemId.size()/2 - 1){
                lpoints[ptr++] = tpoints[0];
                lpoints[ptr++] = tpoints[1]+1.0;
                lpoints[ptr++] = -tpoints[2];
            }
        }
        drawLine->init(lpoints, ptr, drawLine->V, GL_LINES);
        drawLine->setMaterial(1.0, 1.0, 0.0);
    }
    
    //int aaa = drawPosition[0];
    //if(pos == NULL) return;
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    int useSC;
    
    drawLine->render();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, -angle+M_PI);
    //Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[3]);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected) 
        pointer3dSelected->render(selectionColor + 1*131072*8*useSC);
    else
        pointer3d->render(selectionColor + 1*131072*8*useSC);
    gluu->mvPopMatrix();
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[0] + 0 * (drawPositionE[4] - this->x), drawPositionE[1] + 1, -drawPositionE[2] + 0 * (-drawPositionE[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, -angle+M_PI);
    //Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[3]);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected) 
        pointer3dSelected->render(selectionColor + 3*131072*8*useSC);
    else
        pointer3d->render(selectionColor + 3*131072*8*useSC);
    gluu->mvPopMatrix();
};

int SoundRegionObj::getDefaultDetailLevel(){
    return -5;
}

int SoundRegionObj::getSoundregionTrackType(){
    return this->soundregionTrackType;
}

void SoundRegionObj::save(QTextStream* out){
    if (!loaded) return;
    if (jestPQ < 2) return;
    
*(out) << "	Soundregion (\n";
    
*(out) << "		SoundregionRoty ( "<<this->soundregionRoty<<" )\n";
*(out) << "		SoundregionTrackType ( "<<this->soundregionTrackType<<" )\n";
for(int i = 0; i < this->trItemId.size(); i+=2){
*(out) << "		TrItemId ( "<<this->trItemId[i]<<" "<<this->trItemId[i+1]<<" )\n";
}
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
if(this->staticFlags != 0)
*(out) << "		StaticFlags ( "<<ParserX::MakeFlagsString(this->staticFlags)<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}