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

void SoundRegionObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    this->qDirection[2] = -this->qDirection[2];
    this->loaded = true;

    setMartix();
}

void SoundRegionObj::initTrItems(float* tpos){
    /*if(tpos == NULL)
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
    this->drawPositions = NULL;*/
}

void SoundRegionObj::set(QString sh, QString val){
    if (sh == ("filename")) {
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
        fileName = ParserX::odczytajtc(data);
        return;
    }
    if (sh == ("soundregionroty")) {
        soundregionRoty = ParserX::parsujr(data);
        return;
    }
    if (sh == ("soundregiontracktype")) {
        soundregionTrackType = ParserX::parsujr(data);
        return;
    }
    if (sh == ("tritemid")) {
        //this->trItemId = new int[2];
        //his->trItemId[0] = ParserX::parsujUint(data);
        //this->trItemId[1] = ParserX::parsujUint(data);
        trItemId.push_back(ParserX::parsujUint(data));
        trItemId.push_back(ParserX::parsujUint(data));
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SoundRegionObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor) {
    if (!loaded) return;

    if(Game::viewInteractives) 
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
            pointer3d = new TrackItemObj(0);
            pointer3d->setMaterial(1.0,1.0,0.0);
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
            lpoints[ptr++] = tpoints[1]+0.5;
            lpoints[ptr++] = -tpoints[2];
            if((i > 0 && i < trItemId.size()/2-1) || trItemId.size() == 2){
            //if(i == 0 || i == trItemId.size()/2 - 1){
                lpoints[ptr++] = tpoints[0];
                lpoints[ptr++] = tpoints[1]+0.5;
                lpoints[ptr++] = -tpoints[2];
            }
        }
        drawLine->init(lpoints, ptr, drawLine->V, GL_LINES);
        drawLine->setMaterial(1.0, 1.0, 0.0);
    }
    
    //int aaa = drawPosition[0];
    //if(pos == NULL) return;
    Mat4::identity(gluu->objStrMatrix);
    gluu->setMatrixUniforms();
    int useSC;
    
    drawLine->render();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 0 * (drawPositionB[4] - this->x), drawPositionB[1] + 1, -drawPositionB[2] + 0 * (-drawPositionB[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[3]);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    pointer3d->render(selectionColor + (1)*65536*25*useSC);
    gluu->mvPopMatrix();
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[0] + 0 * (drawPositionE[4] - this->x), drawPositionE[1] + 1, -drawPositionE[2] + 0 * (-drawPositionE[5] - this->y));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[3]);
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    pointer3d->render(selectionColor + (1)*65536*25*useSC);
    gluu->mvPopMatrix();
};