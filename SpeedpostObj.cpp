/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SpeedpostObj.h"
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
#include "SpeedPostDAT.h"
#include "SpeedPost.h"
#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

bool SpeedpostObj::allowNew(){
    return true;
}

bool SpeedpostObj::isTrackItem(){
    return true;
}

SpeedpostObj::SpeedpostObj() {
    this->shape = -1;
    this->loaded = false;
}

SpeedpostObj::SpeedpostObj(const SpeedpostObj& orig) {
}

SpeedpostObj::~SpeedpostObj() {
}

void SpeedpostObj::load(int x, int y) {
    this->shape = Game::currentShapeLib->addShape(resPath +"/"+ fileName);
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->size = -1;
    this->skipLevel = 1;
    this->modified = false;
    this->drawPosition = NULL;
    this->pointer3d = NULL;
    setMartix();
}

void SpeedpostObj::deleteTrItems(){
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

QString SpeedpostObj::getSpeedpostType(){
    if(this->trItemId.size() < 2){
        qDebug() << "speedpost: fail trItemId";
        return "";
    }
    TRitem* item = Game::trackDB->trackItems[this->trItemId[1]];
    if(item == NULL) return "";
    TRitem::SType val = item->getSpeedpostType();
    if((val) == TRitem::MILEPOST)
        return QString("milepost");
    if((val) == TRitem::WARNING)
        return QString("warning");
    if((val) == TRitem::SIGN)
        return QString("speedsign");
    if((val) == TRitem::RESUME)
        return QString("resume");
    return "";
}

float SpeedpostObj::getSpeed(){
    if(this->trItemId.size() < 2){
        qDebug() << "speedpost: fail trItemId";
        return 0;
    }
    TRitem* item = Game::trackDB->trackItems[this->trItemId[1]];
    if(item == NULL) return 0;
    return item->speedpostTrItemData[1];
}

float SpeedpostObj::getNumber(){
    if(this->trItemId.size() < 2){
        qDebug() << "speedpost: fail trItemId";
        return 0;
    }
    TRitem* item = Game::trackDB->trackItems[this->trItemId[1]];
    if(item == NULL) return 0;
    return item->speedpostTrItemData[1];
}

void SpeedpostObj::setSpeed(float val){
    if(this->trItemId.size() < 2){
        qDebug() << "speedpost: fail trItemId";
        return;
    }
    TDB* tdb = Game::trackDB;
    for(int j = 0; j < this->trItemId.size()/2; j++){
        if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
            continue;
        tdb->trackItems[this->trItemId[j*2+1]]->setSpeedpostSpeed(val);
    }
}

void SpeedpostObj::setNumber(float val){
    if(this->trItemId.size() < 2){
        qDebug() << "speedpost: fail trItemId";
        return;
    }
    TDB* tdb = Game::trackDB;
    for(int j = 0; j < this->trItemId.size()/2; j++){
        if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
            continue;
        tdb->trackItems[this->trItemId[j*2+1]]->setSpeedpostNum(val);
    }
}

void SpeedpostObj::flip(bool flipShape){
    if(flipShape){
        Quat::rotateY(this->qDirection, this->qDirection, M_PI);
        this->setMartix();
    }

    TDB* tdb = Game::trackDB;
    for(int j = 0; j < this->trItemId.size()/2; j++){
        if(tdb->trackItems[this->trItemId[j*2+1]] == NULL)
            continue;
        tdb->trackItems[this->trItemId[j*2+1]]->flipSpeedpost();
    }
    this->modified = true; 
    delete[] drawPositions;
    drawPositions = NULL;
}

void SpeedpostObj::initTrItems(float* tpos){
    if(tpos == NULL)
        return;
    int trNodeId = tpos[0];
    float metry = tpos[1];
    
    TDB* tdb = Game::trackDB;
    qDebug() <<"new speedpost  "<<this->fileName;

    tdb->newSpeedPostObject(speedPostId, speedPostType, trItemId, trNodeId, metry, this->typeID);
    
    //this->signalSubObj = 0;
    //qDebug() <<"signalUnits  "<<this->signalUnits;
    //for(int i = 0; i < this->signalUnits; i++)
    //    this->signalSubObj = this->signalSubObj | (1 << i);
   // this->trItemIdCount = 4;
  //  this->trItemId[0] = isRoad;
   // this->trItemId[1] = trItemId[0];
   // this->trItemId[2] = isRoad;
  //  this->trItemId[3] = trItemId[1];
    this->drawPositions = NULL;
}

void SpeedpostObj::set(QString sh, int val){
    if (sh == ("ref_value")) {
        speedPostId = val/1000;
        speedPostType = val - speedPostId*1000;
        qDebug() << "speedPostId "<<speedPostId<< " speedPostType " << speedPostType;
        SpeedPost *speedPost = Game::trackDB->speedPostDAT->speedPost[speedPostId];
        
        if(speedPostType == 0){
            fileName = speedPost->speedSignShapeName;
            speedSignShape = new float[speedPost->speedSignShapeCount*4+1];
            speedSignShape[0] = speedPost->speedSignShapeCount;
            for(int i = 0; i<speedSignShape[0]*4; i++)
                speedSignShape[i+1] = speedPost->speedSignShape[i];
        }else if(speedPostType == 1){
            fileName = speedPost->speedResumeSignShapeName;
            speedSignShape = new float[speedPost->speedResumeSignShapeCount*4+1];
            speedSignShape[0] = speedPost->speedResumeSignShapeCount;
            for(int i = 0; i<speedSignShape[0]*4; i++)
                speedSignShape[i+1] = speedPost->speedResumeSignShape[i];
        }else if(speedPostType == 2){
            fileName = speedPost->speedWarningSignShapeName;
            speedSignShape = new float[speedPost->speedWarningSignShapeCount*4+1];
            speedSignShape[0] = speedPost->speedWarningSignShapeCount;
            for(int i = 0; i<speedSignShape[0]*4; i++)
                speedSignShape[i+1] = speedPost->speedWarningSignShape[i];
        }else if(speedPostType == 3){
            fileName = speedPost->milepostShapeName;
            speedSignShape = new float[speedPost->milepostShapeCount*4+1];
            speedSignShape[0] = speedPost->milepostShapeCount;
            for(int i = 0; i<speedSignShape[0]*4; i++)
                speedSignShape[i+1] = speedPost->milepostShape[i];
        }
        
        speedDigitTex = speedPost->speedDigitTex;
        speedTextSize[0] = speedPost->speedTextSize[0];
        speedTextSize[1] = speedPost->speedTextSize[1];
        speedTextSize[2] = speedPost->speedTextSize[2];
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void SpeedpostObj::set(QString sh, QString val){
    if (sh == ("filename")) {
        fileName = val;
        return;
    }
    if (sh == ("ref_filename")) {
        fileName = val;
        return;
    }
    /*if (sh == ("ref_class")) {
        if(val == "speedsign")
            this->sType = SIGN;
        else if(val == "speedresume")
            this->sType = RESUME;
        else if(val == "speedwarning")
            this->sType = WARNING;
        else if(val == "milepost")
            this->sType = MILEPOST;
        else
            this->sType = UNDEFINED;
        return;
    }*/
    WorldObj::set(sh, val);
    return;
}

void SpeedpostObj::set(int sh, FileBuffer* data) {
    if (sh == TS::FileName) {
        data->off++;
        int slen = data->getShort()*2;
        fileName = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    if (sh == TS::Speed_Digit_Tex) {
        data->off++;
        int slen = data->getShort()*2;
        speedDigitTex = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    if (sh == TS::Speed_Sign_Shape) {
        data->off++;
        int len = data->getUint();
        speedSignShape = new float[len*4+1];
        speedSignShape[0] = len;
        for(int i = 0; i<speedSignShape[0]*4; i++)
            speedSignShape[i+1] = data->getFloat();
        return;
    }
    if (sh == TS::Speed_Text_Size) {
        data->off++;
        speedTextSize[0] = data->getFloat();
        speedTextSize[1] = data->getFloat();
        speedTextSize[2] = data->getFloat();
        return;
    }
    if (sh == TS::TrItemId) {
        data->off++;
        //this->trItemId = new int[2];
        //trItemId[0] = data->getUint();
        //trItemId[1] = data->getUint();
        trItemId.push_back(data->getUint());
        trItemId.push_back(data->getUint());
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SpeedpostObj::set(QString sh, FileBuffer* data) {
    if (sh == ("filename")) {
        fileName = ParserX::GetString(data);
        return;
    }
    if (sh == ("speed_digit_tex")) {
        speedDigitTex = ParserX::GetString(data);
        return;
    }
    if (sh == ("speed_sign_shape")) {
        int len = ParserX::GetNumber(data);
        speedSignShape = new float[len*4+1];
        speedSignShape[0] = len;
        for(int i = 0; i<speedSignShape[0]*4; i++)
            speedSignShape[i+1] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("speed_text_size")) {
        speedTextSize[0] = ParserX::GetNumber(data);
        speedTextSize[1] = ParserX::GetNumber(data);
        speedTextSize[2] = ParserX::GetNumber(data);
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

void SpeedpostObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
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
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
    } else {
        gluu->enableTextures();
    }
        
    Game::currentShapeLib->shape[shape]->render();
    
    if(selected){
        drawBox();
    }
    gluu->mvPopMatrix();
    
    if(Game::viewInteractives) 
        this->renderTritems(gluu, selectionColor);
};

void SpeedpostObj::renderTritems(GLUU* gluu, int selectionColor){
    
    ///////////////////////////////
    TDB* tdb = Game::trackDB;
    if(drawPosition == NULL){
        if(this->trItemId.size() < 2){
            qDebug() << "speedpost: fail trItemId";
            loaded = false;
            return;
        }
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 0) {
            qDebug() << "speedpost: fail id";
            loaded = false;
            return;
        }
                //qDebug() << "id: "<< this->trItemId[i*2+1] << " "<< id;
        drawPosition = new float[7];
        //qDebug() << this->trItemId[1];
        bool ok = tdb->getDrawPositionOnTrNode(drawPosition, id, tdb->trackItems[this->trItemId[1]]->trItemSData1);
        if(!ok){
            this->loaded = false;
            return;
        }
        drawPosition[0] += 2048 * (drawPosition[5] - this->x);
        drawPosition[2] -= 2048 * (-drawPosition[6] - this->y);
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.7,0.7,0.7);
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
                qDebug() << "speedpost: fail id";
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
            lpoints[ptr++] = tpoints[0];
            lpoints[ptr++] = tpoints[1]+1.0;
            lpoints[ptr++] = -tpoints[2];
            if((i > 0 && i < trItemId.size()/2-1) || trItemId.size() == 2){
                lpoints[ptr++] = tpoints[0];
                lpoints[ptr++] = tpoints[1]+1.0;
                lpoints[ptr++] = -tpoints[2];
            }
        }
        drawLine->init(lpoints, ptr, drawLine->V, GL_LINES);
        drawLine->setMaterial(1.0, 0.0, 0.0);
    }
    
    //int aaa = drawPosition[0];
    //if(pos == NULL) return;
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();
    int useSC;

    gluu->mvPushMatrix();
    drawLine->render();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 0 * (drawPosition[4] - this->x), drawPosition[1] + 1, -drawPosition[2] + 0 * (-drawPosition[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3]);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    pointer3d->render(selectionColor + (1)*131072*8*useSC);
    gluu->mvPopMatrix();
};

bool SpeedpostObj::getSimpleBorder(float* border){
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

bool SpeedpostObj::getBoxPoints(QVector<float>& points){
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

int SpeedpostObj::getDefaultDetailLevel(){
    return -3;
}

void SpeedpostObj::save(QTextStream* out){
    if (!loaded) return;
*(out) << "	Speedpost (\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "		Speed_Digit_Tex ( "<<this->speedDigitTex<<" )\n";
*(out) << "		Speed_Sign_Shape ( "<<this->speedSignShape[0];
for(int i = 0; i < this->speedSignShape[0]*4; i++)
    *(out) <<" "<<this->speedSignShape[i+1];
*(out) <<" )\n";
//<<" "<<this->speedSignShape[1]<<" "<<this->speedSignShape[2]<<" "<<this->speedSignShape[3]<<" "<<this->speedSignShape[4]<<" "<<this->speedSignShape[5]<<" "<<this->speedSignShape[6]<<" "<<this->speedSignShape[7]<<" "<<this->speedSignShape[8]<<" )\n";
*(out) << "		Speed_Text_Size ( "<<this->speedTextSize[0]<<" "<<this->speedTextSize[1]<<" "<<this->speedTextSize[2]<<" )\n";
for(int i = 0; i < this->trItemId.size(); i+=2)
    *(out) << "		TrItemId ( "<<this->trItemId[i]<<" "<<this->trItemId[i+1]<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		QDirection ( "<<this->qDirection[0]<<" "<<this->qDirection[1]<<" "<<-this->qDirection[2]<<" "<<this->qDirection[3]<<" )\n";
*(out) << "		VDbId ( "<<this->vDbId<<" )\n";
*(out) << "	)\n";
}