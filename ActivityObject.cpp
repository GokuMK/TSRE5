/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityObject.h"
#include "Consist.h"
#include "Game.h"
#include "GLUU.h"
#include <QDebug>
#include <QMenu>
#include "TDB.h"
#include "GLMatrix.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "GLMatrix.h"
#include "TrackItemObj.h"
#include "TRitem.h"
#include "Activity.h"
#include "TerrainLib.h"
#include <math.h>

ActivityObject::~ActivityObject() {
}

ActivityObject::ActivityObject(){
    typeObj = GameObj::activityobj;
    id = -1;
}

ActivityObject::ActivityObject(int tid){
    typeObj = GameObj::activityobj;
    id = tid;
}

ActivityObject::ActivityObject(const ActivityObject& orig){
    typeObj = orig.typeObj;
    id = orig.id;
    con = orig.con;
    direction = orig.direction;
    objectType = orig.objectType;
    objectTypeId = orig.objectTypeId;
    parentActivity = orig.parentActivity;
    selected = orig.selected;
    selectionValue = orig.selectionValue;
    failedSignalData = orig.failedSignalData;
    speedZoneData = orig.speedZoneData;
    //failedSignal = orig.failedSignal;
    modified = orig.modified;
    Vec4::copy((float*)tile, (float*)orig.tile);
    //Vec4::copy((float*)restrictedSpeedZoneStart, (float*)orig.restrictedSpeedZoneStart);
    //Vec4::copy((float*)restrictedSpeedZoneEnd, (float*)orig.restrictedSpeedZoneEnd);
}

void ActivityObject::setPosition(int x, int z, float* p){
    if(Game::trackDB == NULL)
        return;
    
    if(con != NULL){
        float tp[3];
        float tpos[3];
        float posT[2];

        Vec3::copy(tp, p);
        Game::check_coords(x, z, tp);
        posT[0] = x;
        posT[1] = z;

        int ok = Game::trackDB->findNearestPositionOnTDB(posT, tp, NULL, tpos);
        if(ok >= 0){
            tile[0] = posT[0];
            tile[1] = -posT[1];
            tile[2] = tp[0];
            tile[3] = -tp[2];
            con->isOnTrack = false;
            modified = true;
        }
    }
    
    if(speedZoneData != NULL){
        float playerT[2];
        playerT[0] = x;
        playerT[1] = z;
        float tpos[3];

        if(Game::trackDB->findNearestPositionOnTDB(playerT, p, NULL, tpos) < 0)
            return;
        
        if(tpos[0] != speedZoneData->trid[0])
            return;
        
        if(speedZoneData->selectionValue == 1){
            speedZoneData->start[0] = playerT[0];
            speedZoneData->start[1] = -playerT[1];
            speedZoneData->start[2] = p[0];
            speedZoneData->start[3] = -p[2];
        } else {
            speedZoneData->end[0] = playerT[0];
            speedZoneData->end[1] = -playerT[1];
            speedZoneData->end[2] = p[0];
            speedZoneData->end[3] = -p[2];
        }
        
        speedZoneData->drawPositionB = NULL;
        speedZoneData->drawPositionE = NULL;
        speedZoneData->lineShape = NULL;
        modified = true;
    }
}

void ActivityObject::reverseWagonListUnit(){
    if(con == NULL)
        return;
    
    con->flipSelected();
    con->isOnTrack = false;
    modified = true;
}

void ActivityObject::toggleDirection(){
    if(con == NULL)
        return;
    
    direction = abs(direction - 1);
    con->isOnTrack = false;
    modified = true;
}

void ActivityObject::setFailedSignalData(int id){
    objectType = "FailedSignal";
    objectTypeId = ActivityObject::FAILEDSIGNAL;
    failedSignalData = new ActivityObject::FailedSignalData();
    failedSignalData->failedSignal = id;
}

