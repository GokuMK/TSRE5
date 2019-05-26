/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "CarSpawnerObj.h"
#include "SFile.h"
#include "ShapeLib.h"
#include "GLMatrix.h"
#include <math.h>
#include "ParserX.h"
#include <QDebug>
#include "Game.h"
#include "TDB.h"
#include "TRitem.h"
#include "TrackItemObj.h"
#include "OglObj.h"
#include "TS.h"
#include <math.h>
#include <QFile>
#include "FileBuffer.h"
#include "ReadFile.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

QVector<CarSpawnerObj::CarSpawnerList> CarSpawnerObj::carSpawnerList;

void CarSpawnerObj::LoadCarSpawnerList(){
    QString path = Game::root + "/routes/" + Game::route + "/carspawn.dat";
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    ParserX::NextLine(data);
    QString sh = "";
    parseCarList(data);
    delete data;
    
    path = Game::root + "/routes/" + Game::route + "/openrails/carspawn.dat";
    path.replace("//", "/");
    qDebug() << path;
    file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    ParserX::NextLine(data);
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("carspawnerlist")) {
            parseCarList(data);
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }
    delete data;
}

void CarSpawnerObj::parseCarList(FileBuffer* data){
    QString sh;
    carSpawnerList.push_back(CarSpawnerList());
    carSpawnerList.back().name = "DEFAULT";
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("listname")) {
            carSpawnerList.back().name = ParserX::GetString(data);
            qDebug() << carSpawnerList.back().name;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ignorexrotation")) {
            carSpawnerList.back().ignoreXRot = true;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("carspawneritem")) {
            carSpawnerList.back().carName.push_back(ParserX::GetString(data));
            carSpawnerList.back().val.push_back(ParserX::GetNumber(data));
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
    }
    return;
}

CarSpawnerObj::CarSpawnerObj() {
    this->shape = -1;
    this->loaded = false;
    this->staticFlags = 0x100;
    this->internalLodControl = true;

    //pointer3d = new TrackItemObj();
    //pointer3dSelected = new TrackItemObj();
}

CarSpawnerObj::CarSpawnerObj(const CarSpawnerObj& o) : WorldObj(o) {
    trItemId[0] = o.trItemId[0];
    trItemId[1] = o.trItemId[1];
    trItemId[2] = o.trItemId[2];
    trItemId[3] = o.trItemId[3];
    trItemIdCount = o.trItemIdCount;
    carFrequency = o.carFrequency;
    carAvSpeed = o.carAvSpeed;
    rotB = o.rotB;
    rotE = o.rotE;
    selectionValue = o.selectionValue;
    carspawnerListName = o.carspawnerListName;
}

WorldObj* CarSpawnerObj::clone(){
    return new CarSpawnerObj(*this);
}

CarSpawnerObj::~CarSpawnerObj() {
}

void CarSpawnerObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;
    this->skipLevel = 1;
    this->modified = false;
    
    /*if(this->typeID == this->carspawner){
        this->carAvSpeed = 20;
        this->carFrequency = 5;
    }*/
    
    carListId = 0;
    if(carspawnerListName.length() > 0){
        for(int i = 0; i < this->carSpawnerList.size(); i++){
            if(carspawnerListName == this->carSpawnerList[i].name ){
                carListId = i;
                break;
            }
        }
    }
    
    setMartix();
}

CarSpawnerObj::SimpleCar::SimpleCar(){
}

CarSpawnerObj::SimpleCar::~SimpleCar(){
    //if(drawPosition != NULL){
    //    delete[] drawPosition;
    //    drawPosition = NULL;
    //}
}


CarSpawnerObj::SimpleCar::SimpleCar(QString name){
    carName = name;
    QString resPath = Game::root + "/routes/" + Game::route + "/shapes";
    shapeId = Game::currentShapeLib->addShape(resPath +"/"+ name);
    shapePointer = Game::currentShapeLib->shape[shapeId];
    shapeState = shapePointer->newState();
    shapePointer->setAnimated(shapeState, true);
}

