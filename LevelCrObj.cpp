/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "LevelCrObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include "TDB.h"
#include "TRitem.h"
#include "Game.h"
#include "TrackItemObj.h"
#include "TS.h"
#include <QDebug>
#include "ErrorMessagesLib.h"
#include "ErrorMessage.h"

float LevelCrObj::MaxPlacingDistance = 30;

LevelCrObj::LevelCrObj() {
    this->shape = -1;
    this->loaded = false;
    this->levelCrParameters[0] = 30; 
    this->levelCrParameters[1] = 20; 
    this->crashProbability = 0;
    this->levelCrData[0] = 0;
    this->levelCrTiming[0] = 60;
    this->levelCrTiming[1] = 60;
    this->levelCrTiming[2] = 4;
}

LevelCrObj::LevelCrObj(const LevelCrObj& o) : WorldObj(o) {
    selectionValue = o.selectionValue;
    levelCrParameters[0] = o.levelCrParameters[0];
    levelCrParameters[1] = o.levelCrParameters[2];
    crashProbability = o.crashProbability;
    levelCrData[0] = o.levelCrData[0];
    levelCrData[1] = o.levelCrData[1];
    levelCrTiming[0] = o.levelCrTiming[0];
    levelCrTiming[1] = o.levelCrTiming[1];
    levelCrTiming[2] = o.levelCrTiming[2];
    trItemId.append(o.trItemId);
    trItemIdCount = o.trItemIdCount;
}

WorldObj* LevelCrObj::clone(){
    return new LevelCrObj(*this);
}

LevelCrObj::~LevelCrObj() {
}

bool LevelCrObj::allowNew(){
    return true;
}

bool LevelCrObj::isTrackItem(){
    return true;
}

void LevelCrObj::loadingFixes(){
    if(Game::useOnlyPositiveQuaternions){
        if(qDirection[3] < 0){
            Quat::makePositive(qDirection);
            ErrorMessage *e = new ErrorMessage(
                    ErrorMessage::Type_Info, 
                    ErrorMessage::Source_Editor, 
                    QString("Fixed negative quaternion in tile ") + QString::number(x) + " " + QString::number(y) + " : " + QString::number(typeID) 
                    );
            ErrorMessagesLib::PushErrorMessage(e);
            modified = true;
        }            
    }
}

void LevelCrObj::load(int x, int y) {
    this->shape = Game::currentShapeLib->addShape(resPath +"/"+ fileName);
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 1;
    this->modified = false;
    setMartix();
}

ErrorMessage* LevelCrObj::checkForErrors(){
    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    TRitem *item = NULL;
    
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == 0)
            item = tdb->trackItems[this->trItemId[i*2+1]];  
        else if(this->trItemId[i*2] == 1)
            item = rdb->trackItems[this->trItemId[i*2+1]];
        
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
        if(item->type != "levelcritem"){
            ErrorMessage *e = new ErrorMessage(
                ErrorMessage::Type_Error, 
                ErrorMessage::Source_World, 
                QString("Object '") + type + "' - referenced trackItem has wrong type. UiD: " + QString::number(UiD) + ". ",
                        QString("World Object doesn't match TDB data. Is World Database and TDB out of sync? \n")+
                        "Expected: levelcritem but found: "+item->type+" .\n"
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

void LevelCrObj::deleteTrItems(){
    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    
    if(checkForErrors() != NULL){
        return;
    }
    
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == 0)
            tdb->deleteTrItem(this->trItemId[i*2+1]);
        else if(this->trItemId[i*2] == 1)
            rdb->deleteTrItem(this->trItemId[i*2+1]);
        this->trItemId[i*2+1] = -1;
    }
}

void LevelCrObj::initTrItems(float* tpos){
    if(tpos == NULL)
        return;
    
    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    qDebug() <<"new levelcr  "<<this->fileName;

    int* tid;
    float* playerT = Vec2::fromValues(this->x, this->y);
    float pos[3];
    Vec3::set(pos, position[0], position[1], -position[2]);
    float quat[4];
    float* buffer;
    int len;
    qDebug() << "find nearest road ";
    rdb->findNearestPositionOnTDB(playerT, (float*)&pos, (float*)&quat, tpos);
    qDebug() << "road pos "<<tpos[0]<<" "<<tpos[1];
    rdb->getVectorSectionLine(buffer, len, playerT[0], playerT[1], tpos[0]);
    qDebug() << "and find intersections ";
    QVector<TDB::IntersectionPoint> ipoints;
    tdb->getSegmentIntersectionPositionOnTDB(ipoints, rdb, playerT, buffer, len, (float*)&pos);
    qDebug() << "intersection count: "<<ipoints.size();
    
    int pointCount;
    for(pointCount = 0; pointCount < ipoints.size(); ){
        if(ipoints[pointCount].distance > MaxPlacingDistance)
            break;
        else
            pointCount++;
    }

    trItemIdCount = pointCount*4;
    levelCrData[1] = pointCount;
    trItemId.fill(-1,levelCrData[1]*4);
    for(int i = 0; i < pointCount; i++){
        //qDebug() << ipoints[i].distance;
        tdb->newLevelCrObject(tid, ipoints[i].idx, ipoints[i].m, this->typeID);
        trItemId[i*2+0] = 0;
        trItemId[i*2+1] = tid[1];
        rdb->newLevelCrObject(tid, ipoints[i].sidx, ipoints[i].sm, this->typeID);
        trItemId[pointCount*2+i*2+0] = 1;
        trItemId[pointCount*2+i*2+1] = tid[1];
    }

    this->drawPosition = NULL;
}