void ActivityObject::setSpeedZoneData(float* drawposition){
    objectType = "RestrictedZone";
    objectTypeId = ActivityObject::RESTRICTEDSPEEDZONE;
    speedZoneData = new ActivityObject::SpeedZone();
    
    if(drawposition != NULL){
        speedZoneData->start[0] = drawposition[5];
        speedZoneData->start[1] = drawposition[6];
        speedZoneData->end[0] = drawposition[5];
        speedZoneData->end[1] = drawposition[6];
        speedZoneData->start[2] = drawposition[0];
        speedZoneData->start[3] = drawposition[2];
        speedZoneData->end[2] = drawposition[0];
        speedZoneData->end[3] = drawposition[2];
    }
}

QString ActivityObject::getSpeedZoneName(){
    QString n = "";
    if(objectTypeId == ActivityObject::RESTRICTEDSPEEDZONE && speedZoneData != NULL){
        n += QString::number(speedZoneData->start[0])+" "+QString::number(speedZoneData->start[1])+" "+QString::number(speedZoneData->start[2])+" "+QString::number(speedZoneData->start[3]);
    }
    return n;
}

int ActivityObject::getFailedSignalId(){
    if(failedSignalData == NULL)
        return -1;
    return failedSignalData->failedSignal;
}

void ActivityObject::setParentActivity(Activity* a){
    parentActivity = a;
}

QString ActivityObject::getParentName(){
    if(parentActivity == NULL)
        return "";
    return parentActivity->header->name;
}

bool ActivityObject::select(int value){
    selected = true;
    selectionValue = value;
    if(con != NULL){
        con->select(value);
    }
    if(speedZoneData != NULL)
        speedZoneData->selectionValue = value;
    return true;
}

int ActivityObject::getId(){
    return id;
}

int ActivityObject::getSelectedElementId(){
    if(con != NULL){
        return con->selectedIdx;
    }
    if(speedZoneData != NULL){
        return speedZoneData->selectionValue;
    }
    return 0;
}

bool ActivityObject::getElementPosition(int id, float *posTW){
    if(objectTypeId == WAGONLIST){
        if(con != NULL)
            return con->getWagonWorldPosition(id, posTW);
    }
    if(objectTypeId == FAILEDSIGNAL){
        if(failedSignalData != NULL)
            return failedSignalData->getWorldPosition(posTW);
    }
    if(objectTypeId == RESTRICTEDSPEEDZONE){
        if(speedZoneData != NULL)
            return speedZoneData->getWorldPosition(posTW);
    }
    return false;
}

bool ActivityObject::unselect(){
    selected = false;
    selectionValue = -1;
    if(con != NULL){
        con->unselect();
    }
    return false;
}

void ActivityObject::remove(){
    if(parentActivity == NULL)
        return;
    if(objectTypeId == FAILEDSIGNAL){
        parentActivity->deleteObjectFailedSignal(id);
        return;
    }
    if(objectTypeId == RESTRICTEDSPEEDZONE){
        parentActivity->deleteObjectSpeedZone(id);
        return;
    }
    parentActivity->deleteObject(id);
}

bool ActivityObject::isUnSaved(){
    if(con != NULL)
        if(con->isUnSaved())
            return true;
    return modified;
}

void ActivityObject::setModified(bool val){
    modified = val;
    if(con != NULL)
        con->setModified(val);
}

void ActivityObject::render(GLUU* gluu, float* playerT, int renderMode, int index){
    int selectionColor = 0;
    if(renderMode == gluu->RENDER_SELECTION){
        selectionColor = 11 << 20;
        selectionColor |= index << 8;
    }
    
    if(objectTypeId == ActivityObject::WAGONLIST ){
        if(con != NULL){
            if (!con->isOnTrack)
                con->initOnTrack(tile, direction);
            con->renderOnTrack(gluu, playerT, selectionColor);
        }
    }
    
    if(objectTypeId == ActivityObject::RESTRICTEDSPEEDZONE ){
        if(speedZoneData != NULL)
            speedZoneData->render(gluu, playerT, selectionColor, selected);
    }
    
    if(objectTypeId == ActivityObject::FAILEDSIGNAL ){
        if(failedSignalData != NULL)
            failedSignalData->render(gluu, playerT, selectionColor, selected);
    }
}