void CarSpawnerObj::SimpleCar::updateSim(float deltaTime){
    trPosMb += speed*deltaTime*direction;
    if(trPosMb*direction > trPosMe*direction)
        loaded = false;
    if(trPosMb < 0)
        loaded = false;
    if(shapePointer != NULL){
        shapePointer->updateSim(deltaTime, shapeState);
    }
}

void CarSpawnerObj::SimpleCar::render(GLUU *gluu, int selectionColor){
    //if(drawPosition == NULL)
    //    drawPosition = new float[7];
        //qDebug() <<"car"<< trNodeId<< trPosM;
        bool ok = Game::roadDB->getDrawPositionOnTrNode(drawPosition, trNodeId, trPosMb);
        if(!ok){
            loaded = false;
            return;
        }
        drawPosition[0] += 2048 * (drawPosition[5] - x);
        drawPosition[2] -= 2048 * (-drawPosition[6] - y);
    //}
    int useSC;

    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0], drawPosition[1], -drawPosition[2]);
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3] + M_PI);
    if(!ignoreXRot)
        Mat4::rotateX(gluu->mvMatrix, gluu->mvMatrix, drawPosition[4] );
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, M_PI*0.5 + M_PI*0.5*direction);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(shapePointer != NULL){
        shapePointer->render(shapeState);
    }
    //this->shapePointer->render();
    gluu->mvPopMatrix();
}

bool CarSpawnerObj::allowNew(){
    return true;
}