bool LevelCrObj::containsTrackItem(int tdbId, int id){
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == tdbId){
            if(this->trItemId[i*2+1] == id)
                return true;
        }
    }
    return false;
}

void LevelCrObj::getTrackItemIds(QVector<int> &ids, int tdbId){
    if(!this->loaded)
        return;
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == tdbId){
            ids.push_back(this->trItemId[i*2+1]);
        }
    }
}

void LevelCrObj::set(QString sh, QString val){
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

void LevelCrObj::set(int sh, FileBuffer* data) {
    if (sh == TS::LevelCrParameters) {
        data->off++;
        levelCrParameters[0] = data->getFloat();
        levelCrParameters[1] = data->getFloat();
        return;
    }
    if (sh == TS::CrashProbability) {
        data->off++;
        crashProbability = data->getFloat();
        return;
    }
    if (sh == TS::LevelCrData) {
        data->off++;
        levelCrData[0] = data->getUint();
        levelCrData[1] = data->getUint();
        trItemId.fill(-1,levelCrData[1]*4);
        return;
    }
    if (sh == TS::LevelCrTiming) {
        data->off++;
        levelCrTiming[0] = data->getFloat();
        levelCrTiming[1] = data->getFloat();
        levelCrTiming[2] = data->getFloat();
        return;
    }
    if (sh == TS::TrItemId) {
        data->off++;
        trItemId[trItemIdCount++] = data->getUint();
        trItemId[trItemIdCount++] = data->getUint();
        return;
    }
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

void LevelCrObj::set(QString sh, FileBuffer* data) {
    if (sh == ("levelcrparameters")) {
        levelCrParameters[0] = ParserX::GetNumber(data);
        levelCrParameters[1] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("crashprobability")) {
        crashProbability = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("levelcrdata")) {
        levelCrData[0] = ParserX::GetHex(data);
        levelCrData[1] = ParserX::GetNumber(data);
        trItemId.fill(-1,levelCrData[1]*4);
        return;
    }
    if (sh == ("levelcrtiming")) {
        levelCrTiming[0] = ParserX::GetNumber(data);
        levelCrTiming[1] = ParserX::GetNumber(data);
        levelCrTiming[2] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        //qDebug() << trItemId.size();
        //qDebug() <<"levelcr "<<trItemId[trItemIdCount-2] <<" "<<trItemId[trItemIdCount-1];
        return;
    }
    if (sh == ("filename")) {
        fileName = ParserX::GetString(data);
        return;
    }
    if (sh == ("ortssoundfilename")) {
        ORTSSoundFileName = ParserX::GetString(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void LevelCrObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if (!loaded) return;
    if (shape < 0) return;
    if (jestPQ < 2) return;
    //GLUU* gluu = GLUU::get();
    //if((this.position===undefined)||this.qDirection===undefined) return;
    //
    if (size > 0) {
        if ((lod > size + 150)) {
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
            if ((ccos > 0) && (xxx > size) && (skipLevel == 1)) return;
        }
    } else {
        if (Game::currentShapeLib->shape[shape]->loaded)
            size = Game::currentShapeLib->shape[shape]->size;
    }
    
    gluu->mvPushMatrix();
    Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    if(selectionColor != 0){
        gluu->disableTextures(selectionColor);
    } else {
        gluu->enableTextures();
    }
        
    Game::currentShapeLib->shape[shape]->render(selectionColor, 0);
    
    if(selected){
        drawBox();
    }
    gluu->mvPopMatrix();
    
    if(Game::viewInteractives && renderMode != gluu->RENDER_SHADOWMAP) 
        this->renderTritems(gluu, selectionColor);
};

void LevelCrObj::renderTritems(GLUU* gluu, int selectionColor){

    ///////////////////////////////
    TDB* tdb = Game::trackDB;
    if(drawPositions.size() == 0){
        if(this->trItemId.capacity() == 0 || trItemIdCount == 0){
            qDebug() << "LevelCrObj: fail trItemId";
            loaded = false;
            return;
        }
        for(int i = 0; i < trItemIdCount/2; i++){
            if(this->trItemId[i*2] != 0)
                continue;

            int id = tdb->findTrItemNodeId(this->trItemId[i*2+1]);
            if (id < 0) {
                qDebug() << "LevelCrObj: fail id";
                loaded = false;
                return;
            }
            //qDebug() << "id: "<< this->trItemId[i*2+1] << " "<< id;
            drawPosition = new float[7];
            bool ok = tdb->getDrawPositionOnTrNode(drawPosition, id, tdb->trackItems[this->trItemId[i*2+1]]->getTrackPosition());
            if(!ok){
                this->loaded = false;
                return;
            }
            drawPosition[0] += 2048 * (drawPosition[5] - this->x);
            drawPosition[2] -= 2048 * (-drawPosition[6] - this->y);
            if(pointer3d == NULL){
                pointer3d = new TrackItemObj(1);
                pointer3d->setMaterial(0.9,0.5,0.0);
            }
            if(pointer3dSelected == NULL){
                pointer3dSelected = new TrackItemObj(1);
                pointer3dSelected->setMaterial(1.0,0.8,0.3);
            }
            drawPositions.push_back(drawPosition);
        }
    }

    //if(pos == NULL) return;
    int useSC;

    for(int i = 0; i < drawPositions.size(); i++){
        drawPosition = drawPositions[i];
        gluu->mvPushMatrix();
        Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 0 * (drawPosition[4] - this->x), drawPosition[1] + 1, -drawPosition[2] + 0 * (-drawPosition[5] - this->y));
        Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3]);
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
        //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));

        useSC = (float)selectionColor/(float)(selectionColor+0.000001);
        if(this->selected && this->selectionValue == i+1) 
            pointer3dSelected->render(selectionColor | (i+1)*useSC);
        else
            pointer3d->render(selectionColor | (i+1)*useSC);
        gluu->mvPopMatrix();
    }

};

bool LevelCrObj::select(int value){
    this->selectionValue = value;
    this->selected = true;
}

void LevelCrObj::deleteSelectedTrItem(){
    if(selectionValue < 1)
        return;
    if(selectionValue > trItemIdCount/4)
        return;
    if(trItemId.size() < 5)
        return;

    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    
    int i = selectionValue - 1;
    qDebug() << "remove tritem " << i;
    qDebug() << trItemIdCount;
    qDebug() << this->trItemId[i*2] << this->trItemId[trItemIdCount/2+i*2];
    if(this->trItemId[i*2] == 0 && this->trItemId[trItemIdCount/2+i*2] == 1){
        
        tdb->deleteTrItem(this->trItemId[i*2+1]);
        rdb->deleteTrItem(this->trItemId[trItemIdCount/2+i*2+1]);
        trItemId.remove(trItemIdCount/2+i*2+1);
        trItemId.remove(trItemIdCount/2+i*2+0);
        trItemId.remove(i*2+1);
        trItemId.remove(i*2+0);
        trItemIdCount-=4;
        this->levelCrData[1]-=1;
        drawPositions.clear();
        //qDebug() << drawPositions.size();
        this->modified = true;
    }
}

bool LevelCrObj::getSimpleBorder(float* border){
    if (shape < 0) return false;
    if (!Game::currentShapeLib->shape[shape]->loaded)
        return false;
    float* bound = Game::currentShapeLib->shape[shape]->bound;
    border[0] = bound[0];
    border[1] = bound[1];
    border[2] = bound[2];
    border[3] = bound[3];
    border[4] = bound[4];
    border[5] = bound[5];
    return true;
}

bool LevelCrObj::getBoxPoints(QVector<float>& points){
        float bound[6];
        if (!getSimpleBorder((float*)&bound)) return false;
        
        for(int i=0; i<2; i++)
            for(int j=4; j<6; j++){
                points.push_back(bound[i]);
                points.push_back(bound[2]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
                points.push_back(bound[3]);
                points.push_back(bound[j]);
            }
        for(int i=0; i<2; i++)
            for(int j=2; j<4; j++){
                points.push_back(bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[4]);
                points.push_back(bound[i]);
                points.push_back(bound[j]);
                points.push_back(bound[5]);
            }
        for(int i=4; i<6; i++)
            for(int j=2; j<4; j++){
                points.push_back(bound[0]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
                points.push_back(bound[1]);
                points.push_back(bound[j]);
                points.push_back(bound[i]);
            }
        return true;
}

int LevelCrObj::getDefaultDetailLevel(){
    return -9;
}

void LevelCrObj::setSensitivityActivateLevel(float val){
    this->levelCrParameters[0] = val;
    this->modified = true;
}

void LevelCrObj::setSensitivityMinimunDistance(float val){
    this->levelCrParameters[1] = val;
    this->modified = true;
}

void LevelCrObj::setTimingInitialWarning(float val){
    this->levelCrTiming[0] = val;
    this->modified = true;
}

void LevelCrObj::setTimingSeriousWarning(float val){
    this->levelCrTiming[1] = val;
    this->modified = true;
}

void LevelCrObj::setTimingAnimationLength(float val){
    this->levelCrTiming[2] = val;
    this->modified = true;
}

void LevelCrObj::setCrashProbability(float val){
    this->crashProbability = val;
    this->modified = true;
}

void LevelCrObj::setInvisible(bool val){
    if(val)
        this->levelCrData[0] = this->levelCrData[0] | 1;
    else
        this->levelCrData[0] = this->levelCrData[0] & ~(1);
    this->modified = true;
}

void LevelCrObj::setSilentMstsHax(bool val){
    if(val)
        this->levelCrData[0] = this->levelCrData[0] | 6;
    else
        this->levelCrData[0] = this->levelCrData[0] & ~(6);
    this->modified = true;
}

void LevelCrObj::setSoundFileName(QString val){
    ORTSSoundFileName = val;
    modified = true;
}

QString LevelCrObj::getSoundFileName(){
    return ORTSSoundFileName;
}

float LevelCrObj::getSensitivityActivateLevel(){
    return this->levelCrParameters[0];
}

float LevelCrObj::getSensitivityMinimunDistance(){
    return this->levelCrParameters[1];
}

float LevelCrObj::getTimingInitialWarning(){
    return this->levelCrTiming[0];
}

float LevelCrObj::getTimingSeriousWarning(){
    return this->levelCrTiming[1];
}

float LevelCrObj::getTimingAnimationLength(){
    return this->levelCrTiming[2];
}

float LevelCrObj::getCrashProbability(){
    return this->crashProbability;
}

bool LevelCrObj::isInvisibleEnabled(){
    return ((this->levelCrData[0]) & 1) == 1;
}

bool LevelCrObj::isSilentMstsHaxEnabled(){
    return ((this->levelCrData[0]) & 6) == 6;
}

void LevelCrObj::translate(float px, float py, float pz){
    if(this->selectionValue == 0){
        return WorldObj::translate(px, py, pz);
    }
    
    if(pz == 0) 
        return;
    TDB* tdb = Game::trackDB;
    
    int trid = this->selectionValue - 1;
        
    int id = tdb->findTrItemNodeId(this->trItemId[trid*2+1]);
    if (id < 0) {
        qDebug() << "fail id";
        return;
    }
    
    float dlugosc = tdb->getVectorSectionLength(id);
    TRitem* trit = tdb->trackItems[this->trItemId[trid*2+1]];
    if(trit == NULL) 
        return;
    if(pz < 0){
        trit->trackPositionAdd(-0.5);
        if(trit->getTrackPosition() < 0)
            trit->setTrackPosition(0);
        tdb->updateTrItemRData(trit);
    } else if(pz > 0){
        trit->trackPositionAdd(0.5);
        if(trit->getTrackPosition() > dlugosc)
            trit->setTrackPosition(dlugosc);
        tdb->updateTrItemRData(trit);
    }
    drawPositions.clear();
    this->modified = true;
    setMartix();
}

void LevelCrObj::save(QTextStream* out){
    if (!loaded) return;
    if(Game::useOnlyPositiveQuaternions)
        Quat::makePositive(this->qDirection);
    
*(out) << "	LevelCr (\n";
    
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		LevelCrParameters ( "<<this->levelCrParameters[0]<<" "<<this->levelCrParameters[1]<<" )\n";
*(out) << "		CrashProbability ( "<<this->crashProbability<<" )\n";
*(out) << "		LevelCrData ( "<< ParserX::MakeFlagsString(this->levelCrData[0]) <<" "<<this->levelCrData[1]<<" )\n";
*(out) << "		LevelCrTiming ( "<<this->levelCrTiming[0]<<" "<<this->levelCrTiming[1]<<" "<<this->levelCrTiming[2]<<" )\n";
for(int i = 0; i < trItemIdCount; i+=2){
*(out) << "		TrItemId ( "<<this->trItemId[i]<<" "<<this->trItemId[i+1]<<" )\n";
}
*(out) << "		FileName ( "<<ParserX::AddComIfReq(this->fileName)<<" )\n";
if(ORTSSoundFileName.length() > 0)
    *(out) << "		ORTSSoundFileName ( "<<ParserX::AddComIfReq(this->ORTSSoundFileName)<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
if(this->staticDetailLevel > -1)
*(out) << "		StaticDetailLevel ( "<<this->staticDetailLevel<<" )\n";
*(out) << "	)\n";
}