void ActivityObject::SpeedZone::render(GLUU* gluu, float* playerT, int selectionColor, bool selected){
    if(init < 0)
        return;
    float posT[2], pos[3];
    if (drawPositionB == NULL) {
        TDB* tdb = Game::trackDB;
        Vec2::set(posT, start[0], -start[1]);
        float h = Game::terrainLib->getHeight(start[0], -start[1], start[2], start[3]);
        Vec3::set(pos, start[2], h, -start[3]);
        qDebug() << posT[0]<< posT[1];
        qDebug() << pos[0]<< pos[1]<< pos[2];
        int ok = Game::trackDB->findNearestPositionOnTDB(posT, pos, NULL, trid);
        if(ok < 0) {
            qDebug() << "SpeedZone fail";
            init = -1;
            return;
        }
        if (trid[0] < 1) {
            qDebug() << "SpeedZone fail id "<<trid[0];
            init = -1;
            return;
        }
        drawPositionB = new float[7];
        bool ok1 = tdb->getDrawPositionOnTrNode(drawPositionB, trid[0], trid[1]);
        if(!ok1){
            qDebug() << "SpeedZone fail tdb "<<trid[0];
            init = -1;
            return;
        }
        drawPositionB[0] += 2048 * (drawPositionB[5] - start[0]);
        drawPositionB[2] -= 2048 * (-drawPositionB[6] - -start[1]);
    }
    if (drawPositionE == NULL) {
        TDB* tdb = Game::trackDB;
        Vec2::set(posT, end[0], -end[1]);
        float h = Game::terrainLib->getHeight(end[0], -end[1], end[2], end[3]);
        Vec3::set(pos, end[2], h, -end[3]);

        qDebug() << posT[0]<< posT[1];
        qDebug() << pos[0]<< pos[1]<< pos[2];

        int ok = Game::trackDB->findNearestPositionOnTDB(posT, pos, NULL, trid+3);
        if(ok < 0) {
            qDebug() << "SpeedZone fail";
            init = -1;
            return;
        }
        if (trid[3] < 1) {
            qDebug() << "SpeedZone fail id "<<trid[3];
            init = -1;
            return;
        }
        drawPositionE = new float[7];
        bool ok1 = tdb->getDrawPositionOnTrNode(drawPositionE, trid[3], trid[4]);
        if(!ok1){
            qDebug() << "SpeedZone fail tdb "<<trid[3];
            init = -1;
            return;
        }
        drawPositionE[0] += 2048 * (drawPositionE[5] - start[0]);
        drawPositionE[2] -= 2048 * (-drawPositionE[6] - -start[1]);
    }
    
    if(pointer3d == NULL) pointer3d = new TrackItemObj();
    if(pointer3dSelected == NULL) pointer3dSelected = new TrackItemObj();
    
    if(lineShape == NULL){
        makelineShape();
        lineShape->setMaterial(1.0, 0.0, 0.4);
        pointer3d->setMaterial(1.0, 0.0, 0.4);
        pointer3dSelected->setMaterial(1.0, 0.3, 0.7);
    }
    float aa = (drawPositionE[2]-drawPositionB[2]);
    if(aa != 0) aa = (aa/fabs(aa));
    int useSC;

    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[0] + 2048 * (start[0] - playerT[0]), drawPositionB[1] + 1, -drawPositionB[2] + 2048 * (-start[5] - playerT[1]));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionB[3] + rotB*M_PI);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(selected && selectionValue == 1) 
        pointer3dSelected->render(selectionColor | 1*useSC);
    else
        pointer3d->render(selectionColor | 1*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[0] + 2048 * (start[0] - playerT[0]), drawPositionE[1] + 1, -drawPositionE[2] + 2048 * (-start[1] - playerT[1]));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPositionE[3] + rotE*M_PI);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(selected && selectionValue == 3) 
        pointer3dSelected->render(selectionColor | 3*useSC);
    else
        pointer3d->render(selectionColor | 3*useSC);
    gluu->mvPopMatrix();
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * (start[0] - playerT[0]), 0, 2048 * (-start[1] - playerT[1]));
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    lineShape->render();
    gluu->mvPopMatrix();
};