void CarSpawnerObj::set(int sh, FileBuffer* data) {
    if (sh == TS::CarFrequency) {
        data->off++;
        carFrequency = data->getFloat();
        return;
    }
    if (sh == TS::CarAvSpeed) {
        data->off++;
        carAvSpeed = data->getFloat();
        return;
    }
    if (sh == TS::TrItemId) {
        data->off++;
        trItemId[trItemIdCount++] = data->getUint();
        trItemId[trItemIdCount++] = data->getUint();
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void CarSpawnerObj::set(QString sh, FileBuffer* data) {
    if (sh == ("carfrequency")) {
        carFrequency = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("caravspeed")) {
        carAvSpeed = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("tritemid")) {
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        trItemId[trItemIdCount++] = ParserX::GetNumber(data);
        return;
    }
    if (sh == ("ortslistname")) {
        this->carspawnerListName = ParserX::GetString(data);
        return;
    }
     
    WorldObj::set(sh, data);
    return;
}

void CarSpawnerObj::setPosition(int x, int z, float* p){
    TDB* tdb = Game::roadDB;
    
    int nodeId[4];
    nodeId[1] = tdb->findTrItemNodeId(trItemId[1]);
    nodeId[3] = tdb->findTrItemNodeId(trItemId[3]);
    /*if (nodeId[1] < 0 || nodeId[3] < 0) {
        qDebug() << "fail id";
        return;
    }*/
    float playerT[2];
    playerT[0] = x;
    playerT[1] = z;
    float tpos[3];

    if(tdb->findNearestPositionOnTDB(playerT, p, NULL, tpos) < 0)
        return;

    //tdb- >trackItems[trItemId[selectionValue]]-
    if(nodeId[selectionValue] == tpos[0]){
        tdb->trackItems[trItemId[selectionValue]]->setTrackPosition(tpos[1]);
        tdb->updateTrItemRData(tdb->trackItems[trItemId[selectionValue]]);
    }
    //if(tpos[0] != nodeId[1])
    //    return;

    if(this->selectionValue == 1){
        delete[] drawPositionB;
        drawPositionB = NULL;   
    } else if(this->selectionValue == 3){
        delete[] drawPositionE;
        drawPositionE = NULL;
    }
    delete line;
    line = NULL;
    this->modified = true;
}

void CarSpawnerObj::translate(float px, float py, float pz){
    if(pz == 0) 
        return;
    TDB* tdb = Game::roadDB;
    
    int id = tdb->findTrItemNodeId(this->trItemId[this->selectionValue]);
    if (id < 0) {
        qDebug() << "fail id";
        return;
    }
    
    float dlugosc = tdb->getVectorSectionLength(id);
    TRitem* trit = tdb->trackItems[this->trItemId[this->selectionValue]];
    if(trit == NULL) 
        return;
    if(pz < 0){
        trit->trackPositionAdd(-1);
        if(trit->getTrackPosition() < 0)
            trit->setTrackPosition(0);
        tdb->updateTrItemRData(trit);
    } else if(pz > 0){
        trit->trackPositionAdd(1);
        if(trit->getTrackPosition() > dlugosc)
            trit->setTrackPosition(dlugosc);
        tdb->updateTrItemRData(trit);
    }
    if(this->selectionValue == 1){
        delete[] drawPositionB;
        drawPositionB = NULL;   
    } else if(this->selectionValue == 3){
        delete[] drawPositionE;
        drawPositionE = NULL;
    }
    delete line;
    line = NULL;
    this->modified = true;
    setMartix();
}

bool CarSpawnerObj::select(int value){
    this->selectionValue = value;
    this->selected = true;
}

bool CarSpawnerObj::isTrackItem(){
    return true;
}

void CarSpawnerObj::initTrItems(float* tpos){
    if(tpos == NULL)
        return;
    int trNodeId = tpos[0];
    float metry = tpos[1];
    
    int isRoad = 0;
    TDB* tdb = Game::trackDB;
    if(this->typeID == this->carspawner){
        isRoad = 1;
        tdb = Game::roadDB;
    }

    int trItemId[2];

    tdb->newPlatformObject(trItemId, trNodeId, metry, this->typeID);

    this->trItemIdCount = 4;
    this->trItemId[0] = isRoad;
    this->trItemId[1] = trItemId[0];
    this->trItemId[2] = isRoad;
    this->trItemId[3] = trItemId[1];
}

void CarSpawnerObj::deleteTrItems(){
    TDB* tdb = Game::trackDB;
    TDB* rdb = Game::roadDB;
    for(int i = 0; i<this->trItemIdCount/2; i++){
        if(this->trItemId[i*2] == 0)
            tdb->deleteTrItem(this->trItemId[i*2+1]);
        else if(this->trItemId[i*2] == 1)
            rdb->deleteTrItem(this->trItemId[i*2+1]);
        this->trItemId[i*2+1] = -1;
    }
}
    
float CarSpawnerObj::getLength(){
    TDB* tdb = Game::trackDB;
    TRitem* p1 = tdb->trackItems[this->trItemId[1]];
    TRitem* p2 = tdb->trackItems[this->trItemId[3]];
    if(p1 == NULL || p2 == NULL) return 0;
    return fabs(p2->getTrackPosition() - p1->getTrackPosition());
}

float CarSpawnerObj::getCarNumber(){
    return this->carFrequency;
}

float CarSpawnerObj::getCarSpeed(){
    return this->carAvSpeed;
}

void CarSpawnerObj::setCarNumber(float val){
    this->carFrequency = val;
    this->modified = true;
}

void CarSpawnerObj::setCarSpeed(float val){
    this->carAvSpeed = val;
    this->modified = true;
}


void CarSpawnerObj::updateSim(float deltaTime){
    if(!this->loaded) 
        return;
    
    carsNewTime += deltaTime;

    if(carsNewTime > carFreq){
        TDB* tdb = Game::roadDB;

        int carRnd = std::rand()%carSpawnerList[carListId].carName.size();
        
        cars.push_back(SimpleCar(carSpawnerList[carListId].carName[carRnd]));
        cars.back().ignoreXRot = carSpawnerList[carListId].ignoreXRot;
        cars.back().trNodeId = tdb->findTrItemNodeId(this->trItemId[1]);
        //qDebug() << "this->trItemId[1]"<<this->trItemId[1];
        //qDebug() << "this->trItemId[3]"<<this->trItemId[3];
        cars.back().trPosMb = tdb->trackItems[this->trItemId[1]]->getTrackPosition();
        cars.back().trPosMe = tdb->trackItems[this->trItemId[3]]->getTrackPosition();
        if(cars.back().trPosMe > cars.back().trPosMb)
            cars.back().direction = 1;
        else
            cars.back().direction = -1;
        cars.back().speed = carAvSpeed;
        cars.back().x = x;
        cars.back().y = y;
        cars.back().loaded = true;
        //carFreq = (float)(std::rand()%(100)+10)/1000.0;
        carFreq = (float)(std::rand()%((int)carFrequency*1000+1)+500)/1000.0;
        carsNewTime = 0;
    }
    for(int i = 0; i < cars.size(); i++){
        cars[i].updateSim(deltaTime);
        if(!cars[i].loaded){
            cars.remove(i);
            i--;
        }
    }
};

void CarSpawnerObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    //Vector3f *pos = tdb->getDrawPositionOnTrNode(playerT, id, this->trItemSData1);
    if(!this->loaded) 
        return;
    
    if(Game::showWorldObjPivotPoints){
        gluu->mvPushMatrix();
        Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);
        gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
        if(pointer3d == NULL){
            pointer3d = new TrackItemObj(1);
            pointer3d->setMaterial(0.9,0.9,0.7);
        }
        pointer3d->render(selectionColor);
        gluu->mvPopMatrix();
    }
    
    if(selectionColor != 0){
        gluu->disableTextures(selectionColor);
    } else {
        gluu->enableTextures();
    }

    for(int i = 0; i < cars.size(); i++){
        cars[i].render(gluu, selectionColor);
    }
    
    if(Game::viewInteractives && renderMode != gluu->RENDER_SHADOWMAP) 
        this->renderTritems(gluu, selectionColor);
};

