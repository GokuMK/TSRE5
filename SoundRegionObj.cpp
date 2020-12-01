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
#include "Vector4f.h"
#include "ErrorMessage.h"
#include "ErrorMessagesLib.h"

float SoundRegionObj::MaxPlacingDistance = 30;

SoundRegionObj::SoundRegionObj() {
}

SoundRegionObj::SoundRegionObj(const SoundRegionObj& o) : WorldObj(o) {
    soundregionRoty = o.soundregionRoty;
    soundregionTrackType = o.soundregionTrackType;
    trItemId.append(o.trItemId);
    selectionValue = o.selectionValue;
    angle = o.angle;
}

WorldObj* SoundRegionObj::clone(){
    return new SoundRegionObj(*this);
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

ErrorMessage* SoundRegionObj::checkForErrors(){
    TDB* tdb = Game::trackDB;

    TRitem *item = NULL;
    for(int i = 0; i < trItemId.size()/2; i++){
        if(this->trItemId[i*2] != 0)
            continue;
        
        item = Game::trackDB->trackItems[this->trItemId[i*2+1]];
        if(item == NULL) {
            ErrorMessage *e = new ErrorMessage(
                ErrorMessage::Type_Error, 
                ErrorMessage::Source_World, 
                QString("Object '") + type + "' - reference to trackItem not found. UiD: " + QString::number(UiD) + ". ",
                        "World Object requires Interactive Item that does not exist in Track DB. \n"
                );
            e->setLocationXYZ(x, -y, position[0], position[1], -position[2]);
            e->setObject((GameObj*)this);
            ErrorMessagesLib::PushErrorMessage(e);
            return e;
        }
        if(item->type != "soundregionitem"){
            ErrorMessage *e = new ErrorMessage(
                ErrorMessage::Type_Error, 
                ErrorMessage::Source_World, 
                QString("Object '") + type + "' - referenced trackItem has wrong type. UiD: " + QString::number(UiD) + ". ",
                        QString("World Object doesn't match TDB data. Is World Database and TDB out of sync? \n")+
                        "Expected: soundregionitem but found: "+item->type+" .\n"
                        "This may cause fatal errors."
                );
            e->setLocationXYZ(x, -y, position[0], position[1], -position[2]);
            e->setObject((GameObj*)this);
            ErrorMessagesLib::PushErrorMessage(e);
            return e;
        }
        
        if(soundregionTrackType != item->trItemSRData[1]){
            ErrorMessage *e = new ErrorMessage(
                ErrorMessage::Type_Error, 
                ErrorMessage::Source_World, 
                QString("Object '") + type + "' - referenced wrong SoundRegion Type. UiD: " + QString::number(UiD) + ". ",
                        QString("World Object doesn't match TDB data. Is World Database and TDB out of sync? \n") +
                        "Expected: " + QString::number(soundregionTrackType) + " but found: " + QString::number(item->trItemSRData[1]) + "\n" +
                        "This may cause fatal errors."
                );
            e->setLocationXYZ(x, -y, position[0], position[1], -position[2]);
            e->setObject((GameObj*)this);
            ErrorMessagesLib::PushErrorMessage(e);
            return e;
        }
    }
    
    return NULL;
}

void SoundRegionObj::deleteTrItems(){
    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    
    if(checkForErrors() != NULL){
        return;
    }
    
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
    trItemId.clear();
    TDB* tdb = Game::trackDB;
    qDebug() <<"new soundregion  "<<this->fileName;
    
    tdb->newSoundRegionObject(this->soundregionTrackType, trItemId, trNodeId, metry, this->typeID);
    float *srd = tdb->trackItems[this->trItemId[1]]->trItemSRData;
    this->soundregionRoty = srd[2];
    Quat::fill(this->qDirection);
    Quat::rotateY(this->qDirection, this->qDirection, -this->soundregionRoty);
    this->position[1] += 1;
    this->drawPositions.clear();
}

void SoundRegionObj::flip(bool flipShape){

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
    this->drawPositions.clear();
}

void SoundRegionObj::expandTrItems(){
    TDB* tdb = Game::trackDB;
    qDebug() <<"ex sound region  "<<this->fileName;

    float* playerT = Vec2::fromValues(this->x, this->y);
    float pos[3];
    Vec3::set(pos, position[0], position[1], position[2]);
    float quat[4];
    float* buffer = new float[12];
    float vec1[3];
    float vec2[3];
    Vec3::set(vec1, -MaxPlacingDistance, 0, 0);
    Vec3::set(vec2, MaxPlacingDistance, 0, 0);
    Quat::fill(quat);
    Quat::rotateY(quat, quat, -soundregionRoty);
    Vec3::transformQuat(vec1, vec1, quat);
    Vec3::transformQuat(vec2, vec2, quat);
    
    buffer[0] = position[0]+vec1[0];
    buffer[1] = position[1];
    buffer[2] = position[2]+vec1[2];
    buffer[6] = position[0]+vec2[0];
    buffer[7] = position[1];
    buffer[8] = position[2]+vec2[2]; 
    int len = 12;
    qDebug() << "and find intersections ";
    QVector<TDB::IntersectionPoint> ipoints;
    tdb->getSegmentIntersectionPositionOnTDB(ipoints, NULL, playerT, buffer, len, (float*)&pos);
    qDebug() << "intersection count: "<<ipoints.size();
    
    int tid = tdb->findTrItemNodeId(this->trItemId[1]);
    for(int i = 0; i < ipoints.size(); i++){
        qDebug() << ipoints[i].distance;
        if(ipoints[i].distance < 0.5)
            continue;
        if(ipoints[i].idx == tid)
            continue;
        
        int trNodeId = ipoints[i].idx;
        float metry = ipoints[i].m;

        qDebug() <<"new soundregion  "<<this->fileName;

        tdb->newSoundRegionObject(this->soundregionTrackType, trItemId, trNodeId, metry, this->typeID);
        tdb->trackItems[this->trItemId.last()]->trItemSRData[2] = this->soundregionRoty;
    }
    drawPositions.clear();
    modified = true;
}

void SoundRegionObj::deleteSelectedTrItem(){
    if(selectionValue < 1)
        return;
    if(selectionValue > trItemId.size()/2)
        return;
    if(trItemId.size() < 3)
        return;

    TDB* tdb = Game::trackDB;
    
    int i = selectionValue - 1;
    qDebug() << "remove tritem " << i;
    tdb->deleteTrItem(this->trItemId[i*2+1]);
    trItemId.remove(i*2+1);
    trItemId.remove(i*2+0);
    
    drawPositions.clear();
    modified = true;
}

bool SoundRegionObj::containsTrackItem(int tdbId, int id){
    for(int i = 0; i<this->trItemId.size()/2; i++){
        if(this->trItemId[i*2] == tdbId){
            if(this->trItemId[i*2+1] == id)
                return true;
        }
    }
    return false;
}

void SoundRegionObj::addTrackItemIdOffset(unsigned int trackOffset, unsigned int roadOffset){
    for(int i = 0; i<this->trItemId.size()/2; i++){
        if(this->trItemId[i*2] == 0)
            this->trItemId[i*2+1] += trackOffset;
        if(this->trItemId[i*2] == 1)
            this->trItemId[i*2+1] += roadOffset;
    }
}

void SoundRegionObj::getTrackItemIds(QVector<int> &ids, int tdbId){
    if(!this->loaded)
        return;
    for(int i = 0; i<this->trItemId.size()/2; i++){
        if(this->trItemId[i*2] == tdbId){
            ids.push_back(this->trItemId[i*2+1]);
        }
    }
}

bool SoundRegionObj::select(int value){
    this->selectionValue = value;
    this->selected = true;
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
    
    if(drawPositions.size() == 0){
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
        
        QVector<Vector4f> ipoints;
        float* tpoints = new float[7];
        int ptr = 0;
        int minlidx = -1;
        float minlval = 9999;
        
        for(int i = 0; i < trItemId.size()/2; i++){
            if(this->trItemId[i*2] != 0)
                continue;
            int id = tdb->findTrItemNodeId(this->trItemId[i*2+1]);
            if (id < 0) {
                qDebug() << "SoundRegionObj: fail id";
                loaded = false;
                return;
            }
            
            drawPosition = new float[8];
            bool ok = tdb->getDrawPositionOnTrNode(drawPosition, id, tdb->trackItems[this->trItemId[i*2+1]]->getTrackPosition());
            if(!ok){
                qDebug() << "SoundRegionObj: fail drawPosition";
                this->loaded = false;
                return;
            }
            memcpy(tpoints, drawPosition, sizeof(float)*7 );
            drawPosition[0] += 2048 * (drawPosition[5] - this->x);
            drawPosition[2] -= 2048 * (-drawPosition[6] - this->y);
            float *srd = tdb->trackItems[this->trItemId[i*2+1]]->trItemSRData;
            if(srd != NULL)
                drawPosition[7] = srd[2];
            drawPositions.push_back(drawPosition);
            
            tpoints[0] += 2048 * (tpoints[5] - this->x);
            tpoints[2] -= 2048 * (-tpoints[6] - this->y);
            ipoints.push_back(Vector4f(tpoints[0], tpoints[1]+1.0, -tpoints[2], 0));
            if(tpoints[0] < minlval){
                minlval = tpoints[0];
                minlidx = ipoints.size()-1;
            }
        }

        if(ipoints.size() > 1){
            for(int i = 0; i < ipoints.size(); i++){
                ipoints[i].c = Vec3::dist((float*)&ipoints[minlidx], (float*)&ipoints[i]);
            }
            std::sort(ipoints.begin(), ipoints.end(), Vector4f::SortByC);
            float* lpoints = new float[ipoints.size()*6];
            for(int i = 0; i < ipoints.size()-1; i++){
                lpoints[ptr++] = ipoints[i].x;
                lpoints[ptr++] = ipoints[i].y;
                lpoints[ptr++] = ipoints[i].z;
                lpoints[ptr++] = ipoints[i+1].x;
                lpoints[ptr++] = ipoints[i+1].y;
                lpoints[ptr++] = ipoints[i+1].z;
            }
            drawLine->init(lpoints, ptr, RenderItem::V, GL_LINES);
            drawLine->setMaterial(1.0, 1.0, 0.0);
        }
    }

    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    if(selectionColor == 0)
        drawLine->render();
    int useSC;

    for(int i = 0; i < drawPositions.size(); i++){
        drawPosition = drawPositions[i];
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 0 * (drawPosition[4] - this->x), drawPosition[1] + 1, -drawPosition[2] + 0 * (-drawPosition[5] - this->y));
        Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, -drawPosition[7]+M_PI);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        useSC = (float)selectionColor/(float)(selectionColor+0.000001);
        if(this->selected && this->selectionValue == i+1) 
            pointer3dSelected->render(selectionColor | (i+1)*useSC);
        else
            pointer3d->render(selectionColor | (i+1)*useSC);
        gluu->mvPopMatrix();
    }

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
    if(Game::useOnlyPositiveQuaternions)
        Quat::makePositive(this->qDirection);
    
*(out) << "	Soundregion (\n";
    
*(out) << "		SoundregionRoty ( "<<this->soundregionRoty<<" )\n";
*(out) << "		SoundregionTrackType ( "<<this->soundregionTrackType<<" )\n";
for(int i = 0; i < this->trItemId.size(); i+=2){
*(out) << "		TrItemId ( "<<this->trItemId[i]<<" "<<this->trItemId[i+1]<<" )\n";
}
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		FileName ( "<<ParserX::AddComIfReq(this->fileName)<<" )\n";
if(this->staticFlags != 0)
*(out) << "		StaticFlags ( "<<ParserX::MakeFlagsString(this->staticFlags)<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}