void ActivityObject::SpeedZone::makelineShape(){
        lineShape = new OglObj();
        float *ptr, *punkty;
        int length, len = 0;
        TDB* tdb = Game::trackDB;
        int id = tdb->findTrItemNodeId(trid[0]);
        tdb->getVectorSectionLine(ptr, length, start[0], -start[1], trid[0], true);
        
        float beg = trid[1];
        float end = trid[4];
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

        float quat90[4]{0.707,0,-0.707,0};
        if(end - beg > 4){
            punkty = new float[length+6];
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
        lineShape->init(punkty, len, lineShape->V, GL_LINES);
        delete[] ptr;
        delete[] punkty;
}

bool ActivityObject::SpeedZone::getWorldPosition(float *posTW){
    if(drawPositionB == NULL)
        return false;
    posTW[0] = drawPositionB[5];
    posTW[1] = drawPositionB[6];
    posTW[2] = drawPositionB[0];
    posTW[3] = drawPositionB[1];
    posTW[4] = drawPositionB[2];
    return true;
}

bool ActivityObject::FailedSignalData::getWorldPosition(float *posTW){
    if(drawPosition == NULL)
        return false;
    posTW[0] = drawPosition[5];
    posTW[1] = drawPosition[6];
    posTW[2] = drawPosition[0];
    posTW[3] = drawPosition[1];
    posTW[4] = drawPosition[2];
    return true;
}

void ActivityObject::FailedSignalData::render(GLUU* gluu, float* playerT, int selectionColor, bool selected){
    if(init < 0)
        return;
    if(init == 0){
        TDB* tdb = Game::trackDB;
        if(drawPosition == NULL){
            qDebug() << "ss";
            int id = tdb->findTrItemNodeId(failedSignal);
            if (id < 0) {
                qDebug() << "fail id";
                return;
            }

            drawPosition = new float[7];
            bool ok = tdb->getDrawPositionOnTrNode(drawPosition, id, tdb->trackItems[failedSignal]->getTrackPosition());
            if(!ok){
                qDebug() << "fail id";
                init = -1;
                return;
            }
            if(pointer3d == NULL){
                pointer3d = new TrackItemObj(1);
                pointer3d->setMaterial(0.8,0.2,0.8);
            }
            if(pointer3dSelected == NULL){
                pointer3dSelected = new TrackItemObj(1);
                pointer3dSelected->setMaterial(0.9,0.5,0.9);
            }
        }
        init = 1;
    }
    
    gluu->mvPushMatrix();
    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, drawPosition[0] + 2048 * (drawPosition[5]-playerT[0]), drawPosition[1] + 1, -drawPosition[2] + 2048 * (-drawPosition[6]-playerT[1]));
    Mat4::rotateY(gluu->mvMatrix, gluu->mvMatrix, drawPosition[3]);
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 2048*(this->trItemRData[3] - playerT[0] ), this->trItemRData[1]+2, -this->trItemRData[2] + 2048*(-this->trItemRData[4] - playerT[1]));
    //Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, this->trItemRData[0] + 0, this->trItemRData[1]+0, -this->trItemRData[2] + 0);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    //int useSC = (float)selectionColor/(float)(selectionColor+0.000001);
    if(selected)
        pointer3dSelected->render(selectionColor);
    else
        pointer3d->render(selectionColor);
    gluu->mvPopMatrix();
}

