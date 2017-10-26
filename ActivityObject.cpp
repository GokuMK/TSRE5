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
    
    if(con == NULL)
        return;

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

void ActivityObject::setSpeedZoneData(){
    objectType = "RestrictedZone";
    objectTypeId = ActivityObject::RESTRICTEDSPEEDZONE;
    speedZoneData = new ActivityObject::SpeedZone();
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
    return true;
}

int ActivityObject::getSelectedElementId(){
    if(con != NULL){
        return con->selectedIdx;
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
        if(failedSignalData != NULL)
            return failedSignalData->getWorldPosition(posTW);
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
        //renderZone(gluu, playerT, selectionColor);
    }
    
    if(objectTypeId == ActivityObject::FAILEDSIGNAL ){
        if(failedSignalData != NULL)
            failedSignalData->render(gluu, playerT, selectionColor, selected);
    }
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
                pointer3dSelected->setMaterial(0.9,0.3,0.9);
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
    menu->addAction(contextMenuActions["ToggleDirection"]);
}

void ActivityObject::menuToggleDirection(){
    toggleDirection();
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