void CarSpawnerObj::renderTritems(GLUU* gluu, int selectionColor){
    
    if (drawPositionB == NULL) {
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        if (id < 1) {
            qDebug() << "platform fail id "<<id;
            this->loaded = false;
            return;
        }
        //qDebug() << "id: "<< this->trItemId[1] << " "<< id;
        //qDebug() << "d: "<< tdb->trackItems[this->trItemId[1]]->trItemSData1;
        drawPositionB = new float[7];
        bool ok = tdb->getDrawPositionOnTrNode(drawPositionB, id, tdb->trackItems[this->trItemId[1]]->getTrackPosition());
        if(!ok){
            qDebug() << "platform fail tdb "<<id;
            this->loaded = false;
            return;
        }
        drawPositionB[0] += 2048 * (drawPositionB[5] - this->x);
        drawPositionB[2] -= 2048 * (-drawPositionB[6] - this->y);
    }
    if (drawPositionE == NULL) {
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[3]);
        if (id < 1) {
            qDebug() << "platform fail id";
            this->loaded = false;
            return;
        }
        drawPositionE = new float[7];
        bool ok = tdb->getDrawPositionOnTrNode(drawPositionE, id, tdb->trackItems[this->trItemId[3]]->getTrackPosition());
        if(!ok){
            qDebug() << "platform fail tdb "<<id;
            this->loaded = false;
            return;
        }
        drawPositionE[0] += 2048 * (drawPositionE[5] - this->x);
        drawPositionE[2] -= 2048 * (-drawPositionE[6] - this->y);
    }

    if(line == NULL){
        if(spointer3d == NULL) spointer3d = new TrackItemObj();
        if(spointer3dSelected == NULL) spointer3dSelected = new TrackItemObj();

        makelineShape();
        
        if(this->typeID == this->platform){
            line->setMaterial(0.0, 1.0, 0.0);
            spointer3d->setMaterial(0.0, 1.0, 0.0);
            spointer3dSelected->setMaterial(0.5, 1.0, 0.5);
        }
        if(this->typeID == this->siding){
            line->setMaterial(1.0, 0.7, 0.0);
            spointer3d->setMaterial(1.0, 0.7, 0.0);
            spointer3dSelected->setMaterial(1.0, 1.0, 0.5);
        }
        if(this->typeID == this->carspawner){
            line->setMaterial(0.4, 0.0, 1.0);
            spointer3d->setMaterial(0.4, 0.0, 1.0);
            spointer3dSelected->setMaterial(0.9, 0.5, 1.0);
        }
    }
    //if(pos == NULL) return;
    //gluu->setMatrixUniforms();
    //float dlugosc = (float) sqrt(pow(drawPositionB[2]-drawPositionE[2], 2) + pow(drawPositionB[0]-drawPositionE[0], 2));
    float aa = (drawPositionE[2]-drawPositionB[2]);
    if(aa != 0) aa = (aa/fabs(aa));
    //float rot = (aa+1)*M_PI/2 + (float)(atan((drawPositionB[0]-drawPositionE[0])/(drawPositionB[2]-drawPositionE[2]))); 
    
    //(-(float)(atan((drawPositionB[1]-drawPositionE[1])/(dlugosc))
    int useSC;
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[3] + rotB*M_PI);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 1) 
        spointer3dSelected->render(selectionColor | 1*useSC);
    else
        spointer3d->render(selectionColor | 1*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[0] + 0 * (drawPositionE[4] - this->x), drawPositionE[1] + 1, -drawPositionE[2] + 0 * (-drawPositionE[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[3] + rotE*M_PI);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(this->selected && this->selectionValue == 3) 
        spointer3dSelected->render(selectionColor | 3*useSC);
    else
        spointer3d->render(selectionColor | 3*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    line->render();
    gluu->mvPopMatrix();
};

void CarSpawnerObj::expand(){
    TDB *tdb = Game::roadDB;
    int id = tdb->findTrItemNodeId(this->trItemId[1]);
    float maxLen = tdb->getVectorSectionLength(id);
    float p1 = tdb->trackItems[this->trItemId[1]]->getTrackPosition();
    float p2 = tdb->trackItems[this->trItemId[3]]->getTrackPosition();
    if( p1 < p2 ){
        tdb->trackItems[this->trItemId[1]]->setTrackPosition(1);
        tdb->trackItems[this->trItemId[3]]->setTrackPosition(maxLen-1);
    } else {
        tdb->trackItems[this->trItemId[3]]->setTrackPosition(1);
        tdb->trackItems[this->trItemId[1]]->setTrackPosition(maxLen-1);
    }
    tdb->updateTrItemRData(tdb->trackItems[this->trItemId[1]]);
    tdb->updateTrItemRData(tdb->trackItems[this->trItemId[3]]);
    delete[] drawPositionB;
    drawPositionB = NULL;   
    delete[] drawPositionE;
    drawPositionE = NULL;
    delete line;
    line = NULL;
    this->modified = true;
    
}

void CarSpawnerObj::makelineShape(){
        line = new OglObj();
        float *ptr, *punkty;// = new float[6];
        int length, len = 0;
        TDB* tdb = Game::trackDB;
        if(this->typeID == this->carspawner)
            tdb = Game::roadDB;
        int id = tdb->findTrItemNodeId(this->trItemId[1]);
        tdb->getVectorSectionLine(ptr, length, x, y, id, true);
        
        float beg = tdb->trackItems[this->trItemId[1]]->getTrackPosition();
        float end = tdb->trackItems[this->trItemId[3]]->getTrackPosition();
        float posB[3], posE[3];
        int side = 0;
        if(end < beg){
            rotE = 1;
            rotB = 0;
            side = 1;
            float t = end;
            end = beg;
            beg = t;
            posE[0] = drawPositionB[0];
            posE[1] = drawPositionB[1];
            posE[2] = drawPositionB[2];
            posB[0] = drawPositionE[0];
            posB[1] = drawPositionE[1];
            posB[2] = drawPositionE[2];     
        } else {
            rotE = 0;
            rotB = 1;
            side = -1;
            posB[0] = drawPositionB[0];
            posB[1] = drawPositionB[1];
            posB[2] = drawPositionB[2];
            posE[0] = drawPositionE[0];
            posE[1] = drawPositionE[1];
            posE[2] = drawPositionE[2];
        }
        //for(int i = 0; i < length; i+=12){
        //    qDebug() << ptr[i+5] << "-"<<ptr[i+11];
        //}
        float quat90[4]{0.707,0,-0.707,0};
        if(end - beg > 4){
            punkty = new float[length+6];
            //qDebug() << beg <<" "<<end;
            float tp[3], tp1[3], tp2[3], tp3[3];
            bool endd = false;
            for(int i = 0; i < length; i+=12){
                if(ptr[i+5] < beg) continue;
                if(ptr[i+5+12] > end)
                    endd = true;
                
                if(len == 0)
                    Vec3::set(tp1, posB[0], posB[1]+1, -posB[2]);
                else 
                    Vec3::set(tp1, ptr[i+0], ptr[i+1]+1, ptr[i+2]);
                
                if(endd)
                    Vec3::set(tp2, posE[0], posE[1]+1, -posE[2]); 
                else
                    Vec3::set(tp2, ptr[i+6], ptr[i+7]+1, ptr[i+8]);
                
                Vec3::sub(tp, tp2, tp1);
                Vec3::normalize(tp, tp);
                Vec3::transformQuat(tp3, tp, quat90);
                
                punkty[len++] = tp1[0];
                punkty[len++] = tp1[1];
                punkty[len++] = tp1[2];
                punkty[len++] = tp2[0];
                punkty[len++] = tp2[1];
                punkty[len++] = tp2[2];

                if(endd) break;
            }
        } else {
            punkty = new float[6];
            len = 6;
            punkty[0] = posB[0];
            punkty[1] = posB[1]+1;
            punkty[2] = -posB[2];
            punkty[len-3] = posE[0];
            punkty[len-2] = posE[1]+1;
            punkty[len-1] = -posE[2];
        }

        line->init(punkty, len, line->V, GL_LINES);
        delete[] ptr;
        delete[] punkty;
}

int CarSpawnerObj::getDefaultDetailLevel(){
    return -7;
}

QString CarSpawnerObj::getCarListName(){
    return carspawnerListName;
}

void CarSpawnerObj::setCarListName(QString val){
    carspawnerListName = val;
    carListId = 0;
    for(int i = 0; i < this->carSpawnerList.size(); i++)
        if(carspawnerListName == this->carSpawnerList[i].name ){
            carListId = i;
            break;
        }
    modified = true;
}

void CarSpawnerObj::save(QTextStream* out) {
    if (!loaded) return;
    QString flags = ParserX::MakeFlagsString(this->staticFlags);

    *(out) << "	CarSpawner (\n";
    *(out) << "		UiD ( " << this->UiD << " )\n";
    *(out) << "		CarFrequency ( " << this->carFrequency << " )\n";
    *(out) << "		CarAvSpeed ( " << this->carAvSpeed << " )\n";
    if (this->carspawnerListName.length() > 0)
        *(out) << "		ORTSListName ( " << ParserX::AddComIfReq(this->carspawnerListName) << " )\n";
    *(out) << "		TrItemId ( " << this->trItemId[0] << " " << this->trItemId[1] << " )\n";
    *(out) << "		TrItemId ( " << this->trItemId[2] << " " << this->trItemId[3] << " )\n";
    *(out) << "		StaticFlags ( " << flags << " )\n";
    *(out) << "		Position ( " << this->position[0] << " " << this->position[1] << " " << -this->position[2] << " )\n";
    *(out) << "		QDirection ( " << this->qDirection[0] << " " << this->qDirection[1] << " " << -this->qDirection[2] << " " << this->qDirection[3] << " )\n";
    *(out) << "		VDbId ( " << this->vDbId << " )\n";
    if (this->staticDetailLevel > -1)
        *(out) << "		StaticDetailLevel ( " << this->staticDetailLevel << " )\n";
    *(out) << "	)\n";
}