void ActivityObject::pushContextMenuActions(QMenu *menu){
    if(contextMenuActions["ToggleDirection"] == NULL){
        contextMenuActions["ToggleDirection"] = new QAction(tr("&Toggle Direction")); 
        QObject::connect(contextMenuActions["ToggleDirection"], SIGNAL(triggered()), this, SLOT(menuToggleDirection()));
    }
    if(contextMenuActions["Reverse"] == NULL){
        contextMenuActions["Reverse"] = new QAction(tr("&Reverse Selected")); 
        QObject::connect(contextMenuActions["Reverse"], SIGNAL(triggered()), this, SLOT(menuToggleReverse()));
    }
    if(objectTypeId == WAGONLIST){
        menu->addAction(contextMenuActions["ToggleDirection"]);
        menu->addAction(contextMenuActions["Reverse"]);
    }
}

void ActivityObject::menuToggleDirection(){
    toggleDirection();
}

void ActivityObject::menuToggleReverse(){
    reverseWagonListUnit();
}

void ActivityObject::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("objecttype")) {
            objectType = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("train_config")) {
            objectTypeId = WAGONLIST;
            con = new Consist();
            if (con->load(data)) {
                con->loaded = 1;
                con->setMaxVelocityFixed(true);
                con->initPos();
            }
            //ParserX::SkipToken(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("direction")) {
            direction = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("id")) {
            id = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("tile")) {
            tile[0] = ParserX::GetNumber(data);
            tile[1] = ParserX::GetNumber(data);
            tile[2] = ParserX::GetNumber(data);
            tile[3] = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("startposition")) {
            if(speedZoneData != NULL){
                speedZoneData->start[0] = ParserX::GetNumber(data);
                speedZoneData->start[1] = ParserX::GetNumber(data);
                speedZoneData->start[2] = ParserX::GetNumber(data);
                speedZoneData->start[3] = ParserX::GetNumber(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("endposition")) {
            if(speedZoneData != NULL){
                speedZoneData->end[0] = ParserX::GetNumber(data);
                speedZoneData->end[1] = ParserX::GetNumber(data);
                speedZoneData->end[2] = ParserX::GetNumber(data);
                speedZoneData->end[3] = ParserX::GetNumber(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#activityObject - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void ActivityObject::save(QTextStream* out) {
    if(objectTypeId == RESTRICTEDSPEEDZONE && speedZoneData != NULL){
        *out << "			ActivityRestrictedSpeedZone (\n";
        *out << "				StartPosition ( "<<speedZoneData->start[0]<<" "<<speedZoneData->start[1]<<" "<<speedZoneData->start[2]<<" "<<speedZoneData->start[3]<<" )\n";
        *out << "				EndPosition ( "<<speedZoneData->end[0]<<" "<<speedZoneData->end[1]<<" "<<speedZoneData->end[2]<<" "<<speedZoneData->end[3]<<" )\n";
        *out << "			)\n";
        return;
    }
    if(objectTypeId == FAILEDSIGNAL && failedSignalData != NULL){
        *out << "			ActivityFailedSignal ( " << failedSignalData->failedSignal << " )\n";
        return;
    }
    
    *out << "			ActivityObject (\n";
    *out << "				ObjectType ( "<<ParserX::AddComIfReq(objectType)<<" )\n";
    if(con != NULL){
        *out << "				Train_Config (\n";
        con->save("				", out);
        *out << "				)\n";
    }
    *out << "				Direction ( "<<direction<<" )\n";
    *out << "				ID ( "<<id<<" )\n";
    *out << "				Tile ( "<<tile[0]<<" "<<tile[1]<<" "<<tile[2]<<" "<<tile[3]<<" )\n";
    *out << "			)\n";
}