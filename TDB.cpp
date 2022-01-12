/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TDB.h"
#include <QDebug>
#include <functional>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "GLMatrix.h"
#include "Ref.h"
#include "TRnode.h"
#include "TRitem.h"
#include "DynTrackObj.h"
#include "TrackShape.h"
#include "Intersections.h"
#include "TSectionDAT.h"
#include "SigCfg.h"
#include "SignalShape.h"
#include "FileBuffer.h"
#include "SpeedPostDAT.h"
#include "GLUU.h"
#include "SignalObj.h"
#include "ErrorMessagesLib.h"
#include "ErrorMessage.h"
#include "Route.h"

std::unordered_map<int, TRitem*>* TDB::StaticTrackItems;

TDB::TDB(TSectionDAT* tsection, bool road) {
    loaded = false;
    this->road = road;
    if(this->road){
        tdbId = 1;
        tdbName = ErrorMessage::Source_RDB;
    }
    serial = 0;
    wysokoscSieci = 4;
    iTRitems = 0;
    iTRnodes = 0;

    if(tsection == NULL)
        this->tsection = new TSectionDAT();
    else
        this->tsection = tsection;
    
    return;
}

void TDB::loadTdb(){

    QString sh;
    QString extension = "tdb";
    if(this->road) extension = "rdb";
    QString path = Game::root + "/routes/" + Game::route + "/" + Game::routeName + "." + extension;
    path.replace("//", "/");
    qDebug() << "Wczytywanie pliku tdb: " << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    file.close();
    data->toUtf16();
    data->skipBOM();
    ParserX::NextLine(data);
    iTRnodes = 0;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == "trackdb") {
            loadUtf16Data(data);
            ParserX::SkipToken(data);
            continue;
            
        }
        qDebug() << "#TDB undefined token " << sh;
        ParserX::SkipToken(data);
    }
    
    if(tsection->updateSectionDataRequired){
        this->updateSectionAndShapeIds(tsection->autoFixedSectionIds, tsection->autoFixedShapeIds);
    }
    /*for(int i = 1; i <= iTRnodes; i++){
        int old;
        for(int j = 0; j < trackNodes[i]->iTri; j++){
            if(j > 0 && old > trackItems[trackNodes[i]->trItemRef[j]]->getTrackPosition())
                qDebug() << "--fail!--"<< old << trackItems[trackNodes[i]->trItemRef[j]]->getTrackPosition();
            old = trackItems[trackNodes[i]->trItemRef[j]]->getTrackPosition();
        }
    }*/
    if(!this->road){
        loadTit();
        checkTrSignalRDirs();
        this->speedPostDAT = new SpeedPostDAT();
        this->sigCfg = new SigCfg();
        //checkSignals();
    }
    //save();
    loaded = true;
}

void TDB::loadUtf16Data(FileBuffer *data){
    int i, j, ii, uu;
    float xx;
    int t;
    bool ok;
    QString sh;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if(sh == "tracknodes"){
                    iTRnodes = (int) ParserX::GetNumber(data); //odczytanie ilosci sciezek
                    qDebug() << "TDB TrackNodes count " << iTRnodes;

                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if(sh == "tracknode"){
                            t = (int) ParserX::GetNumber(data); // odczytanie numeru sciezki
                            trackNodes[t] = new TRnode();
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if(sh == "trendnode"){
                                    trackNodes[t]->typ = 0; //typ endnode
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if(sh == "trvectornode"){
                                    trackNodes[t]->typ = 1; //typ vector 
                                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                        if(sh == "trvectorsections"){
                                            uu = (int) ParserX::GetNumberInside(data, &ok);
                                            if(ok){
                                                trackNodes[t]->iTrv = uu;
                                                trackNodes[t]->trVectorSection = new TRnode::TRSect[uu]; // przydzielenie pamieci dla sciezki
                                                for (j = 0; j < uu; j++) {
                                                    for (ii = 0; ii < 16; ii++) {
                                                        xx = ParserX::GetNumber(data);
                                                        if(std::isnan(xx)){
                                                            qDebug() << "#TrackDB: NAN found in tracknode: "<<t;
                                                        }
                                                        trackNodes[t]->trVectorSection[j].param[ii] = xx;
                                                    }
                                                }
                                            }
                                            ParserX::SkipToken(data);
                                            continue;
                                        }
                                        if(sh == "tritemrefs"){
                                            uu = (int) ParserX::GetNumber(data);
                                            trackNodes[t]->iTri = uu;
                                            trackNodes[t]->trItemRef = new int[uu]; // przydzielenie pamieci dla sciezki
                                            if(uu > 0){
                                                for (j = 0; j < uu; j++) {
                                                    trackNodes[t]->trItemRef[j] = ParserX::GetNumber(data);
                                                }
                                                ParserX::SkipToken(data);
                                            }
                                            ParserX::SkipToken(data);
                                            continue;
                                        }
                                        qDebug() << "#TDB TrVectorNode - undefined token " << sh;
                                        ParserX::SkipToken(data);
                                    }
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if(sh == "trjunctionnode"){
                                    trackNodes[t]->typ = 2; //typ rozjazd
                                    trackNodes[t]->args[0] = ParserX::GetNumber(data);
                                    trackNodes[t]->args[1] = ParserX::GetNumber(data);
                                    trackNodes[t]->args[2] = ParserX::GetNumber(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if(sh == "trpins"){
                                    trackNodes[t]->TrP1 = (int) ParserX::GetNumber(data);
                                    trackNodes[t]->TrP2 = (int) ParserX::GetNumber(data);

                                    for (int i = 0; i < (trackNodes[t]->TrP1 + trackNodes[t]->TrP2); i++) {
                                        trackNodes[t]->TrPinS[i] = (int) ParserX::GetNumber(data);
                                        trackNodes[t]->TrPinK[i] = (int) ParserX::GetNumber(data);
                                    }
                                    ParserX::SkipToken(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                if(sh == "uid"){
                                    for (ii = 0; ii < 12; ii++) {
                                        xx = ParserX::GetNumber(data);
                                        if(std::isnan(xx)){
                                            qDebug() << "#TrackDB: NAN found in tracknode: "<<t;
                                        }
                                        trackNodes[t]->UiD[ii] = xx;
                                    }
                                    ParserX::SkipToken(data);
                                    continue;              
                                }
                                qDebug() << "#TDB TrackNode - undefined token " << sh;
                                //trackNodes[t] = NULL;
                                ParserX::SkipToken(data);
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        qDebug() << "#TDB TrackNodes - undefined token " << sh;
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if(sh == "serial"){
                    this->serial = (int) ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if(sh == "tritemtable"){
                    iTRitems = (int) ParserX::GetNumber(data); //odczytanie ilosci sciezek
                    TRitem* nowy;
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        //qDebug() <<"ssh1 "<< sh;
                        nowy = new TRitem();
                        if(this->road)
                            nowy->tdbId = 1;
                        
                        if(!nowy->init(sh)){
                            qDebug() << "#TDB TrItemTable undefined token " << sh;
                            ParserX::SkipToken(data);
                            continue;
                        }

                        while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                            nowy->set(sh, data);
                            ParserX::SkipToken(data);
                        }

                        this->trackItems[nowy->trItemId] = nowy;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#TDB trackdb undefined token " << sh;
                ParserX::SkipToken(data);
            }
}

void TDB::updateUiDs(QVector<int*> &trackObjUpdates, int startNode){
        for(int i = startNode; i <= iTRnodes; i++){
            TRnode *n = trackNodes[i];
            if(n == NULL)
                continue;
            
            for(int j = 0; j < trackObjUpdates.size(); j++){
                if(n->typ == 2){
                    if(n->UiD[0] == trackObjUpdates[j][0]) 
                        if(n->UiD[1] == -trackObjUpdates[j][1])
                            if(n->UiD[2] == trackObjUpdates[j][2]){
                                n->UiD[0] = trackObjUpdates[j][3];
                                n->UiD[1] = -trackObjUpdates[j][4];
                                n->UiD[2] = trackObjUpdates[j][5];
                    }
                }
                if(n->typ == 1){
                    for(int jj = 0; jj < n->iTrv; jj++){
                        if(n->trVectorSection[jj].param[2] == trackObjUpdates[j][0])
                            if(n->trVectorSection[jj].param[3] == -trackObjUpdates[j][1])
                                if(n->trVectorSection[jj].param[4] == trackObjUpdates[j][2]){
                                    n->trVectorSection[jj].param[2] = trackObjUpdates[j][3];
                                    n->trVectorSection[jj].param[3] = -trackObjUpdates[j][4];
                                    n->trVectorSection[jj].param[4] = trackObjUpdates[j][5];
                        }
                    }
                }
            }
        }
}

void TDB::updateSectionAndShapeIds(QHash<unsigned int,unsigned int>& fixedSectionIds, QHash<unsigned int,unsigned int>& fixedShapeIds){
        for(int i = 1; i <= iTRnodes; i++){
            TRnode *n = trackNodes[i];
            if(n == NULL)
                continue;
            if(n->typ == 2){
                if(fixedShapeIds[n->args[1]] > 0)
                    n->args[1] = fixedShapeIds[n->args[1]];
            }
            if(n->typ == 1)
                for(int j = 0; j < n->iTrv; j++){
                    if(fixedShapeIds[n->trVectorSection[j].param[1]] > 0)
                        n->trVectorSection[j].param[1] = fixedShapeIds[n->trVectorSection[j].param[1]];
                    if(fixedSectionIds[n->trVectorSection[j].param[0]] > 0)
                        n->trVectorSection[j].param[0] = fixedSectionIds[n->trVectorSection[j].param[0]];
                }
        }
}

void TDB::loadTit(){
    QString sh;
    QString extension = "tit";
    if(this->road) extension = "rit";
    QString path = Game::root + "/routes/" + Game::route + "/" + Game::routeName + "." + extension;
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* bufor = ReadFile::read(&file);
    file.close();
    bufor->toUtf16();
    bufor->skipBOM();
    ParserX::NextLine(bufor);
    
    while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
        if(sh == "tritemtable"){
            int iiTRitems = (int) ParserX::GetNumber(bufor); //odczytanie ilosci sciezek
            TRitem* nowy = new TRitem();
            nowy->titLoading = true;
            
            while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                //qDebug() <<"ssh2 "<< sh;
                if(!nowy->init(sh)){
                    qDebug() << "#TIT TrItemTable undefined token " << sh;
                    ParserX::SkipToken(bufor);
                    continue;
                }

                while (!((sh = ParserX::NextTokenInside(bufor).toLower()) == "")) {
                    nowy->set(sh, bufor);
                    ParserX::SkipToken(bufor);
                }
                
                if(this->trackItems[nowy->trItemId] == NULL){
                    qDebug() << "#TIT tdb fail" << nowy->trItemId;
                } else {
                    if(nowy->trSignalDirs > 0){
                        this->trackItems[nowy->trItemId]->trSignalRDir = new float[nowy->trSignalDirs * 6];
                        memcpy(this->trackItems[nowy->trItemId]->trSignalRDir, nowy->trSignalRDir, sizeof(float[nowy->trSignalDirs * 6]));
                    }
                }
                ParserX::SkipToken(bufor);
            }
            ParserX::SkipToken(bufor);
            continue;
        }
        qDebug() << "#TIT undefined token " << sh;
        ParserX::SkipToken(bufor);
    }
    return;
}

void TDB::mergeTDB(TDB *secondTDB, float offsetXYZ[3], unsigned int& trackNodeOffset, unsigned int& trackItemOffset, QHash<unsigned int,unsigned int>& fixedSectionIds, QHash<unsigned int,unsigned int>& fixedShapeIds){
    trackNodeOffset = this->iTRnodes;
    trackItemOffset = this->iTRitems;
    
    // Merge TrackSection file
    if(!road){
        qDebug() << "merge tsc";
        this->tsection->mergeTSection(secondTDB->tsection, fixedSectionIds, fixedShapeIds);
        qDebug() << "update tdb";
        secondTDB->updateSectionAndShapeIds(fixedSectionIds, fixedShapeIds);
    }
    
    // Add new trackNodes
    qDebug() << "merge tdb";
    for(int i = 1; i <= secondTDB->iTRnodes; i++){
        TRnode *n = secondTDB->trackNodes[i];
        if(n == NULL){
            qDebug() << "TRnode NULL" << i;
            continue;
        }
        //qDebug() << "o";
        n->addPositionOffset(offsetXYZ);
        //qDebug() << "i";
        n->addTrackNodeItemOffset(trackNodeOffset, trackItemOffset);
        this->trackNodes[++iTRnodes] = n;
    }
    qDebug() << "new tracknodes" << secondTDB->iTRnodes;
    
    // Add new trackItems
    for(int i = 0; i < secondTDB->iTRitems; i++){
        //qDebug() << i;
        TRitem *n = secondTDB->trackItems[i];
        if(n == NULL){
            qDebug() << "TRitem NULL" << i;
            continue;
        }
        //qDebug() << "addPositionOffset";
        n->addPositionOffset(offsetXYZ);
        //qDebug() << "addTrackNodeItemOffset";
        n->addTrackNodeItemOffset(trackNodeOffset, trackItemOffset);
        this->trackItems[iTRitems++] = n;
    }
    qDebug() << "tdb end";
}

void TDB::checkTrSignalRDirs(){
    for (int i = 0; i <= this->iTRitems; i++) {
        TRitem *it = this->trackItems[i];
        if(it == NULL)
            continue;
        if (it->type != "signalitem") 
            continue;
        if (it->trSignalDirs == 0) 
            continue;
        
        if (it->trSignalDirs > 1){
            qDebug() << "# WARNING - signal dirs more than 1" << i;
        }
        
        if (it->trSignalDir == NULL){ 
            //FAIL, remove link
            qDebug() << "# FAIL - remove link" << i;
            it->trSignalDirs = 0;
            continue;
        }
        
        if (it->trSignalRDir == NULL){
            // Regen trSignalRDir
            qDebug() << "# Regen trSignalRDir " << i;
            int jid = it->trSignalDir[0];
            TRnode *n = trackNodes[jid];
            if(n == NULL){
                //FAIL, remove link
                qDebug() << "# FAIL - remove link" << i;
                it->trSignalDirs = 0;
                it->trSignalDir = NULL;
            }
            if(n->typ == 1){
                //FAIL, remove link
                qDebug() << "# FAIL - remove link" << i;
                it->trSignalDirs = 0;
                it->trSignalDir = NULL;
            }
            
            it->trSignalRDir = new float[it->trSignalDirs * 6];
            it->trSignalRDir[0 + 0] = n->UiD[6];
            it->trSignalRDir[0 + 1] = n->UiD[7];
            it->trSignalRDir[0 + 2] = n->UiD[8];
            it->trSignalRDir[0 + 3] = n->UiD[4];
            it->trSignalRDir[0 + 4] = n->UiD[5];
            it->trSignalRDir[0 + 5] = n->UiD[10];
            
        }
    }

}

int TDB::getNewTRitemId(){
    for (int i = 0; i < this->iTRitems; i++) {
        if(this->trackItems[i] == NULL)
            continue;
        if(Game::useTdbEmptyItems)
            if(this->trackItems[i]->type == "emptyitem"){
                return i;
            }
    }
    return this->iTRitems++;
}

void TDB::fillDynTrack(DynTrackObj* track){
    int tType[5];
    float tAngle[5];
    float tRadius[5];
    int count = 0;
    
    int foundIdx = -1;
    TrackShape::SectionIdx* newRShape = NULL;
    for (int i = 0; i < 5; i++) {
        if(track->sections[i].sectIdx > 1000000) {
            continue;
        }
        tType[count] = track->sections[i].type;
        tAngle[count] = track->sections[i].a;
        tRadius[count] = track->sections[i].r;
        count++;
    }
    
    bool success;
    for(int i = tsection->tsectionShapes; i < tsection->routeShapes; i++){
        success = true;
        if(tsection->shape[i] == NULL) continue;
        
        if(tsection->shape[i]->path[0].n == count){
            for(int j = 0; j<count; j++){
                if(!(tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->type == tType[j])){
                    success = false;
                    break;
                }
                if(tType[j] == 1){
                    if(!(tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->angle == tAngle[j])){
                        success = false;
                        break;
                    }
                    if(!(tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->radius == tRadius[j])){
                        success = false;
                        break;
                    }
                } 
                else if(tType[j] == 0){
                    if(!(tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->size == tAngle[j])){
                        success = false;
                        break;
                    }
                    if(!(tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->val1 == tRadius[j])){
                        success = false;
                        break;
                    }
                }
            }
            ////
            if(success){
                foundIdx = i;
                break;
            }
        }
    }
    qDebug() << "foundIdx "<<foundIdx;
    if(foundIdx == -1){
        newRShape = new TrackShape::SectionIdx[1];
        newRShape->n = count;
        
        for(int j = 0; j<count; j++){
            TSection* newSect0 = new TSection(tsection->routeMaxIdx);
            TSection* newSect1 = new TSection(tsection->routeMaxIdx+1);
            
            if(tType[j] == 1){
                newSect0->type = 1;
                newSect0->angle = -fabs(tAngle[j]);
                newSect0->radius = tRadius[j];
                newSect1->type = 1;
                newSect1->angle = fabs(tAngle[j]);
                newSect1->radius = tRadius[j];
                tsection->sekcja[newSect0->id] = newSect0;
                tsection->sekcja[newSect1->id] = newSect1;
                this->updateTrackSection(newSect0->id);
                this->updateTrackSection(newSect1->id);
                
                if(tAngle[j]<0) 
                    newRShape[0].sect[j] = newSect0->id;
                else 
                    newRShape[0].sect[j] = newSect1->id;
            } 
            else if(tType[j] == 0){
                newSect0->type = 0;
                newSect0->size = tAngle[j];
                newSect0->val1 = tRadius[j];
                
                tsection->sekcja[newSect0->id] = newSect0;
                this->updateTrackSection(newSect0->id);
                newRShape[0].sect[j] = newSect0->id;
            }
            //qDebug() << "sid "<< newSect0->id;
            //qDebug() << "sid "<< newSect1->id;
            tsection->routeMaxIdx+=2;
        }
        foundIdx = tsection->routeShapes;
        TrackShape* newShape = new TrackShape(tsection->routeShapes);
        newShape->dyntrack = true;
        newShape->path = newRShape;
        newRShape->pos[0] = 0;
        newRShape->pos[1] = 0;
        newRShape->pos[2] = 0;
        newShape->numpaths = 1;
        tsection->shape[tsection->routeShapes++] = newShape;
        this->updateTrackShape(newShape->id);
    }
    //qDebug() << "foundIdx "<<foundIdx;
    track->sectionIdx = foundIdx;
    for (int i = 0, j = 0; i < 5; i++) {
        if(track->sections[i].sectIdx > 1000000) {
            continue;
        }
        track->sections[i].sectIdx = tsection->shape[track->sectionIdx]->path[0].sect[j];
        j++;
    }
}

int TDB::findVectorNodeBetweenTwoNodes(int first, int second){
    for(int i = 0; i < trackNodes[first]->TrP1 + trackNodes[first]->TrP2; i++)
        for(int j = 0; j < trackNodes[second]->TrP1 + trackNodes[second]->TrP2; j++){
            if(trackNodes[first]->TrPinS[i] == trackNodes[second]->TrPinS[j])
                return trackNodes[first]->TrPinS[i];
        }
    
    return -1;
}

int TDB::findNearestNode(int &x, int &z, float* p, float* q, float maxD, bool updatePosition) {
    int nearestID = -1;
    float nearestD = 999;
    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = trackNodes[j];
        if(n == NULL) continue;
        if (n->typ == 0 || n->typ == 2) {
            float lenx = ((n->UiD[4] - x)*2048 + n->UiD[6] - p[0]);
            float leny = (n->UiD[7]) - p[1];
            float lenz = ((-n->UiD[5] - z)*2048 - n->UiD[8] - p[2]);
            float dist = fabs(lenx) + fabs(leny) + fabs(lenz);
            if(dist < nearestD && dist < maxD){
                nearestID = j;
                nearestD = dist;
            }
        }
    }
    if ((nearestD < maxD) && updatePosition) {
        //qDebug() << ":"<<len;
        x = trackNodes[nearestID]->UiD[4];
        z = -trackNodes[nearestID]->UiD[5];
        p[0] = trackNodes[nearestID]->UiD[6];
        p[1] = trackNodes[nearestID]->UiD[7];
        p[2] = -trackNodes[nearestID]->UiD[8];

        q[0] = 0;//n->UiD[9]; //fix ??????????
        q[1] = trackNodes[nearestID]->UiD[10];
        q[2] = trackNodes[nearestID]->UiD[11];
                //Quat::rotateY(q, q, n->UiD[10]);
    }
    
    return nearestID;
}

int TDB::appendTrack(int id, int* ends, int r, int sect, int uid) {
    TRnode* endNode = trackNodes[id];
    float p[3];
    if (endNode->typ == 0) {
        int kierunek = endNode->TrPinK[0];
        TRnode* n = trackNodes[endNode->TrPinS[0]];
        if (n->typ != 1) {
            qDebug() << "tdb error";
            return -1;
        }

        qDebug() << kierunek;
        n->iTrv++;
        TRnode::TRSect *newV = new TRnode::TRSect[n->iTrv];

        if (kierunek == 1) {
            std::copy(n->trVectorSection, n->trVectorSection + n->iTrv - 1, newV + 1);
        } else {
            std::copy(n->trVectorSection, n->trVectorSection + n->iTrv - 1, newV);
        }
        delete n->trVectorSection;
        n->trVectorSection = newV;
        //qDebug() <<"sect"<< sect;
        float dlugosc = this->tsection->sekcja[sect]->getDlugosc();
        //qDebug() <<"dlugosc"<< dlugosc;
        Vector3f aa;
        this->tsection->sekcja[sect]->getDrawPosition(&aa, dlugosc);
        aa.rotateX(endNode->UiD[9], 0);  
        aa.rotateY(M_PI + endNode->UiD[10], 0);
        float angle = this->tsection->sekcja[sect]->getAngle();
        int sid = sect;

        p[0] = endNode->UiD[6] + aa.x;
        p[1] = endNode->UiD[7] + aa.y;
        p[2] = endNode->UiD[8] - aa.z;
        int x = endNode->UiD[4];
        int z = endNode->UiD[5];
        int xx = endNode->UiD[4];
        int zz = endNode->UiD[5];
        float pp[3];
        pp[0] = endNode->UiD[6];
        pp[1] = endNode->UiD[7];
        pp[2] = endNode->UiD[8];
        Game::check_coords(x, z, p);

        float vangle = 0;
        float * vector = n->trVectorSection[n->iTrv - 1].param;
        if (kierunek == 1) {
            vector = n->trVectorSection[0].param;
            int tmp = ends[0];
            ends[0] = ends[1];
            ends[1] = tmp;
            if (angle != 0) sid++;
            vangle = angle + M_PI;
            xx = x;
            zz = z;
            pp[0] = p[0];
            pp[1] = p[1];
            pp[2] = p[2];
        }

        vector[0] = sid;
        vector[1] = r;
        vector[2] = endNode->UiD[0];
        vector[3] = endNode->UiD[1];
        vector[4] = uid;
        vector[5] = ends[0];
        vector[6] = ends[1];
        vector[7] = 0;
        vector[8] = xx;
        vector[9] = zz;
        vector[10] = pp[0];
        vector[11] = pp[1];
        vector[12] = pp[2];
        vector[13] = endNode->UiD[9];
        vector[14] = endNode->UiD[10] + vangle;
        vector[15] = endNode->UiD[11];

        //endNode->UiD[0] = endNode->UiD[0];
        //endNode->UiD[1] = endNode->UiD[1];
        endNode->UiD[2] = uid;
        endNode->UiD[3] = ends[1];
        endNode->UiD[4] = x;
        endNode->UiD[5] = z;
        endNode->UiD[6] = p[0];
        endNode->UiD[7] = p[1];
        endNode->UiD[8] = p[2];
        endNode->UiD[9] = endNode->UiD[9];
        endNode->UiD[10] = endNode->UiD[10] + angle;
        endNode->UiD[11] = endNode->UiD[11];
    }
    updateTrNode(id);
    updateTrNode(endNode->TrPinS[0]);
    return id;
}

int TDB::newTrack(int x, int z, float* p, float* qe, int* ends, int r, int sect, int uid) {
    return newTrack(x, z,  p,  qe, ends, r, sect, uid, NULL);
}

int TDB::newTrack(int x, int z, float* p, float* qe, int* ends, int r, int sect, int uid, int* start) {

    //TrackShape* shp = this->tsection->shape[r->value];
    //qDebug() << shp->filename;

    int end1Id = getNextItrNode();//++this->iTRnodes;
    int vecId = getNextItrNode();//++this->iTRnodes;
    int end2Id = getNextItrNode();//++this->iTRnodes;

    int xx = x;
    int zz = z;
    float pp[3];
    pp[0] = p[0]; pp[1] = p[1]; pp[2] = p[2];
    Game::check_coords(xx, zz, pp);
    z = -z;
    zz = -zz;
    ////////////////////////////////////
    this->trackNodes[end1Id] = new TRnode();
    TRnode *newNode = this->trackNodes[end1Id];
    newNode->typ = 0;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = ends[0];
    newNode->UiD[4] = xx;
    newNode->UiD[5] = zz;
    newNode->UiD[6] = pp[0];
    newNode->UiD[7] = pp[1];
    newNode->UiD[8] = -pp[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + M_PI;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 1;

    /////////////////////////////////////////////////////
    this->trackNodes[vecId] = new TRnode();
    newNode = this->trackNodes[vecId];
    qDebug() << vecId;
    newNode->typ = 1;
    newNode->iTrv = 1;
    newNode->trVectorSection = new TRnode::TRSect[newNode->iTrv];
    newNode->trVectorSection[0].param[0] = sect;
    newNode->trVectorSection[0].param[1] = r;
    newNode->trVectorSection[0].param[2] = x;
    newNode->trVectorSection[0].param[3] = z;
    newNode->trVectorSection[0].param[4] = uid;
    newNode->trVectorSection[0].param[5] = ends[0];
    newNode->trVectorSection[0].param[6] = ends[1];
    newNode->trVectorSection[0].param[7] = 0;
    newNode->trVectorSection[0].param[8] = xx;
    newNode->trVectorSection[0].param[9] = zz;
    newNode->trVectorSection[0].param[10] = pp[0];
    newNode->trVectorSection[0].param[11] = pp[1];
    newNode->trVectorSection[0].param[12] = -pp[2];
    newNode->trVectorSection[0].param[13] = qe[0];
    newNode->trVectorSection[0].param[14] = qe[1];
    newNode->trVectorSection[0].param[15] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrP2 = 1;
    newNode->TrPinS[0] = end1Id;
    newNode->TrPinK[0] = 1;
    newNode->TrPinS[1] = end2Id;
    newNode->TrPinK[1] = 1;
    /////////////////////////////////////////////////////
    qDebug() << sect;
    float dlugosc = this->tsection->sekcja[sect]->getDlugosc();
    qDebug() << dlugosc;
    Vector3f aa;
    this->tsection->sekcja[sect]->getDrawPosition(&aa, dlugosc);
    //if(qe[1] > M_PI)
    //    aa->rotateX(-qe[0], 0);
    //else
    
    aa.rotateX(qe[0], 0);  
    aa.rotateY(M_PI + qe[1], 0);
    
    
    float angle = this->tsection->sekcja[sect]->getAngle();
    //Quat::
    //float pp[3];
    pp[0] = pp[0] + aa.x;
    pp[1] = pp[1] + aa.y;
    pp[2] = -pp[2] - aa.z;
    Game::check_coords(xx, zz, pp);

    this->trackNodes[end2Id] = new TRnode();
    newNode = this->trackNodes[end2Id];
    newNode->typ = 0;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = ends[1];
    newNode->UiD[4] = xx;
    newNode->UiD[5] = zz;
    newNode->UiD[6] = pp[0];
    newNode->UiD[7] = pp[1];
    qDebug() << "uid7" << newNode->UiD[7];
    newNode->UiD[8] = pp[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + angle;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 0;
    
    updateTrNode(end1Id);
    updateTrNode(vecId);
    updateTrNode(end2Id);
    
    if(start != NULL)
        *start = end1Id;
    return end2Id;
}

int TDB::newJunction(int x, int z, float* p, float* qe, int r, int uid, int end) {

    //TrackShape* shp = this->tsection->shape[r->value];
    //qDebug() << shp->filename;

    int junction = getNextItrNode();//++this->iTRnodes;
    
    int xx = x;
    int zz = z;
    float pp[3];
    pp[0] = p[0]; pp[1] = p[1]; pp[2] = p[2];
    Game::check_coords(xx, zz, pp);
    z = -z;
    zz = -zz;
    ////////////////////////////////////
    this->trackNodes[junction] = new TRnode();
    TRnode *newNode = this->trackNodes[junction];
    newNode->typ = 2;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = end;
    newNode->UiD[4] = xx;
    newNode->UiD[5] = zz;
    newNode->UiD[6] = pp[0];
    newNode->UiD[7] = pp[1];
    newNode->UiD[8] = -pp[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + M_PI;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrP2 = 2;
    newNode->TrPinS[0] = 0;
    newNode->TrPinK[0] = 0;
    newNode->TrPinS[1] = 0;
    newNode->TrPinK[1] = 0;
    newNode->TrPinS[2] = 0;
    newNode->TrPinK[2] = 0;
    
    newNode->args[1] = r;
    
    return junction;
}

int TDB::joinTracks(int iendp) {
    TRnode* endp = trackNodes[iendp];

    if(endp->typ == 0){
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if(n == NULL) continue;
            if (n->typ == 0) {
                if (j == iendp)
                    continue;
                if (endp->equals(n)) {
                    qDebug() << "polacze " << iendp << " " << j;
                    qDebug() << n->TrPinS[0] << " " << n->TrPinK[0];
                    qDebug() << endp->TrPinS[0] << " " << endp->TrPinK[0];
                    joinVectorSections(endp->TrPinS[0], n->TrPinS[0]);
                    return 0;
                }
            }
        }
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if(n == NULL) continue;
            if (n->typ == 2) {
                if (j == iendp)
                    continue;
                if (endp->equalsIgnoreType(n)) {
                    qDebug() << "polacze rozjazd " << iendp << " " << j;
                    appendToJunction(j, iendp, 0);
                    return 0;
                }
            }
        }
    }

    if(endp->typ == 2){
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if(n == NULL) continue;
            if (n->typ == 0) {
                if (j == iendp)
                    continue;
                if (endp->equalsIgnoreType(n)) {
                    qDebug() << "polacze rozjazd " << iendp << " " << j;
                    appendToJunction(iendp, j, 0);
                    return 0;
                }
            }
        }
    }
    return 0;
}

int TDB::fillJunction(int id){
    
    return 0;
}

int TDB::joinVectorSections(int id1, int id2) {
    TRnode* section1 = trackNodes[id1];
    TRnode* section2 = trackNodes[id2];
    if(section1 == section2)
        return 0;
    int endpk1 = section1->TrPinS[1];
    int endpk2 = section2->TrPinS[0];
    int endpk11 = section1->TrPinS[0];
    int endpk22 = section2->TrPinS[1];
    TRnode* section1e1 = trackNodes[section1->TrPinS[0]];
    TRnode* section1e2 = trackNodes[section1->TrPinS[1]];
    TRnode* section2e1 = trackNodes[section2->TrPinS[0]];
    TRnode* section2e2 = trackNodes[section2->TrPinS[1]];
    
    if (section1e2->equals(section2e1)) {
        qDebug() << "ok";
    }
    else if (section2e2->equals(section1e1)) {
        qDebug() << "switch";
        return joinVectorSections(id2, id1);
    }
    else if (section1e2->equals(section2e2)) {
        qDebug() << "rot2";
        rotate(id2);
        return joinVectorSections(id1, id2);
    }
    else if (section1e1->equals(section2e1)) {
        qDebug() << "rot1";
        rotate(id1);
        return joinVectorSections(id1, id2);
    }

    if(section2->iTri > 0){
        qDebug() << "przeniose " << section2->iTri << " items z " << id2 << " do " << id1;
    }
    moveItemsFrom2to1(id2, id1);
    
    TRnode::TRSect *newV = new TRnode::TRSect[section1->iTrv + section2->iTrv];

    std::copy(section1->trVectorSection, section1->trVectorSection + section1->iTrv, newV);
    std::copy(section2->trVectorSection, section2->trVectorSection + section2->iTrv, newV + section1->iTrv);
    section1->iTrv = section1->iTrv + section2->iTrv;
    
    delete section1->trVectorSection;
    delete section2->trVectorSection;
    
    
    section1->trVectorSection = newV;
    section1->TrPinS[1] = section2->TrPinS[1];
    section1->TrPinK[1] = section2->TrPinK[1];
    section2e2->podmienTrPin(id2, id1);
    //section2e2->TrPinS[0] = section1e2->TrPinS[0];
    
    trackNodes[id2] = NULL;
    trackNodes[endpk1] = NULL;
    trackNodes[endpk2] = NULL;
    
    updateTrNode(id1);
    updateTrNode(id2);
    updateTrNode(endpk1);
    updateTrNode(endpk2);
    updateTrNode(endpk11);
    updateTrNode(endpk22);
    return 0;
}

void TDB::moveItemsFrom2to1(int id2, int id1){
    TRnode* section1 = trackNodes[id1];
    TRnode* section2 = trackNodes[id2];
    if(section1 == section2)
        return;
    
    // join arrays
    int iTri = section1->iTri + section2->iTri;
    int* array = new int[iTri];
    std::copy(section1->trItemRef, section1->trItemRef + section1->iTri, array);
    std::copy(section2->trItemRef, section2->trItemRef + section2->iTri, array + section1->iTri);
    
    // update items
    float d = getVectorSectionLength(id1);
    qDebug() << "d: " << d;
    TRitem* trit;
    for(int i = 0; i < section2->iTri; i++){
        trit = this->trackItems[section2->trItemRef[i]];
        if(trit == NULL){
            qDebug() << "NULL Item: " << i << " " << section2->trItemRef[i];
        } else {
            trit->addToTrackPos(d);
        }
    }
    
    section1->trItemRef = array;
    section1->iTri = iTri;
}

float TDB::getVectorSectionLength(int id){
    TRnode* n = trackNodes[id];
    float dlugosc = 0;
    TSection* sect;
    for (int i = 0; i < n->iTrv; i++) {
        sect = tsection->sekcja[(int)n->trVectorSection[i].param[0]];
        if(sect != NULL)
            dlugosc += sect->getDlugosc();
    }
    return dlugosc;
}

float TDB::getVectorSectionLengthToIdx(int id, int idx){
    TRnode* n = trackNodes[id];
    float dlugosc = 0;
    TSection* sect;
    for (int i = 0; i < idx; i++) {
        sect = tsection->sekcja[(int)n->trVectorSection[i].param[0]];
        if(sect != NULL)
            dlugosc += sect->getDlugosc();
    }
    return dlugosc;
}

int TDB::splitVectorSection(int id, int j){
    TRnode* vect = trackNodes[id];
    TRnode* end2 = trackNodes[vect->TrPinS[1]];
    TRnode* newNode;
    
    int end1Id = getNextItrNode();//++this->iTRnodes;
    int vecId = getNextItrNode();//++this->iTRnodes;
    int end2Id = getNextItrNode();//++this->iTRnodes;
    end2->podmienTrPin(id, vecId);
    
    updateTrNode(vect->TrPinS[1]);
    
    this->trackNodes[vecId] = new TRnode();
    newNode = this->trackNodes[vecId];
    newNode->typ = 1;
    newNode->iTrv = vect->iTrv - j;
    
    // slpit items
    if(vect->iTri > 0){
        float vectDlugosc = this->getVectorSectionLengthToIdx(id, j);
        // calculate
        int ivecItems = 0;
        int inewItems = 0;
        TRitem* trit; 
        for(int i = 0; i < vect->iTri; i++){
            trit = this->trackItems[vect->trItemRef[i]];
            if(trit == NULL) continue;
            if(trit->getTrackPosition() < vectDlugosc) ivecItems++;
            else inewItems++;
        }
        int* vecItems = new int[ivecItems];
        int* newItems = new int[inewItems];
        // fill
        ivecItems = 0;
        inewItems = 0;
        for(int i = 0; i < vect->iTri; i++){
            trit = this->trackItems[vect->trItemRef[i]];
            if(trit == NULL) 
                continue;
            if(trit->getTrackPosition() < vectDlugosc) 
                vecItems[ivecItems++] = vect->trItemRef[i];
            else {
                trit->trackPositionAdd(-vectDlugosc);
                newItems[inewItems++] = vect->trItemRef[i];
            }
        }
        vect->iTri = ivecItems;
        newNode->iTri = inewItems;
        delete[] vect->trItemRef;
        vect->trItemRef = vecItems;
        newNode->trItemRef = newItems;
    }
    
    TRnode::TRSect *newV = new TRnode::TRSect[newNode->iTrv];
    std::copy(vect->trVectorSection + j, vect->trVectorSection + vect->iTrv, newV);
    newNode->trVectorSection = newV;
    
    newNode->TrP1 = 1;
    newNode->TrP2 = 1;
    newNode->TrPinS[0] = end2Id;
    newNode->TrPinK[0] = 1;
    newNode->TrPinS[1] = vect->TrPinS[1];
    newNode->TrPinK[1] = vect->TrPinK[1];
    
    newV = new TRnode::TRSect[j];
    std::copy(vect->trVectorSection, vect->trVectorSection + j, newV);
    
    vect->iTrv = j;
    vect->TrPinS[1] = end1Id;
    vect->TrPinK[1] = 1;
    
    
    ////////////////////
    this->trackNodes[end1Id] = new TRnode();
    newNode = this->trackNodes[end1Id];
    newNode->typ = 0;
    newNode->UiD[0] = vect->trVectorSection[j-1].param[2];
    newNode->UiD[1] = vect->trVectorSection[j-1].param[3];
    newNode->UiD[2] = vect->trVectorSection[j-1].param[4];
    newNode->UiD[3] = vect->trVectorSection[j-1].param[6];
    newNode->UiD[4] = vect->trVectorSection[j].param[8];
    newNode->UiD[5] = vect->trVectorSection[j].param[9];
    newNode->UiD[6] = vect->trVectorSection[j].param[10];
    newNode->UiD[7] = vect->trVectorSection[j].param[11];
    newNode->UiD[8] = vect->trVectorSection[j].param[12];
    newNode->UiD[9] = vect->trVectorSection[j].param[13];
    newNode->UiD[10] = vect->trVectorSection[j].param[14];
    newNode->UiD[11] = vect->trVectorSection[j].param[15];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = id;
    newNode->TrPinK[0] = 0;
    /////////////////
    this->trackNodes[end2Id] = new TRnode();
    newNode = this->trackNodes[end2Id];
    newNode->typ = 0;
    newNode->UiD[0] = vect->trVectorSection[j].param[2];
    newNode->UiD[1] = vect->trVectorSection[j].param[3];
    newNode->UiD[2] = vect->trVectorSection[j].param[4];
    newNode->UiD[3] = vect->trVectorSection[j].param[5];
    newNode->UiD[4] = vect->trVectorSection[j].param[8];
    newNode->UiD[5] = vect->trVectorSection[j].param[9];
    newNode->UiD[6] = vect->trVectorSection[j].param[10];
    newNode->UiD[7] = vect->trVectorSection[j].param[11];
    newNode->UiD[8] = vect->trVectorSection[j].param[12];
    newNode->UiD[9] = vect->trVectorSection[j].param[13];
    newNode->UiD[10] = vect->trVectorSection[j].param[14] + M_PI;
    newNode->UiD[11] = vect->trVectorSection[j].param[15];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 1;
    
    delete vect->trVectorSection;
    vect->trVectorSection = newV;
    
    updateTrNode(id);
    updateTrNode(end1Id);
    updateTrNode(end2Id);
    updateTrNode(vecId);
    return vecId;
}

void TDB::deleteJunction(int id){
    TRnode* junction = trackNodes[id];
    if(junction->typ != 2) 
        return;
    
    int count = 0;
    int vecId = 0;
    for(int i = 0; i < 3; i++){
        qDebug() << junction->TrPinS[i];
        if(junction->TrPinS[i] != 0) count++;
    }
    qDebug() << count;
    if(count > 1){
        qDebug() << "junction delete fail";
        return;
    }
    if(count == 0){
        delete trackNodes[id];
        trackNodes[id] = NULL;
        updateTrNode(id);
        return;
    }
    if(count == 1){
        vecId = 0;
        for(int i = 0; i < 3; i++){
            if(junction->TrPinS[i] != 0){
                junction->TrPinS[0] = junction->TrPinS[i];
                junction->TrPinK[0] = junction->TrPinK[i];
                vecId = junction->TrPinS[i];
                break;
            }
        }
        TRnode* vect = trackNodes[vecId];
        
        if(!vect->isLikedTo(id)){
            qDebug() << "FAIL, TrackNode not linked to this juction!";
            trackNodes[id] = NULL;
            updateTrNode(id);
            return;
        }
        
        vect->setTrPinK(id, 1);

        if(junction->TrPinK[0] == 1){
            junction->UiD[2] = vect->trVectorSection[0].param[4];
            junction->UiD[3] = vect->trVectorSection[0].param[5];
        } else if(junction->TrPinK[0] == 0){
            junction->UiD[2] = vect->trVectorSection[vect->iTrv-1].param[4];
            junction->UiD[3] = vect->trVectorSection[vect->iTrv-1].param[6];
        }
        junction->UiD[10] += M_PI;
        
        junction->typ = 0;
        junction->TrP1 = 1;
        junction->TrP2 = 0;
        updateTrNode(vecId);
        updateTrNode(id);
    }
}

void TDB::deleteVectorSection(int id){
    TRnode* vect = trackNodes[id];
    TRnode* end1 = trackNodes[vect->TrPinS[0]];
    TRnode* end2 = trackNodes[vect->TrPinS[1]];
    
    deleteAllTrItemsFromVectorSection(id);
    
    delete trackNodes[id];
    trackNodes[id] = NULL;
    
    if(end1->typ == 0){
        delete trackNodes[vect->TrPinS[0]];
        trackNodes[vect->TrPinS[0]] = NULL;
    } else if (end1->typ == 2) {
        end1->podmienTrPin(id, 0);
        end1->setTrPinK(0, 0);
    } 
    
    if(end2->typ == 0){
        delete trackNodes[vect->TrPinS[1]];
        trackNodes[vect->TrPinS[1]] = NULL;
    } else if (end2->typ == 2) {
        end2->podmienTrPin(id, 0);
        end2->setTrPinK(0, 0);
    }
    
    updateTrNode(id);
    updateTrNode(vect->TrPinS[0]);
    updateTrNode(vect->TrPinS[1]);
    
}

bool TDB::deleteAllTrItemsFromVectorSection(int id){
    TRnode* vect = trackNodes[id];
    if(vect->iTri > 0){
        TRitem* trit;
        for(int i = 0; i < vect->iTri; i++){
            qDebug() << vect->trItemRef[i];
            trit = this->trackItems[vect->trItemRef[i]];
            if(trit == NULL){
                continue;
            }
            // item delete
            qDebug() << "item delete " << vect->trItemRef[i]<< " "<<trit->trItemId;
            deleteTrItem(trit->trItemId);
            i--;
        }
    }
    vect->iTri = 0;
    return true;
}

bool TDB::deleteFromVectorSection(int id, int j){
    TRnode* vect = trackNodes[id];
    if(vect->iTrv == 1){
        deleteVectorSection(id);
        return false;
    }
    int vid = -1;
    if(j > 0 && j < vect->iTrv - 1){
        vid = splitVectorSection(id, j);
        vect = trackNodes[vid];
        j = 0;
    }
    int endNId1 = vect->TrPinS[0];
    int endNId2 = vect->TrPinS[1];
    TRnode* end1 = trackNodes[vect->TrPinS[0]];
    TRnode* end2 = trackNodes[vect->TrPinS[1]];
    //deleteAllTrItemsFromVectorSection(id);
    TRnode::TRSect *newV = new TRnode::TRSect[vect->iTrv - 1];
    if(j == 0){
        // move & check items
        if(vect->iTri > 0){
            float sectDlugosc = this->tsection->sekcja[vect->trVectorSection[0].param[0]]->getDlugosc();
            TRitem* trit;
            for(int i = 0; i < vect->iTri; i++){
                trit = this->trackItems[vect->trItemRef[i]];
                if(trit == NULL) 
                    continue;
                trit->addToTrackPos(-sectDlugosc);
                if(trit->getTrackPosition() < 0){
                    qDebug() << "delete item? - before section";
                    // item delete
                    qDebug() << "item delete " << trit->trItemId;
                    this->deleteTrItem(trit->trItemId);
                    i--;
                }
                updateTrItem(vect->trItemRef[i]);
            }
        }
        
        std::copy(vect->trVectorSection + 1, vect->trVectorSection + vect->iTrv, newV);
        
        if(end1->typ == 2){
            end1->podmienTrPin(id, 0);
            end1->setTrPinK(0, 0);
            updateTrNode(endNId1);
            
            endNId1 = getNextItrNode();//++this->iTRnodes;
            this->trackNodes[endNId1] = new TRnode();
            end1 = trackNodes[endNId1];
            end1->typ = 0;
            end1->TrP1 = 1;
            end1->TrPinS[0] = id;
            end1->TrPinK[0] = 1;
            vect->TrPinS[0] = endNId1;
            vect->TrPinK[0] = 1;
        }
        
            end1->UiD[0] = vect->trVectorSection[1].param[2];
            end1->UiD[1] = vect->trVectorSection[1].param[3];
            end1->UiD[2] = vect->trVectorSection[1].param[4];
            end1->UiD[3] = vect->trVectorSection[1].param[5];
            end1->UiD[4] = vect->trVectorSection[1].param[8];
            end1->UiD[5] = vect->trVectorSection[1].param[9];
            end1->UiD[6] = vect->trVectorSection[1].param[10];
            end1->UiD[7] = vect->trVectorSection[1].param[11];
            end1->UiD[8] = vect->trVectorSection[1].param[12];
            end1->UiD[9] = vect->trVectorSection[1].param[13];
            end1->UiD[10] = vect->trVectorSection[1].param[14] + M_PI;
            end1->UiD[11] = vect->trVectorSection[1].param[15];
            updateTrNode(endNId1);
            
            
    } else if(j == vect->iTrv - 1) {
        // check items
        if(vect->iTri > 0){
            float vectDlugosc = this->getVectorSectionLengthToIdx(id, j);
            TRitem* trit;
            for(int i = 0; i < vect->iTri; i++){
                trit = this->trackItems[vect->trItemRef[i]];
                if(trit == NULL) 
                    continue;
                if(trit->getTrackPosition() > vectDlugosc){
                    qDebug() << "delete item? - behind section";
                    // item delete
                    qDebug() << "item delete " << trit->trItemId;
                    this->deleteTrItem(trit->trItemId);
                    i--;
                }
            }
        }
        
        std::copy(vect->trVectorSection , vect->trVectorSection + vect->iTrv - 1, newV);
        if(end2->typ == 2){
            end2->podmienTrPin(id, 0);
            end2->setTrPinK(0, 0);
            updateTrNode(endNId2);
            
            endNId2 = getNextItrNode();//++this->iTRnodes;
            this->trackNodes[endNId2] = new TRnode();
            end2 = trackNodes[endNId2];
            end2->typ = 0;
            end2->TrP1 = 1;
            end2->TrPinS[0] = id;
            end2->TrPinK[0] = 0;
            vect->TrPinS[1] = endNId2;
            vect->TrPinK[1] = 1;
        }
            end2->UiD[0] = vect->trVectorSection[vect->iTrv-2].param[2];
            end2->UiD[1] = vect->trVectorSection[vect->iTrv-2].param[3];
            end2->UiD[2] = vect->trVectorSection[vect->iTrv-2].param[4];
            end2->UiD[3] = vect->trVectorSection[vect->iTrv-2].param[6];
            end2->UiD[4] = vect->trVectorSection[vect->iTrv-1].param[8];
            end2->UiD[5] = vect->trVectorSection[vect->iTrv-1].param[9];
            end2->UiD[6] = vect->trVectorSection[vect->iTrv-1].param[10];
            end2->UiD[7] = vect->trVectorSection[vect->iTrv-1].param[11];
            end2->UiD[8] = vect->trVectorSection[vect->iTrv-1].param[12];
            end2->UiD[9] = vect->trVectorSection[vect->iTrv-1].param[13];
            end2->UiD[10] = vect->trVectorSection[vect->iTrv-1].param[14];
            end2->UiD[11] = vect->trVectorSection[vect->iTrv-1].param[15];
            updateTrNode(endNId2);
    }
    
    vect->iTrv -= 1;
    delete vect->trVectorSection;
    vect->trVectorSection = newV;
    updateTrNode(id);
    if(vid >= 0)
        updateTrNode(vid);
    
    return true;
}

int TDB::rotate(int id){
    TRnode* vect = trackNodes[id];
    TRnode* e1 = trackNodes[vect->TrPinS[0]];
    TRnode* e2 = trackNodes[vect->TrPinS[1]];
    
    for(int i = 0; i < vect->iTrv; i++){
        if(i < vect->iTrv - 1){
            vect->trVectorSection[i].param[8] = vect->trVectorSection[i+1].param[8];
            vect->trVectorSection[i].param[9] = vect->trVectorSection[i+1].param[9];
            vect->trVectorSection[i].param[10] = vect->trVectorSection[i+1].param[10];
            vect->trVectorSection[i].param[11] = vect->trVectorSection[i+1].param[11];
            vect->trVectorSection[i].param[12] = vect->trVectorSection[i+1].param[12];
            vect->trVectorSection[i].param[13] = -vect->trVectorSection[i].param[13]; //fix ????
            vect->trVectorSection[i].param[14] = vect->trVectorSection[i+1].param[14];
            vect->trVectorSection[i].param[15] = vect->trVectorSection[i+1].param[15];
        } else {
            vect->trVectorSection[i].param[8] = e2->UiD[4];
            vect->trVectorSection[i].param[9] = e2->UiD[5];
            vect->trVectorSection[i].param[10] = e2->UiD[6];
            vect->trVectorSection[i].param[11] = e2->UiD[7];
            vect->trVectorSection[i].param[12] = e2->UiD[8];
            vect->trVectorSection[i].param[13] = -vect->trVectorSection[i].param[13];
            vect->trVectorSection[i].param[14] = e2->UiD[10];
            vect->trVectorSection[i].param[15] = e2->UiD[11];
        }
        float angle = this->tsection->sekcja[vect->trVectorSection[i].param[0]]->getAngle();
        if (angle > 0) vect->trVectorSection[i].param[0]--;
        if (angle < 0) vect->trVectorSection[i].param[0]++;
        
        int tmp = vect->trVectorSection[i].param[5];
        vect->trVectorSection[i].param[5] = vect->trVectorSection[i].param[6];
        vect->trVectorSection[i].param[6] = tmp;
        
        vect->trVectorSection[i].param[14] += M_PI;
        if(vect->trVectorSection[i].param[14] > 2*M_PI)
            vect->trVectorSection[i].param[14] -= 2*M_PI;
    }
    for(int i = 0; i < vect->iTrv/2; i++){
        TRnode::TRSect t = vect->trVectorSection[i];
        vect->trVectorSection[i] = vect->trVectorSection[vect->iTrv - 1 - i];
        vect->trVectorSection[vect->iTrv - 1 - i] = t;
    }
    
    e1->setTrPinK(id, 0);
    e2->setTrPinK(id, 1);
    /*e1->UiD[10] += M_PI;
    if(e1->UiD[10] > 2*M_PI)
        e1->UiD[10] -= 2*M_PI;
    e2->UiD[10] += M_PI;
    if(e2->UiD[10] > 2*M_PI)
        e2->UiD[10] -= 2*M_PI;*/
    
    int tmp = vect->TrPinS[0];
    vect->TrPinS[0] = vect->TrPinS[1];
    vect->TrPinS[1] = tmp;
    tmp = vect->TrPinK[0];
    vect->TrPinK[0] = vect->TrPinK[1];
    vect->TrPinK[1] = tmp;
    
    // update items
    float d = getVectorSectionLength(id);
    qDebug() << "d: " << d;
    for(int i = 0; i < vect->iTri; i++){
        this->trackItems[vect->trItemRef[i]]->flipTrackPos(d);
    }
    return 0;
}

int TDB::appendToJunction(int junctionId, int eId, int idx){
    TRnode* junction = trackNodes[junctionId];

    if(idx == 1){
        if(junction->TrPinS[idx] != 0)
            idx++;
        if(junction->TrPinS[idx] != 0)
            return 0;
    } else {
        if(junction->TrPinS[idx] != 0)
            return 0;
    }
    
    TRnode* e1 = trackNodes[eId];
    int trackId = e1->TrPinS[0];
    TRnode* track = trackNodes[trackId];
    trackNodes[eId] = NULL;
    
    junction->TrPinS[idx] = trackId;
    
    
    int j = track->podmienTrPin(eId, junctionId);
    //track->TrPinS[0] = junctionId;
    if(idx == 0) {
        track->TrPinK[j] = 1;
        //junction->TrPinK[idx] = 0;
        junction->TrPinK[idx] = e1->TrPinK[0];
    } else {
        track->TrPinK[j] = 0;
        //junction->TrPinK[idx] = 1;
        junction->TrPinK[idx] = e1->TrPinK[0];
    }
    updateTrNode(trackId);
    updateTrNode(junctionId);
    updateTrNode(eId);
    return 0;
}

void TDB::setDefaultEnd(int val){
    this->defaultEnd = val;
}

void TDB::nextDefaultEnd(){
    this->defaultEnd++;
}

bool TDB::findPosition(int &x, int &z, float* p, float* q, float* endp, int sectionIdx){
    float qe[3];
    qe[0] = 0;
    qe[1] = 0;
    qe[2] = 0;
    int findValue = findNearestNode(x, z, p, (float*) &qe);
    if(findValue < 0) return false;
    qDebug() << findValue;
    
    bool b;
    
    if(sectionIdx < 0){
        Quat::fill(q);
        Quat::rotateY(q, q, -qe[1]);
        return true;
    }
    
    TrackShape* shp = this->tsection->shape[sectionIdx];
    qDebug() << shp->filename;
    float startPos[3];
     
    while(defaultEnd >= shp->numpaths*2){
        defaultEnd -= shp->numpaths*2;
    }
    
    qDebug() << "defaultEnd" << defaultEnd;
    
    int startEnd = defaultEnd/2;
    int endend = defaultEnd - (startEnd)*2;
    
    Vector3f aa;
    Vector3f aa3;
    Vector3f bb;
    Vector3f aa2;
    
    if(endend == 1){
        float angle = -qe[1];
        float angle2 = 0;
        float dlugosc = 0;
        for (int i = shp->path[startEnd].n - 1; i >= 0; i--) {
        //for (int i = 0; i < shp->path[startEnd].n; i++) {
            dlugosc = this->tsection->sekcja[shp->path[startEnd].sect[i]]->getDlugosc();
            //qDebug() << dlugosc;
            this->tsection->sekcja[shp->path[startEnd].sect[i]]->getDrawPosition(&aa2, dlugosc);
            aa2.rotateY(angle, 0);
            //qDebug() << "aa " << aa2->x << " "<<aa2->z;
            aa.x+=aa2.x;
            aa.z+=aa2.z;
            angle += this->tsection->sekcja[shp->path[startEnd].sect[i]]->getAngle();
            this->tsection->sekcja[shp->path[startEnd].sect[i]]->getDrawPosition(&aa2, dlugosc);
            aa2.rotateY(angle2, 0);
            aa3.x+=aa2.x;
            aa3.z+=aa2.z;
            angle2 += this->tsection->sekcja[shp->path[startEnd].sect[i]]->getAngle();
        }
        endp[0] = -aa3.x;
        endp[1] = 0;
        endp[2] = -aa3.z;
        //aa.rotateY(-qe[1], 0);
        startPos[0] = aa.x;
        startPos[2] = aa.z;
        qe[1] -= angle + qe[1] - M_PI;
        endp[4] = -qe[1] + shp->path[startEnd].rotDeg*M_PI/180;
    } else {
        endp[0] = aa.x;
        endp[1] = 0;
        endp[2] = aa.z;
        endp[4] = 0;
        startPos[0] = aa.x;
        startPos[2] = aa.z;
    }
    //qDebug() << "sp " << startPos[0]  << " "<< startPos[2];
    
    bb.x = shp->path[0].pos[0];
    bb.z = shp->path[0].pos[2];
    bb.rotateY(-qe[1], 0);
    //aa.set(shp->path[0].pos[0] - shp->path[startEnd].pos[0], shp->path[0].pos[1], shp->path[0].pos[2] - shp->path[startEnd].pos[2]);
    aa.set(0 - shp->path[startEnd].pos[0], shp->path[0].pos[1], 0 - shp->path[startEnd].pos[2]);
    aa.rotateY(shp->path[startEnd].rotDeg*M_PI/180, 0);
    aa.x += shp->path[0].pos[0];
    aa.z += shp->path[0].pos[2];
    aa.rotateY(-qe[1], 0);
    p[0] = p[0] + aa.x + startPos[0];
    p[1] = p[1] - shp->path[0].pos[1];
    p[2] = p[2] - aa.z - startPos[2];
    p[0] -= bb.x;
    p[2] += bb.z;
    
    Quat::fill(q);
    Quat::rotateY(q, q, -qe[1] + shp->path[startEnd].rotDeg*M_PI/180);
    
    if(endend == 0)
        endp[3] = 1;
    if(endend == 1)
        endp[3] = -1;
    
    qDebug() << "ccc";
    qDebug() << startPos[0] << " " << startPos[2];
    
    return true;
}

bool TDB::fillJNodePosn(int x, int z, int uid, QVector<std::array<float, 5>> *jNodePosn){
    if(jNodePosn == NULL)
        return false;
    jNodePosn->clear();
    z = -z;
    qDebug() << "fill jnodeposn " << x << " " << z << " " << uid; 
    
    TRnode *n;
    int count = 0;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 2) {
                if(n->UiD[0] == x)
                    if(n->UiD[1] == z)
                        if(n->UiD[2] == uid){
                            qDebug() << "jest j";
                            count++;
                            jNodePosn->push_back(std::array<float,5>());
                            jNodePosn->back()[0] = n->UiD[0];
                            jNodePosn->back()[1] = n->UiD[1];
                            jNodePosn->back()[2] = n->UiD[6];
                            jNodePosn->back()[3] = n->UiD[7];
                            jNodePosn->back()[4] = n->UiD[8];
                }
            }
        }
    if(count > 0) return true;
    return false;
}

bool TDB::placeTrack(int x, int z, float* p, float* q, int sectionIdx, int uid, QVector<std::array<float, 5>> *jNodePosn) {
    float qe[4];
    float vect[3];
    vect[0] = 0; vect[1] = 0; vect [2] = 10;
    Vec3::transformQuat(vect, vect, q);
    
    //float roll = atan2((2*(q[0]*q[1] + q[2]*q[3])),(1-((q[0]*q[0])+(q[1]*q[1]))));
    float sinv = 2*(q[0]*q[2] - q[1]*q[3]);
    if(sinv > 1.0f)
        sinv = 1.0f;
    if(sinv < -1.0f)
        sinv = -1.0f;
    float pitch = asin(sinv);
    //float yaw = atan2((2*(q[0]*q[3] + q[1]*q[2])),(1-((q[2]*q[2])+(q[3]*q[3]))));
    
    if(vect[2] < 0)
        pitch = M_PI - pitch;
    if(vect[2] == 0 && vect[0] < 0)
        pitch = M_PI/2;
    if(vect[2] == 0 && vect[0] > 0)
        pitch = -M_PI/2;

    sinv = (vect[1]/10.0);
    if(sinv > 1.0f)
        sinv = 1.0f;
    if(sinv < -1.0f)
        sinv = -1.0f;
    qe[0] = asin(sinv);
    qe[1] = pitch;
    qe[2] = 0;
    
    TrackShape* shp = this->tsection->shape[sectionIdx];
    qDebug() << shp->filename;
    float pp[3];
    float qee[3];
    int endp;
    Vector3f aa;
    int start;
    int ends[2];
    
    ////////////////////////////////
    
    int *endsNumbres = new int[shp->numpaths*2];
    std::unordered_map<int, int> endsIds;
    std::unordered_map<int, int> isJunction;
    std::unordered_map<int, int> junctionId;
    int nextNumber = 0;
    
    for (int i = 0; i < shp->numpaths; i++) {
        int posIdx = (float)shp->path[i].pos[0]*100000 + (float)shp->path[i].pos[1]*1000 + (float)shp->path[i].pos[2]*10;
        std::unordered_map<int, int>::iterator iter = endsIds.find(posIdx);
        if(iter == endsIds.end()){
            endsNumbres[i*2] = nextNumber++;
            endsIds[posIdx] = endsNumbres[i*2];
            isJunction[endsNumbres[i*2]] = 0;
        } else {
            endsNumbres[i*2] = endsIds[posIdx];
            isJunction[endsNumbres[i*2]] = 1;
            //qDebug() << "rozjazd";
            //junctions[junctionCount++] = newJunction(x, z, pp, qee, r->value, uid, ends[0]);
        }
        junctionId[endsNumbres[i*2]] = 0;
        endsNumbres[i*2+1] = nextNumber++;
        //qDebug() << "ends: "<< ends[0] <<" "<<ends[1];
    }
    
    ////////////////////////////////

    for (int i = 0; i < shp->numpaths; i++) {
        aa.set(shp->path[i].pos[0], -shp->path[i].pos[1], shp->path[i].pos[2]);
        //aa.rotateY(-qe[1] + shp->path[i].rotDeg*M_PI/180 - shp->path[startEnd].rotDeg*M_PI/180, 0);
        aa.rotateX(-qe[0], 0);
        aa.rotateY(-qe[1], 0);

        pp[0] = p[0] + aa.x;
        pp[1] = p[1] - aa.y;// po[1];//shp->path[i].pos[1];
        pp[2] = p[2] - aa.z;
        if(fabs(shp->path[i].rotDeg) > 90)
            qee[0] = -qe[0];
        else
            qee[0] = qe[0];
        qee[1] = qe[1] + shp->path[i].rotDeg*M_PI/180;
        qee[2] = qe[2];
        
        ends[0] = endsNumbres[i*2];
        ends[1] = endsNumbres[i*2+1];
        
        if(isJunction[ends[0]] == 1){
            isJunction[ends[0]] = 0;
            qDebug() << "rozjazd" << jNodePosn;
            if(jNodePosn != NULL){
                jNodePosn->push_back(std::array<float,5>());
                jNodePosn->back()[0] = x;
                jNodePosn->back()[1] = -z;
                jNodePosn->back()[2] = pp[0];
                jNodePosn->back()[3] = pp[1];
                jNodePosn->back()[4] = -pp[2];
            }
            junctionId[ends[0]] = newJunction(x, z, pp, qee, sectionIdx, uid, ends[0]);
        }

        endp = newTrack(x, z, pp, qee, (int*)ends, sectionIdx, shp->path[i].sect[0], uid, &start);

        for (int j = 1; j < shp->path[i].n; j++) {
            if (endp > 0) {
                endp = appendTrack(endp, (int*)ends, sectionIdx, shp->path[i].sect[j], uid);
            }
        }
        
        if(junctionId[ends[0]] != 0){
            qDebug() << "append to junction";
            appendToJunction(junctionId[ends[0]], start, 1);
            joinTracks(junctionId[ends[0]]);
        } else {
            joinTracks(start);
        }

        joinTracks(endp);
    }
    
    ////////////////////////////////
    
    if(shp->crossovershape){
        QVector<TDB::IntersectionPoint> cPoints;
        float posT[2];
        Vec2::set(posT, x, z);
        Vector3f cPos;
        for(int i = 0; i < shp->xoverpts; i++){
            cPos.set((float*)&shp->xoverpt[i*3]);
            cPos.rotateY(-qe[1], 0);
            cPos.z = - cPos.z;
            cPos.add(p);
            
            this->findNearestPositionsOnTDB(posT, (float*)&cPos, cPoints, 0.2);
            qDebug() << "crossover";
            qDebug() << cPos.x<<cPos.y<<cPos.z;
            if(cPoints.size() != 2)
                continue;
            //qDebug() << cPoints.size();
            qDebug() << cPoints[0].idx << cPoints[0].m << cPoints[1].idx << cPoints[1].m;
            this->newCrossOverObject(cPoints[0].idx, cPoints[0].m, cPoints[1].idx, cPoints[1].m, sectionIdx);
        }
    }

    refresh();
    return true;
}

bool TDB::removeTrackFromTDB(int x, int y, int UiD){
    y = -y;
    qDebug() << "usune Track " << x << " " << y << " " << UiD; 
    
    bool ok = false;
    TRnode *n;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 1) {
                for(int j = 0; j < n->iTrv; j++)
                    if(n->trVectorSection[j].param[2] == x)
                        if(n->trVectorSection[j].param[3] == y)
                            if(n->trVectorSection[j].param[4] == UiD){
                                ok = true;
                                qDebug() << "jest";
                                if(deleteFromVectorSection(i, j))
                                    j = -1;
                    }
            }
        }
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 2) {
                if(n->UiD[0] == x)
                    if(n->UiD[1] == y)
                        if(n->UiD[2] == UiD){
                            ok = true;
                            qDebug() << "jest j";
                            deleteJunction(i);
                }
            }
        }
    
    if(ok)
        TDB::refresh();
    return ok;
}

void TDB::fillTrackAngles(int x, int z, int UiD, QMap<int, float>& angles){
    TRnode *n;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 1) {
                for(int j = 0; j < n->iTrv; j++)
                    if(n->trVectorSection[j].param[2] == x)
                        if(n->trVectorSection[j].param[3] == z)
                            if(n->trVectorSection[j].param[4] == UiD){
                                int endp1 = n->trVectorSection[j].param[5];
                                int endp2 = n->trVectorSection[j].param[6];
                                angles[endp1] = fabs(n->trVectorSection[j].param[15]);
                                if(j < n->iTrv - 1)
                                    angles[endp2] = fabs(n->trVectorSection[j+1].param[15]);
                                else
                                    angles[endp2] = 0;
                            
                    }
            }
        }
}

bool TDB::ifTrackExist(int x, int y, int UiD){
    y = -y;
    //qDebug() << "is Track? " << x << " " << y << " " << UiD; 
    
    TRnode *n;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 1) {
                for(int j = 0; j < n->iTrv; j++)
                    if(n->trVectorSection[j].param[2] == x)
                        if(n->trVectorSection[j].param[3] == y)
                            if(n->trVectorSection[j].param[4] == UiD){
                                qDebug() << "jest";
                                return true;
                    }
            }
        }
    return false;
}

int TDB::getNextItrNode(){
    return ++this->iTRnodes;
}

void TDB::refresh() {
    isInitSectLines = false;
    isInitLines = false;
    isInitTrItemsDraw = false;
    collisionLineHash = 0;
}

void TDB::renderAll(GLUU *gluu, float* playerT, float playerRot) {

    if (!loaded) return;
    int hash = (int)playerT[0] * 10000 + (int)playerT[1];
    if (!isInitLines || lineHash != hash) {
        TRnode *n;
        int lLen = 0, kLen = 0, pLen = 0;
        //qDebug() <<"update";
        lineHash = hash;
        isInitLines = true;
        
        for (auto it = endIdObj.begin(); it != endIdObj.end(); ++it) {
            TextObj* obj = (TextObj*) it->second;
            obj->inUse = false;
        }
        for (auto it = junctIdObj.begin(); it != junctIdObj.end(); ++it) {
            TextObj* obj = (TextObj*) it->second;
            obj->inUse = false;
        }
        
        for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                if(n->iTrv < 1 ) continue;
                lLen += 6 * (n->iTrv - 1);
                if (n->TrPinS[1] != 0 )
                    lLen += 6;
            } else if (n->typ == 0) {
                kLen += 6;
            } else if (n->typ == 2) {
                pLen += 6;
            }
        }
        float* linie = new float[lLen];
        float* konce = new float[kLen];
        float* punkty = new float[pLen];
        int lPtr = 0, kPtr = 0, pPtr = 0;

        for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                if(n->iTrv < 1 ) continue;
                for (int i = 0; i < n->iTrv - 1; i++) {
                    linie[lPtr++] = ((n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10]);
                    linie[lPtr++] = (n->trVectorSection[i].param[11] + wysokoscSieci);
                    linie[lPtr++] = (((-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12]));

                    linie[lPtr++] = ((n->trVectorSection[i + 1].param[8] - playerT[0])*2048 + n->trVectorSection[i + 1].param[10]);
                    linie[lPtr++] = (n->trVectorSection[i + 1].param[11] + wysokoscSieci);
                    linie[lPtr++] = (((-n->trVectorSection[i + 1].param[9] - playerT[1])*2048 - n->trVectorSection[i + 1].param[12]));
                }
                if (n->TrPinS[1] != 0) {
                    //qDebug() << "track line " << i << n->iTrv;
                    linie[lPtr++] = ((n->trVectorSection[n->iTrv - 1].param[8] - playerT[0])*2048 + n->trVectorSection[n->iTrv - 1].param[10]);
                    linie[lPtr++] = (n->trVectorSection[n->iTrv - 1].param[11] + wysokoscSieci);
                    linie[lPtr++] = (((-n->trVectorSection[n->iTrv - 1].param[9] - playerT[1])*2048 - n->trVectorSection[n->iTrv - 1].param[12]));

                    linie[lPtr++] = ((trackNodes[n->TrPinS[1]]->UiD[4] - playerT[0])*2048 + trackNodes[n->TrPinS[1]]->UiD[6]);
                    linie[lPtr++] = (trackNodes[n->TrPinS[1]]->UiD[7] + wysokoscSieci);
                    linie[lPtr++] = (((-trackNodes[n->TrPinS[1]]->UiD[5] - playerT[1])*2048 - trackNodes[n->TrPinS[1]]->UiD[8]));
                }
            } else if (n->typ == 0) {
                konce[kPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                konce[kPtr++] = (n->UiD[7]);
                konce[kPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);

                konce[kPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                konce[kPtr++] = (n->UiD[7] + wysokoscSieci);
                konce[kPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
                
                if(fabs(n->UiD[4] - playerT[0]) > 1) continue;
                if(fabs(-n->UiD[5] - playerT[1]) > 1) continue;
                
                if(!road){
                    if(endIdObj[i] == NULL){
                        endIdObj[i] = new TextObj(i);
                        endIdObj[i]->setColor(50,50,255);
                    }    
                    endIdObj[i]->inUse = true;
                    endIdObj[i]->pos[0] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                    endIdObj[i]->pos[1] = n->UiD[7] + wysokoscSieci;
                    endIdObj[i]->pos[2] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
                }
            } else if (n->typ == 2) {
                punkty[pPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                punkty[pPtr++] = (n->UiD[7]);
                punkty[pPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);

                punkty[pPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                punkty[pPtr++] = (n->UiD[7] + wysokoscSieci);
                punkty[pPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
                
                if(fabs(n->UiD[4] - playerT[0]) > 1) continue;
                if(fabs(-n->UiD[5] - playerT[1]) > 1) continue;
                
                if(!road){
                    if(junctIdObj[i] == NULL){
                        junctIdObj[i] = new TextObj(i);
                        junctIdObj[i]->setColor(255,50,50);
                    }    
                    junctIdObj[i]->inUse = true;
                    junctIdObj[i]->pos[0] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                    junctIdObj[i]->pos[1] = n->UiD[7] + wysokoscSieci;
                    junctIdObj[i]->pos[2] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
                }
            }
        }
        linieSieci.setMaterial(0.5, 0.5, 0.5);
        linieSieci.init(linie, lPtr, RenderItem::V, GL_LINES);

        konceSieci.setMaterial(0.0, 0.0, 1.0);
        konceSieci.init(konce, kPtr, RenderItem::V, GL_LINES);

        punktySieci.setMaterial(1.0, 0.0, 0.0);
        punktySieci.init(punkty, pPtr, RenderItem::V, GL_LINES);

        delete[] linie;
        delete[] konce;
        delete[] punkty;
    }

    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    linieSieci.render();
    konceSieci.render();
    punktySieci.render();
    
    if(!road){
        for (auto it = endIdObj.begin(); it != endIdObj.end(); ++it) {
            TextObj* obj = (TextObj*) it->second;
            if(obj->inUse) obj->render(playerRot);
        }
        for (auto it = junctIdObj.begin(); it != junctIdObj.end(); ++it) {
            TextObj* obj = (TextObj*) it->second;
            if(obj->inUse) obj->render(playerRot);
        }
    }
}

void TDB::getLines(float * &lineBuffer, int &length, float* playerT){
    if (!loaded) return;
    int hash = (int)playerT[0] * 10000 + (int)playerT[1];
    if (collisionLineHash == hash && this->collisionLineBuffer != NULL){
        length = this->collisionLineLength;
        lineBuffer = this->collisionLineBuffer;
        return;
    }
    Vector3f p;
    Vector3f o;
    collisionLineHash = hash;
    int len = 0;

    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = trackNodes[j];
        if (n == NULL) continue;
        if (n->typ == -1) continue;
        if (n->typ == 1) {
            for (int i = 0; i < n->iTrv; i++) {
                if (fabs(n->trVectorSection[i].param[8] - playerT[0]) > 1 || fabs(-n->trVectorSection[i].param[9] - playerT[1]) > 1) continue;
                len += getLineBufferSize((int) n->trVectorSection[i].param[0], 6, 0);
            }
        }
    }
    //qDebug() << "len" << len;
    this->collisionLineBuffer = new float[len];
    float* ptr = this->collisionLineBuffer;

    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = trackNodes[j];
        if (n == NULL) continue;
        if (n->typ == -1) continue;
        if (n->typ == 1) {
            for (int i = 0; i < n->iTrv; i++) {
                if (fabs(n->trVectorSection[i].param[8] - playerT[0]) > 1 || fabs(-n->trVectorSection[i].param[9] - playerT[1]) > 1) continue;
                p.set(
                        (n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10],
                        n->trVectorSection[i].param[11],
                        (-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12]
                        );
                o.set(
                        n->trVectorSection[i].param[13],
                        n->trVectorSection[i].param[14],
                        n->trVectorSection[i].param[15]
                        );
                getLine(ptr, p, o, (int) n->trVectorSection[i].param[0], j, i);
            }
        }
    }
    this->collisionLineLength = (ptr - this->collisionLineBuffer)/12;
    length = this->collisionLineLength;
    lineBuffer = this->collisionLineBuffer;
}

void TDB::getVectorSectionLine(float * &buffer, int &len, int x, int y, int uid, bool useOffset){
    if (!loaded) return;

    Vector3f p;
    Vector3f o;
    len = 0;

    TRnode* n = trackNodes[uid];
    if (n == NULL) return;
        
    for (int i = 0; i < n->iTrv; i++) {
        len += getLineBufferSize((int) n->trVectorSection[i].param[0], 6, 0, 1);
    }
    //qDebug() << "len" << len;
    buffer = new float[len]; 
    float* ptr = buffer;
    
    float offset = 0;
    float dlugosc = 0;
    for (int i = 0; i < n->iTrv; i++) {
        p.set(
            (n->trVectorSection[i].param[8] - x)*2048 + n->trVectorSection[i].param[10],
            n->trVectorSection[i].param[11],
            (-n->trVectorSection[i].param[9] - y)*2048 - n->trVectorSection[i].param[12]
            );
        o.set(
            n->trVectorSection[i].param[13],
            n->trVectorSection[i].param[14],
            n->trVectorSection[i].param[15]
        );

        if(useOffset)
            offset = dlugosc;
        getLine(ptr, p, o, (int) n->trVectorSection[i].param[0], uid, i, offset, 2);
        if(tsection->sekcja[n->trVectorSection[i].param[0]] != NULL)
            dlugosc += tsection->sekcja[n->trVectorSection[i].param[0]]->getDlugosc();
    }
    len = ptr - buffer;
    //qDebug() << "len" << len;
}

void TDB::renderLines(GLUU *gluu, float* playerT, float playerRot) {

    if (!loaded) return;
    int hash = (int)playerT[0] * 10000 + (int)playerT[1];
    if (!sectionLines.loaded || sectionHash != hash || !isInitSectLines) {
        Vector3f p;
        Vector3f o;
        sectionHash = hash;
        isInitSectLines = true;

        int len = 0;
        int tileRadius = 1;
        int hOffset = 0;
        //int tileRadius = 5;
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if (n == NULL) continue;
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                for (int i = 0; i < n->iTrv; i++) {
                    if (fabs(n->trVectorSection[i].param[8] - playerT[0]) > tileRadius || fabs(-n->trVectorSection[i].param[9] - playerT[1]) > tileRadius) continue;
                    //p.set(
                    //        (n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10],
                    //        n->trVectorSection[i].param[11],
                    //        (-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12]
                    //        );
                    //if (sqrt(p.x * p.x + p.z * p.z) > Game::objectLod) continue;
                    len += getLineBufferSize((int) n->trVectorSection[i].param[0], 3, 6);
                }
            }
        }
        //qDebug() << len;
        float* punkty = new float[len];
        float* ptr = punkty;
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if (n == NULL) continue;
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                //qDebug() << j;
                for (int i = 0; i < n->iTrv; i++) {
                    if (fabs(n->trVectorSection[i].param[8] - playerT[0]) > tileRadius || fabs(-n->trVectorSection[i].param[9] - playerT[1]) > tileRadius) 
                        continue;
                    p.set(
                            (n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10],
                            n->trVectorSection[i].param[11] + hOffset ,
                            (-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12]
                            );
                    o.set(
                            /*0n->getVectorSectionXRot(i),*/n->trVectorSection[i].param[13],
                            n->trVectorSection[i].param[14],
                            n->trVectorSection[i].param[15]
                            );
                    //if (sqrt(p.x * p.x + p.z * p.z) > Game::objectLod) continue;
                    drawLine(gluu, ptr, p, o, (int) n->trVectorSection[i].param[0]);
                }
            } /*else if (n.typ == 0 || n.typ == 2) {
                float x = (n.UiD[4] - aktwx)*2048 + n.UiD[6];
                float y = n.UiD[7];
                float z = (-n.UiD[5] - aktwz)*2048 - n.UiD[8];
                if (Math.sqrt(x * x + z * z) > 2000) continue;

                renderT.setSmoothing(false);
                renderT.begin3DRendering();
                if (n.typ == 0) renderT.setColor(0.0f, 0.0f, 1.0f, 1.0f);
                if (n.typ == 2) renderT.setColor(1.0f, 0.0f, 0.0f, 1.0f);
                renderT.draw3D("" + j, x, y + 4.5f, z, 0.02f);
                renderT.end3DRendering();
            }*/
        }
        //qDebug() << "aaaa4 "<<ptr - punkty;
        if(road)
            sectionLines.setMaterial(0.0, 0.0, 1.0);
        else
            sectionLines.setMaterial(1.0, 1.0, 0.0);
        sectionLines.init(punkty, ptr - punkty, RenderItem::V, GL_LINES);
        delete[] punkty;
    }

    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    sectionLines.render();
}

bool TDB::getDrawPositionOnTrNode(float* out, int id, float metry, float *sElev){
    TRnode* n = trackNodes[id];
    if (n == NULL) 
        return false;
    if (n->typ != 1) 
        return false;
    
    float sectionLength = 0;
    float length = 0;
    int idx = 0;
    Vector3f position;
    for (int i = 0; i < n->iTrv; i++) {
        idx = n->trVectorSection[i].param[0];
        
        if(tsection->sekcja[idx] == NULL){
            //qDebug() << "nie ma sekcji " << idx;
            return false;
        } else {
            sectionLength = tsection->sekcja[idx]->getDlugosc();
        }
        length += sectionLength;
        if(length < metry)
            continue;
        
        float sDistance = metry - length + sectionLength;
        tsection->sekcja.at(idx)->getDrawPosition(&position, sDistance);
        //qDebug() << "position"<<position.x<<position.y<<position.z;

        float matrix[16];
        float q[4];
        q[0] = q[1] = q[2] = 0; q[3] = 1;
        float rot[3];
        rot[0] = M_PI;
        rot[1] = n->trVectorSection[i].param[14];
        rot[2] = 0;//n->trVectorSection[i].param[15];

        float pos[3];
        pos[0] = n->trVectorSection[i].param[10];
        pos[1] = n->trVectorSection[i].param[11];
        pos[2] = n->trVectorSection[i].param[12];
        
        Quat::fromRotationXYZ(q, rot);
        Mat4::fromRotationTranslation(matrix, q, pos);
        Mat4::rotate(matrix, matrix, -n->trVectorSection[i].param[13], 1, 0, 0);

        pos[0] = position.x;
        pos[1] = position.y;
        pos[2] = -position.z;
        Vec3::transformMat4(pos, pos, matrix);
        
        out[3] = -n->trVectorSection[i].param[14] - tsection->sekcja.at(idx)->getDrawAngle(sDistance);
        out[4] = n->trVectorSection[i].param[13];
        out[5] = n->trVectorSection[i].param[8];
        out[6] = n->trVectorSection[i].param[9];
        out[0] = pos[0];
        out[1] = pos[1];
        out[2] = pos[2];        
        //position->x += (n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10];
        //position->y += n->trVectorSection[i].param[11];
        //position->z += (-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12];
        
        if(sElev != NULL)
            if(Game::useSuperelevation){
                if(i < n->iTrv - 1)
                    *sElev = -(n->trVectorSection[i].param[15]*(1.0 - sDistance/sectionLength) + n->trVectorSection[i+1].param[15]*(sDistance/sectionLength));
                else
                    *sElev = -(n->trVectorSection[i].param[15]*(1.0 - sDistance/sectionLength));
            } else {
                *sElev = 0;
            }
            
        return true;
    }
    return false;
}

int TDB::findTrItemNodeId(int id){
    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = trackNodes[j];
        if (n == NULL) continue;
        if (n->typ == 1) {
            for (int i = 0; i < n->iTri; i++) {
                if(n->trItemRef[i] == id)
                    return j;
            }
        }
    }
    return -1;
}

int TDB::findTrItemNodeIds(int id, QVector<int>& ids){
    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = trackNodes[j];
        if (n == NULL) continue;
        if (n->typ == 1) {
            for (int i = 0; i < n->iTri; i++) {
                if(n->trItemRef[i] == id)
                    ids.push_back(j);
            }
        }
    }
    if(ids.size() == 0)
        return -1;
    return ids[0];
}

int TDB::getEndpointType(int trid, int endp){
    TRnode* n = this->trackNodes[trid];
    if(n->typ !=1 )
        return -1;
    n = this->trackNodes[n->TrPinS[endp]];
    if(n == NULL) return -1;
    return n->typ;
}

void TDB::renderItems(GLUU *gluu, float* playerT, float playerRot, int renderMode) {

    gluu->setMatrixUniforms();
    
    int selectionColor = 0;
    for (auto it = this->trackItems.begin(); it != this->trackItems.end(); ++it) {
        //console.log(obj.type);
        TRitem* obj = (TRitem*) it->second;
        if(obj != NULL){
            if(!isInitTrItemsDraw)
                obj->refresh();
            if(renderMode == gluu->RENDER_SELECTION){
                selectionColor = 12 << 20;
                if(this->road)
                    selectionColor |= 1 << 19;
            }
            obj->render(this, gluu, playerT, playerRot, selectionColor);
        }
    }
    isInitTrItemsDraw = true;
}

int TDB::getLineBufferSize(int idx, int pointSize, int offset, int step) {
    if(tsection->sekcja[idx] == NULL)
        return offset;
    
    return tsection->sekcja[idx]->getLineBufferSize(pointSize, step) + offset + 6;
}

bool TDB::isRoad(){
    return this->road;
}

void TDB::getLine(float* &ptr, Vector3f p, Vector3f o, int idx, int id, int vid, float offset, int step) {

    float matrix[16];
    float q[4];
    q[0] = q[1] = q[2] = 0;
    q[3] = 1;
    float rot[3];
    rot[0] = M_PI;
    rot[1] = -o.y;
    rot[2] = 0;//o.z;

    Quat::fromRotationXYZ(q, rot);
    Mat4::fromRotationTranslation(matrix, q, reinterpret_cast<float *> (&p));
    Mat4::rotate(matrix, matrix, o.x, 1, 0, 0);

    if(tsection->sekcja[idx] != NULL){
        tsection->sekcja[idx]->drawSection(ptr, matrix, 0, id, vid, offset, step);
    }
}

void TDB::drawLine(GLUU *gluu, float* &ptr, Vector3f p, Vector3f o, int idx) {

    float matrix[16];
    float q[4];
    q[0] = q[1] = q[2] = 0; q[3] = 1;
    float rot[3];
    rot[0] = M_PI;
    rot[1] = -o.y;
    rot[2] = 0;

    Quat::fromRotationXYZ(q, rot);
    Mat4::fromRotationTranslation(matrix, q, reinterpret_cast<float *> (&p));
    //Mat4::rotate(matrix, matrix, -o.y+M_PI, 0, 1, 0);
    Mat4::rotate(matrix, matrix, o.x, 1, 0, 0);
    //Mat4::rotate(matrix, matrix, o.z, 0, 0, 1);
    
    float point1[3];
    point1[0] = 0;
    point1[1] = 0;
    point1[2] = 0;
    float point2[3];
    point2[0] = 0;
    point2[1] = 2;
    point2[2] = 0;
    Vec3::transformMat4(point1, point1, matrix);
    Vec3::transformMat4(point2, point2, matrix);
    *ptr++ = point1[0];
    *ptr++ = point1[1];
    *ptr++ = point1[2];
    *ptr++ = point2[0];
    *ptr++ = point2[1];
    *ptr++ = point2[2];

    if(tsection->sekcja[idx] != NULL){
        tsection->sekcja[idx]->drawSection(ptr, matrix, 2, -1, 0, 0, 0);
    }
}

int TDB::findNearestPositionOnTDB(float* posT, float* pos, float * q, float* tpos){
    float *lineBuffer;
    int length = 0;
    getLines(lineBuffer, length, posT);
    
    //qDebug() << "lines length" << length;

    //qDebug() << ": " << posT[0]<<" "<<posT[1]<<" "<<pos[0]<<" "<<pos[1]<<" "<<pos[2];
    float best[7];
    best[0] = 99999;
    float dist = 0;
    float intersectionPoint[3];
    int uu;
    for(int i = 0; i < length*12; i+=12){
        //qDebug() << i/12;
        //qDebug() << lineBuffer[i+0] << " "<< lineBuffer[i+1] << " " << lineBuffer[i+2] << " "<< lineBuffer[i+3] << " "<< lineBuffer[i+4] << " " << lineBuffer[i+5] ;
        //qDebug() << lineBuffer[i+6] << " "<< lineBuffer[i+7] << " " << lineBuffer[i+8] << " "<< lineBuffer[i+9] << " "<< lineBuffer[i+10] << " " << lineBuffer[i+11] ;
        dist = Intersections::pointSegmentDistance(lineBuffer + i, lineBuffer + i+6, pos, (float*)&intersectionPoint);
        if(dist < best[0]){
            best[0] = dist;
            best[1] = lineBuffer[i+3];
            best[2] = lineBuffer[i+4];

            float dist1 = Vec3::distance(lineBuffer + i, lineBuffer + i+6);
            float dist2 = Vec3::distance(lineBuffer + i, intersectionPoint);
            dist1 = dist2/dist1;
            best[3] = lineBuffer[i+5] + (lineBuffer[i+11] - lineBuffer[i+5])*dist1;
            //best[3] = intersectionPoint[0];
            //best[4] = intersectionPoint[1];
            //best[5] = intersectionPoint[2];
        }
    }
    //qDebug() << "item pos: " << best[0] << " " << best[1] << " " << best[2] << " " << best[3];
    float minDistance = best[0];
    //if(best[0] == 99999 )
    //    return false;
    if(best[0] >= 99999)
        return -1;
    //TRnode* n = trackNodes[(int)best[1]];
    //posT[0] = n->trVectorSection[(int)best[2]].param[8];
    //posT[1] = -n->trVectorSection[(int)best[2]].param[9];
    

    
    float metry = this->getVectorSectionLengthToIdx(best[1], best[2]);
    if(tpos != NULL){
        tpos[0] = best[1];
        tpos[1] = metry + best[3];
        tpos[2] = best[2];
    }    
    this->getDrawPositionOnTrNode((float*)best, best[1], metry + best[3]);
    pos[0] = best[0];
    pos[1] = best[1];
    pos[2] = -best[2];
    posT[0] = best[5];
    posT[1] = -best[6];
    if(q != NULL){
        q[0] = 0; q[1] = 0; q[2] = 0; q[3] = 1;
        Quat::rotateY(q, q, best[3]);
        Quat::rotateX(q, q, -best[4]);
    }
    
    return minDistance;
}

int TDB::findNearestPositionsOnTDB(float* posT, float * pos, QVector<TDB::IntersectionPoint> &points, float maxDistance){
    float *lineBuffer;
    int length = 0;
    getLines(lineBuffer, length, posT);

    float dist = 0;
    float intersectionPoint[3];
    float minDistance = 99999;
    
    for(int i = 0; i < length*12; i+=12){
        dist = Intersections::pointSegmentDistance(lineBuffer + i, lineBuffer + i+6, pos, (float*)&intersectionPoint);
        if(dist < maxDistance){
            points.push_back(TDB::IntersectionPoint());
            points.back().distance = dist;
            points.back().idx = lineBuffer[i+3];
            points.back().m = this->getVectorSectionLengthToIdx(lineBuffer[i+3], lineBuffer[i+4]);
            
            float dist1 = Vec3::distance(lineBuffer + i, lineBuffer + i+6);
            float dist2 = Vec3::distance(lineBuffer + i, intersectionPoint);
            dist1 = dist2/dist1;
            points.back().m += lineBuffer[i+5] + (lineBuffer[i+11] - lineBuffer[i+5])*dist1;
        }
        if(dist < minDistance)
            minDistance = dist;
    }

    if(minDistance >= 99999)
        return -1;

    return minDistance;
}

void TDB::fillNearestSquaredDistanceToTDBXZ(float* posT, QVector<Vector4f> &points, float* bbox){
    float *lineBuffer;
    int length = 0;
    getLines(lineBuffer, length, posT);
    
    float dist = 0;
    int yyy = 0;
    for(int i = 0; i < length*12; i+=12){
        if(bbox != NULL){
            if((lineBuffer[i] < bbox[0] && lineBuffer[i+6] < bbox[0] ) || (lineBuffer[i] > bbox[1] && lineBuffer[i+6] > bbox[1] )){
                //yyy++;
                continue;
            }
            if((lineBuffer[i+2] < bbox[2] && lineBuffer[i+8] < bbox[2] ) || (lineBuffer[i+2] > bbox[3] && lineBuffer[i+8] > bbox[3] )){
                //yyy++;
                continue;
            }
        }
        for(int j = 0; j < points.size(); j++){
            dist = Intersections::pointSegmentSquaredDistanceXZ(lineBuffer + i, lineBuffer + i+6, (float*)&points[j]);
            if(dist < points[j].c)
                points[j].c = dist;
        }
    }
    //qDebug() << yyy << length;
}

bool TDB::getSegmentIntersectionPositionOnTDB(float* posT, float* segment, float len, float* pos, float * q, float* tpos){
    float *lineBuffer;
    int length = 0;
    getLines(lineBuffer, length, posT);
    
    qDebug() << "lines length" << length;
    float best[7];
    best[0] = 99999;
    float dist = 0;
    float intersectionPoint[3];
    intersectionPoint[1] = pos[1];
    int uu;
    for(int i = 0; i < length*12; i+=12){
        //qDebug() << i/12;
        //qDebug() << lineBuffer[i+0] << " "<< lineBuffer[i+1] << " " << lineBuffer[i+2] << " "<< lineBuffer[i+3] << " "<< lineBuffer[i+4] << " " << lineBuffer[i+5] ;
        //qDebug() << lineBuffer[i+6] << " "<< lineBuffer[i+7] << " " << lineBuffer[i+8] << " "<< lineBuffer[i+9] << " "<< lineBuffer[i+10] << " " << lineBuffer[i+11] ;
        
        for(int j = 0; j < len; j+=12){
            bool ok = Intersections::segmentIntersection(
                lineBuffer[i], lineBuffer[i + 2], 
                lineBuffer[i+6 + 0], lineBuffer[i+6 + 2], 
                segment [ j + 0], segment [ j + 2], 
                segment [ j+6 + 0], segment [ j+6 + 2], 
                intersectionPoint[0], intersectionPoint[2]
            );
            if(!ok) continue;
            qDebug() << "intersection";
            qDebug() <<  lineBuffer[i]<< " " << lineBuffer[i + 2]<< " " <<
                lineBuffer[i+6 + 0]<< " " << lineBuffer[i+6 + 2]<< " -- " <<
                segment [ j + 0]<< " " << segment [ j + 2]<< " " <<
                segment [ j+6 + 0]<< " " << segment [ j+6 + 2]<< " " ;
            dist = Vec3::distance(intersectionPoint, pos);

            if(dist < best[0]){
                best[0] = dist;
                best[1] = lineBuffer[i+3];
                best[2] = lineBuffer[i+4];

                float dist1 = Vec3::distance(lineBuffer + i, lineBuffer + i+6);
                float dist2 = Vec3::distance(lineBuffer + i, intersectionPoint);
                dist1 = dist2/dist1;
                best[3] = lineBuffer[i+5] + (lineBuffer[i+11] - lineBuffer[i+5])*dist1;
                //best[3] = intersectionPoint[0];
                //best[4] = intersectionPoint[1];
                //best[5] = intersectionPoint[2];
            }
        }
    }
    qDebug() << "item pos: " << best[0] << " " << best[1] << " " << best[2] << " " << best[3];
    if(best[0] == 99999) return false;
    
    //TRnode* n = trackNodes[(int)best[1]];
    //posT[0] = n->trVectorSection[(int)best[2]].param[8];
    //posT[1] = -n->trVectorSection[(int)best[2]].param[9];
    
    float metry = this->getVectorSectionLengthToIdx(best[1], best[2]);
    if(tpos != NULL){
        tpos[0] = best[1];
        tpos[1] = metry + best[3];
    }
    this->getDrawPositionOnTrNode((float*)best, best[1], metry + best[3]);
    pos[0] = best[0];
    pos[1] = best[1];
    pos[2] = -best[2];
    posT[0] = best[5];
    posT[1] = -best[6];
    q[0] = 0; q[1] = 0; q[2] = 0; q[3] = 1;
    Quat::rotateY(q, q, best[3]);
    Quat::rotateX(q, q, -best[4]);
    return true;
}

bool TDB::getSegmentIntersectionPositionOnTDB(QVector<TDB::IntersectionPoint> &ipoints, TDB* segmentTDB, float* posT, float* segment, float len, float* pos){
    float *lineBuffer;
    int length = 0;
    getLines(lineBuffer, length, posT);
    
    qDebug() << "lines length" << length;
    float best[7];
    best[0] = 99999;
    float dist = 0;
    float intersectionPoint[3];
    intersectionPoint[1] = pos[1];
    int uu;
    float metry;
    float dist1, dist2;
    
    IntersectionPoint* p;
    for(int i = 0; i < length*12; i+=12){
        //qDebug() << i/12;
        //qDebug() << lineBuffer[i+0] << " "<< lineBuffer[i+1] << " " << lineBuffer[i+2] << " "<< lineBuffer[i+3] << " "<< lineBuffer[i+4] << " " << lineBuffer[i+5] ;
        //qDebug() << lineBuffer[i+6] << " "<< lineBuffer[i+7] << " " << lineBuffer[i+8] << " "<< lineBuffer[i+9] << " "<< lineBuffer[i+10] << " " << lineBuffer[i+11] ;
        
        for(int j = 0; j < len; j+=12){
            bool ok = Intersections::segmentIntersection(
                lineBuffer[i], lineBuffer[i + 2], 
                lineBuffer[i+6 + 0], lineBuffer[i+6 + 2], 
                segment [ j + 0], segment [ j + 2], 
                segment [ j+6 + 0], segment [ j+6 + 2], 
                intersectionPoint[0], intersectionPoint[2]
            );
            if(!ok) continue;
            qDebug() << "intersection";
            qDebug() <<  lineBuffer[i]<< " " << lineBuffer[i + 2]<< " " <<
                lineBuffer[i+6 + 0]<< " " << lineBuffer[i+6 + 2]<< " -- " <<
                segment [ j + 0]<< " " << segment [ j + 2]<< " " <<
                segment [ j+6 + 0]<< " " << segment [ j+6 + 2]<< " " ;
            dist = Vec3::distance(intersectionPoint, pos);
            
            ipoints.push_back(IntersectionPoint());
            p = &ipoints.back();
            p->distance = dist;
            p->idx = lineBuffer[i+3];
            p->sidx = segment[j+3];

            dist1 = Vec3::distance(lineBuffer + i, lineBuffer + i+6);
            dist2 = Vec3::distance(lineBuffer + i, intersectionPoint);
            dist1 = dist2/dist1;
            metry = this->getVectorSectionLengthToIdx(p->idx, lineBuffer[i+4]);
            p->m = metry + lineBuffer[i+5] + (lineBuffer[i+11] - lineBuffer[i+5])*dist1;
            
            if(segmentTDB != NULL){
                dist1 = Vec3::distance(segment + j, segment + j+6);
                dist2 = Vec3::distance(segment + j, intersectionPoint);
                dist1 = dist2/dist1;
                metry = segmentTDB->getVectorSectionLengthToIdx(p->sidx, segment[j+4]);
                p->sm = metry + segment[j+5] + (segment[j+11] - segment[j+5])*dist1;
            }
            qDebug() << "item p: " << p->distance << " " << p->idx << " " << p->m<< " " << p->sidx << " " << p->sm;
            //ipoints.push_back(p);
        }
    }
    std::sort(ipoints.begin(), ipoints.end());
    return true;
}

void TDB::getVectorSectionPoints(int x, int y, float* pos, QVector<float> &ptr, int mode){
    float posT[2];
    posT[0] = x;
    posT[1] = y;
    float tpos[3];
    int ok = this->findNearestPositionOnTDB((float*)posT, pos, NULL, (float*)&tpos);
    if(ok < 0)
        return;

    y = -y;
    int nid = tpos[0];
    int sid = tpos[2];
    
    if(mode == 0){
        return getVectorSectionPoints(x, y, nid, sid, ptr);
    }
    //All sections
    if(mode == 1){
        if(trackNodes[nid] == NULL)
            return;
        for(int i = 0; i < trackNodes[nid]->iTrv; i++ ){
            //float* oldPtr = ptr;
            getVectorSectionPoints(x, y, nid, i, ptr);
        }
        return;
    }
}

void TDB::getVectorSectionPoints(int x, int y, int uid, QVector<float> &ptr){
    y = -y;
    //qDebug() << "aaa";
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if (n == NULL) continue;
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                for (int i = 0; i < n->iTrv; i++) {
                    if(n->trVectorSection[i].param[2] == x && n->trVectorSection[i].param[3] == y && n->trVectorSection[i].param[4] == uid ){
                        qDebug() << "mam";
                        getVectorSectionPoints(x, y, j, i, ptr);
                    }
                }
            }
    }
}

void TDB::getVectorSectionPoints(int x, int y, int nId, int sId, QVector<float> &ptr){
    float matrix[16];
    float q[4];
    float p[3];
    q[0] = 0; q[1] = 0; q[2] = 0; q[3] = 1;
    float rot[3];

    TRnode* n = trackNodes[nId];

    rot[0] = M_PI;
    rot[1] = -n->trVectorSection[sId].param[14];
    rot[2] = 0;//n->trVectorSection[sId].param[15];
    p[0] = (n->trVectorSection[sId].param[8] - x)*2048 + n->trVectorSection[sId].param[10];
    p[1] = n->trVectorSection[sId].param[11];
    p[2] = (-n->trVectorSection[sId].param[9] + y)*2048 - n->trVectorSection[sId].param[12];

    Quat::fromRotationXYZ(q, rot);
    Mat4::fromRotationTranslation(matrix, q, p);
    Mat4::rotate(matrix, matrix, n->trVectorSection[sId].param[13], 1, 0, 0);
    //Mat4::fromRotationTranslation(matrix, q, objMatrix);
    if(tsection->sekcja[(int) n->trVectorSection[sId].param[0]] == NULL){
        qDebug() << "nie ma sekcji " << (int) n->trVectorSection[sId].param[0];
    }
    tsection->sekcja[(int) n->trVectorSection[sId].param[0]]->getPoints(ptr, matrix);
    return;
}

void TDB::updateTrItemRData(TRitem* tr){
    if(tr == NULL) 
        return;
    float trPosition[7];
    int id = findTrItemNodeId(tr->trItemId);
    if(id < 1) return;
    getDrawPositionOnTrNode((float*)&trPosition, id, tr->getTrackPosition());
    tr->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
}

void TDB::newPickupObject(int* &itemId, int trNodeId, float metry, int type){
    if(type != WorldObj::pickup) 
        return;
    
    int newTRitemId = getNewTRitemId();
    
    float trPosition[7];
    this->trackItems[newTRitemId] = TRitem::newPickupItem(newTRitemId, metry);
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, metry);
    this->trackItems[newTRitemId]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    this->trackItems[newTRitemId]->setTrItemPData((float*)&trPosition+5, (float*)&trPosition);
    itemId = new int[2];
    itemId[0] = 0;
    itemId[1] = newTRitemId;
    
    this->addItemToTrNode(trNodeId, itemId[1]);
    updateTrItem(newTRitemId);
    updateTrNode(trNodeId);
}

void TDB::newPlatformObject(int* itemId, int trNodeId, float metry, int type){
    std::function<TRitem*(int, int)> newTRitem;
    if(type == WorldObj::platform) newTRitem = &TRitem::newPlatformItem;
    if(type == WorldObj::siding) newTRitem = &TRitem::newSidingItem;
    if(type == WorldObj::carspawner) newTRitem = &TRitem::newCarspawnerItem;

    int newTRitemId1 = getNewTRitemId();
    int dlugosc = this->getVectorSectionLength(trNodeId);
    float m = metry - 1;
    if(metry < 0) metry = 0;
    float trPosition[7];
    this->trackItems[newTRitemId1] = newTRitem(newTRitemId1, m);
    int newTRitemId2 = getNewTRitemId();
    this->trackItems[newTRitemId1]->platformTrItemData[1] = newTRitemId2;
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, m);
    this->trackItems[newTRitemId1]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    itemId[0] = newTRitemId1;
    m = metry + 1;
    if(metry > dlugosc) metry = dlugosc;
    this->trackItems[newTRitemId2] = newTRitem(newTRitemId2, m);
    this->trackItems[newTRitemId2]->platformTrItemData[1] = newTRitemId1;
    this->trackItems[newTRitemId2]->platformTrItemData[0] = 0xFFFF0000;
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, m);
    this->trackItems[newTRitemId2]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    itemId[1] = newTRitemId2;
    
    this->addItemToTrNode(trNodeId, itemId[0]);
    this->addItemToTrNode(trNodeId, itemId[1]);
    updateTrItem(itemId[0]);
    updateTrItem(itemId[1]);
    updateTrNode(trNodeId);
    
}

void TDB::newSignalObject(QString filename, SignalObj::SignalUnit* units, int &signalUnits, int trNodeId, float metry, int type){
    if(type != WorldObj::signal) 
        return;
    SignalShape* sShape = this->sigCfg->signalShape[filename];
    if(sShape == NULL)
        return;
    
    //std::vector<int> subObjI;
   // for(int i = 0; i < sShape->iSubObj; i++){
   //     if(sShape->subObj[i].sigSubTypeId == sShape->SIGNAL_HEAD && !sShape->subObj[i].optional){
   //         subObjI.push_back(i);
   //     }
   // }
    
  //  signalUnits = subObjI.size();
    //qDebug() << "tdb signalUnits" << signalUnits;
    //itemId = new int[signalUnits*2];
    float trPosition[7];

    unsigned int flags = 0;
    //if(sShape->isJnLink) flags = 1;
    //int sidx = 0;
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, metry);
    
    signalUnits = 0;
    int enabledFrontFlag = 0;
    int enabledBackFlag = 0;
    int *enableFlag = NULL;
    for(int i = 0; i < sShape->iSubObj; i++){
        if(sShape->subObj[i].optional && (!sShape->subObj[i].defaultt))
            continue;
        if(sShape->subObj[i].sigSubTypeId == sShape->SIGNAL_HEAD)
            continue;
        if(sShape->subObj[i].backFacing)
            enableFlag = &enabledBackFlag;//|= 1 << (sShape->subObj[i].faceidx+3);
        else
            enableFlag = &enabledFrontFlag;//|= 1 << (sShape->subObj[i].faceidx+3);
        
        if(sShape->subObj[i].sigSubType == "NUMBER_PLATE")
            *enableFlag |= 0b0000010000;
        if(sShape->subObj[i].sigSubType == "GRADIENT_PLATE")
            *enableFlag |= 0b0000100000;
        if(sShape->subObj[i].sigSubType == "USER1")
            *enableFlag |= 0b0001000000;
        if(sShape->subObj[i].sigSubType == "USER2")
            *enableFlag |= 0b0010000000;
        if(sShape->subObj[i].sigSubType == "USER3")
            *enableFlag |= 0b0100000000;
        if(sShape->subObj[i].sigSubType == "USER4")
            *enableFlag |= 0b1000000000;
    }
    
    for(int i = 0; i < sShape->iSubObj; i++){
        //sidx = subObjI[i];
        if(sShape->subObj[i].optional && (!sShape->subObj[i].defaultt))
            continue;
        units[i].enabled = true;
        units[i].head = false;
        if(sShape->subObj[i].sigSubTypeId != sShape->SIGNAL_HEAD)
            continue;
        units[i].head = true;
        signalUnits++;
        if(sShape->subObj[i].backFacing)
            flags = enabledBackFlag;
        else
            flags = enabledFrontFlag;
        if(sShape->subObj[i].isJnLink) 
            flags |= 1;
        int newTRitemId = getNewTRitemId();
        int direction = 1;
        float angle = -trPosition[3];
        if(sShape->subObj[i].backFacing){
            direction = 0;
            angle += M_PI;
        }
        if(angle > 2*M_PI)
            angle -= 2*M_PI;
        if(angle < 0)
            angle += 2*M_PI;
        this->trackItems[newTRitemId] = TRitem::newSignalItem(newTRitemId, metry, direction, flags, sShape->subObj[i].sigSubSType);
        this->trackItems[newTRitemId]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
        this->trackItems[newTRitemId]->setSignalRot(angle);
        units[i].tdbId = 0;
        units[i].itemId =  newTRitemId;
        this->addItemToTrNode(trNodeId, units[i].itemId);
        updateTrItem(newTRitemId);
        updateTrNode(trNodeId);
    }
}

void TDB::enableSignalSubObj(QString filename, SignalObj::SignalUnit &unit, int i, int tritemid){
    SignalShape* sShape = this->sigCfg->signalShape[filename];
    if(sShape == NULL)
        return;

    //float trPosition[7];
    int newTRitemId = getNewTRitemId();
    unsigned int flags = 0;
    //if(sShape->isJnLink) flags = 1;
    //int sidx = 0;
    TRitem* trit = trackItems[tritemid];
    if(trit == NULL) return;
    int nid = findTrItemNodeId(tritemid);
    if(nid < 0) return;
    
    unit.enabled = true;
    unit.head = false;
    if(sShape->subObj[i].sigSubTypeId != sShape->SIGNAL_HEAD)
        return;
    unit.head = true;
    flags = trit->trSignalType1 & ~1;
    if(sShape->subObj[i].isJnLink) 
        flags |= 1;
    int direction = trit->trSignalType2;
    float angle = trit->trSignalType3;
    if(sShape->subObj[i].backFacing != sShape->subObj[0].backFacing){
        direction = abs(direction - 1);
        angle += M_PI;
        if(angle > 2*M_PI)
            angle -= 2*M_PI;
    }
    this->trackItems[newTRitemId] = TRitem::newSignalItem(newTRitemId, trit->getTrackPosition(), direction, flags, sShape->subObj[i].sigSubSType);
    this->trackItems[newTRitemId]->setTrItemRData((float*)(trit->trItemRData+3), (float*)trit->trItemRData);
    this->trackItems[newTRitemId]->setSignalRot(angle);
    unit.tdbId = 0;
    unit.itemId =  newTRitemId;
    this->addItemToTrNode(nid, unit.itemId);
}

void TDB::newSpeedPostObject(int speedPostType, QVector<int> & itemId, int trNodeId, float metry, int type){
    if(type != WorldObj::speedpost) 
        return;
    //SpeedPost* sShape = this->speedPostDAT->speedPost[speedPostId];
    //if(sShape == NULL)
    //    return;
    
    float trPosition[7];
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, metry);
    int newTRitemId = getNewTRitemId();
    this->trackItems[newTRitemId] = TRitem::newSpeedPostItem(newTRitemId, metry, speedPostType);
    this->trackItems[newTRitemId]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    this->trackItems[newTRitemId]->setTrItemPData((float*)&trPosition+5, (float*)&trPosition);
    float angle = trPosition[3]-M_PI/2;
    if(angle > 2*M_PI)
        angle -= 2*M_PI;
    if(angle < 0)
        angle += 2*M_PI;
    this->trackItems[newTRitemId]->setSpeedpostRot(angle);
    itemId.push_back(0);
    itemId.push_back(newTRitemId);
    //qDebug() << "tritem size"<<itemId.size();
    this->addItemToTrNode(trNodeId, itemId.last());
    updateTrItem(newTRitemId);
    updateTrNode(trNodeId);
}

void TDB::newLevelCrObject(int* &itemId, int trNodeId, float metry, int type){
    if(type != WorldObj::levelcr) 
        return;
    
    float trPosition[7];
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, metry);
    int newTRitemId = getNewTRitemId();
    this->trackItems[newTRitemId] = TRitem::newLevelCrItem(newTRitemId, metry);
    this->trackItems[newTRitemId]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    this->trackItems[newTRitemId]->setTrItemPData((float*)&trPosition+5, (float*)&trPosition);
    //qDebug() <<"a2";
    itemId = new int[2];
    itemId[0] = 0;
    if(this->road){
        itemId[0] = 1;
        this->trackItems[newTRitemId]->trItemSData2 = 5;
    }
    itemId[1] = newTRitemId;
    this->addItemToTrNode(trNodeId, itemId[1]);
    updateTrItem(newTRitemId);
    updateTrNode(trNodeId);
}

void TDB::newSoundRegionObject(int soundregionTrackType, QVector<int> &itemId, int trNodeId, float metry, int type){
    if(type != WorldObj::soundregion) 
        return;
    int newTRitemId = getNewTRitemId();
    float trPosition[7];
    this->trackItems[newTRitemId] = TRitem::newSoundRegionItem(newTRitemId, metry);
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, metry);
    this->trackItems[newTRitemId]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    this->trackItems[newTRitemId]->setTrItemPData((float*)&trPosition+5, (float*)&trPosition);
    this->trackItems[newTRitemId]->setSoundRegionData(-trPosition[3], soundregionTrackType);
    
    itemId.push_back(0);
    itemId.push_back(newTRitemId);
    
    this->addItemToTrNode(trNodeId, itemId.last());
    updateTrItem(newTRitemId);
    updateTrNode(trNodeId);
}

void TDB::newHazardObject(int * &itemId, int trNodeId, float metry, int type){
    if(type != WorldObj::hazard) 
        return;
    int newTRitemId = getNewTRitemId();
    float trPosition[7];
    this->trackItems[newTRitemId] = TRitem::newHazardItem(newTRitemId, metry);
    getDrawPositionOnTrNode((float*)&trPosition, trNodeId, metry);
    this->trackItems[newTRitemId]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    this->trackItems[newTRitemId]->setTrItemPData((float*)&trPosition+5, (float*)&trPosition);
    itemId = new int[2];
    itemId[0] = 0;
    itemId[1] = newTRitemId;
    
    this->addItemToTrNode(trNodeId, itemId[1]);
    updateTrItem(newTRitemId);
    updateTrNode(trNodeId);
}

void TDB::newCrossOverObject(int id1, float m1, int id2, float m2, int shapeIdx){
    int newTRitemId1 = getNewTRitemId();
    trackItems[newTRitemId1] = NULL;
    int newTRitemId2 = getNewTRitemId();
    float trPosition[7];
    
    trackItems[newTRitemId1] = TRitem::newCrossOverItem(newTRitemId1, m1, newTRitemId2, shapeIdx);
    
    getDrawPositionOnTrNode((float*)&trPosition, id1, m1);
    //qDebug() << trPosition[0]<< trPosition[1]<< trPosition[2];
    trackItems[newTRitemId1]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    addItemToTrNode(id1, newTRitemId1);
    
    trackItems[newTRitemId2] = TRitem::newCrossOverItem(newTRitemId2, m2, newTRitemId1, shapeIdx);
    getDrawPositionOnTrNode((float*)&trPosition, id2, m2);
    //qDebug() << trPosition[0]<< trPosition[1]<< trPosition[2];
    trackItems[newTRitemId2]->setTrItemRData((float*)&trPosition+5, (float*)&trPosition);
    addItemToTrNode(id2, newTRitemId2);
    updateTrItem(newTRitemId1);
    updateTrItem(newTRitemId2);
    updateTrNode(id1);
    updateTrNode(id2);
}

void TDB::deleteTrItem(int trid){
    TRitem* trit = this->trackItems[trid];
    
    if (trit == NULL){
        ErrorMessage *e = new ErrorMessage(
                ErrorMessage::Type_Error, 
                ErrorMessage::Source_Editor, 
                "Track Item not found." );
        ErrorMessagesLib::PushErrorMessage(e);
        return;
    }
    
    /*if(wobj != NULL){
        wobj->typeID;
        trit->type;
        
        if(wobj->typeID == WorldObj::siding){
            if (trit->type != "sidingitem"){
                return;
            }
        }
        if(wobj->typeID == WorldObj::platform){
            if (trit->type != "platformitem"){
                ErrorMessage *e = new ErrorMessage(
                        ErrorMessage::Type_Error, 
                        ErrorMessage::Source_TDB, 
                        "Expected 'platformitem' but '"+trit->type+"' found instead." );
                ErrorMessagesLib::PushErrorMessage(e);
                return;
            }
        }
        if(wobj->typeID == WorldObj::signal){
            if (trit->type != "signalitem"){
                return;
            }
        }
        if(wobj->typeID == WorldObj::levelcr){
            if (trit->type != "levelcritem"){
                return;
            }
        }
        if(wobj->typeID == WorldObj::speedpost){
            if (trit->type != "speedpostitem"){
                return;
            }
        }
        if(wobj->typeID == WorldObj::carspawner){
            if (trit->type != "carspawneritem"){
                return;
            }
        }
        if(wobj->typeID == WorldObj::hazard){
            if (trit->type != "hazzarditem"){
                return;
            }
        }
        if(wobj->typeID == WorldObj::pickup){
            if (trit->type != "pickupitem"){
                return;
            }
        }
        if(wobj->typeID == WorldObj::soundregion){
            if (trit->type != "soundregionitem"){
                return;
            }
        }
    }*/
    
    if(trit != NULL){
        trit->type = "emptyitem";
        updateTrItem(trid);
    }
    QVector<int> ids;
    int nid = findTrItemNodeIds(trid, ids);
    if(ids.size() == 0) 
        return;
    for(int i = 0; i < ids.size(); i++)
        deleteItemFromTrNode(ids[i], trid);

}

void TDB::addItemToTrNode(int tid, int iid){
    TRnode* n = this->trackNodes[tid];
    if(n == NULL) return;
    int* newVec = new int[n->iTri+1];
    std::copy(n->trItemRef, n->trItemRef+n->iTri, newVec);
    newVec[n->iTri++] = iid;
    delete[] n->trItemRef;
    n->trItemRef = newVec;
}

void TDB::deleteItemFromTrNode(int tid, int iid){
    TRnode* n = this->trackNodes[tid];
    if(n == NULL) return;
    if(n->iTri == 1){
        n->iTri = 0;
        updateTrNode(tid);
        return;
    }
    int* newVec = new int[n->iTri-1];
    for(int i = 0, j = 0; i < n->iTri; i++){
        if(n->trItemRef[i] == iid){
            continue;
        }
        newVec[j++] = n->trItemRef[i];
    }
    n->iTri--;
    delete[] n->trItemRef;
    n->trItemRef = newVec;
    updateTrNode(tid);
}

void TDB::fixTDBVectorElevation(int x, int y, int UiD){
    y = -y;
    
    TRnode *n;
    int tid = -1;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 1) {
                bool found = false;
                for(int j = 0; j < n->iTrv; j++){
                    if(n->trVectorSection[j].param[2] == x)
                        if(n->trVectorSection[j].param[3] == y)
                            if(n->trVectorSection[j].param[4] == UiD){
                                found = true;
                                break;
                    }
                }
                if(found){
                    fixTDBVectorElevation(n);
                    continue;
                }
            }
        }
}

void TDB::fixTDBVectorElevation(TRnode *n){
    if (n == NULL) return;
    if (n->typ != 1) return;
    
    n->trVectorSection[0].param[15] = 0;
    n->trVectorSection[n->iTrv - 1].param[15] = 0;
    int sect;
    float angle1, angle2;
    for(int j = 1; j < n->iTrv; j++){
        sect = n->trVectorSection[j-1].param[0];
        angle1 = tsection->sekcja[sect]->getAngle();
        sect = n->trVectorSection[j].param[0];
        angle2 = tsection->sekcja[sect]->getAngle();
        if(angle1 < 0 && angle2 < 0)
            n->trVectorSection[j].param[15] = -0.05;
        else if(angle1 > 0 && angle2 > 0)
            n->trVectorSection[j].param[15] = 0.05;
        else
            n->trVectorSection[j].param[15] = 0;
    }
}

void TDB::deleteVectorSection(int x, int y, int UiD){
    y = -y;
    
    TRnode *n;
    int tid = -1;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 1) {
                for(int j = 0; j < n->iTrv; j++)
                    if(n->trVectorSection[j].param[2] == x)
                        if(n->trVectorSection[j].param[3] == y)
                            if(n->trVectorSection[j].param[4] == UiD){
                                tid = i;
                                break;
                    }
            }
        }
    if(tid > 0){
        qDebug() << "mam id: " << tid;
        deleteVectorSection(tid);
    }
    TDB::refresh();
}

void TDB::deleteTree(int x, int y, int UiD){
    y = -y;
    
    TRnode *n;
    int tid = -1;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 1) {
                for(int j = 0; j < n->iTrv; j++)
                    if(n->trVectorSection[j].param[2] == x)
                        if(n->trVectorSection[j].param[3] == y)
                            if(n->trVectorSection[j].param[4] == UiD){
                                tid = i;
                                break;
                    }
            }
        }
    if(tid > 0){
        qDebug() << "mam id: " << tid;
        deleteTree(tid);
    }
}

void TDB::deleteTree(int d) {
        //if(trackNodes[d].typ!=0){
        //    System.out.println("to nie endpoint");
        //}
        
        int* drzewo = new int[iTRnodes+1];
        for(int i = 1; i <= iTRnodes; i++)
            drzewo[i] = 0;
         
        addToDeletedTree(drzewo, d);
        
        int w = 0;
        for(int i = 1; i <= iTRnodes; i++){
            if(drzewo[i] == 1) w++;
        }
        qDebug() << "Ilosc elementów w tym drzewie: " << w;
        
        if(w > 1000) {
            qDebug() << "Za duzo elementow do usuniecia, lepiej nie usuwac";
            return;
        }

        for(int i = 1; i <= iTRnodes; i++){
            if(drzewo[i] == 1){
                qDebug() << "Usuwam " << i;
                deleteAllTrItemsFromVectorSection(i);
                trackNodes[i] = NULL;
            }
        }
        TDB::refresh();
    }
    
void TDB::addToDeletedTree(int* drzewo, int d){
        drzewo[d] = 1;
        for(int i = 0; i < 3; i++){
            //qDebug() << trackNodes[d]->TrPinS[i];
            if(trackNodes[d]->TrPinS[i] == 0) 
                continue;
            //qDebug() << drzewo[trackNodes[d]->TrPinS[i]];
            if(drzewo[trackNodes[d]->TrPinS[i]] == 0)
                addToDeletedTree(drzewo, trackNodes[d]->TrPinS[i]);
        }
    }

bool TDB::deleteNulls() {
        for(int i = 1; i <= iTRnodes; i++){
            if(trackNodes[i] == NULL){
                qDebug() << "Removing NULL TrackNode at: "<<i;
                int stare = findBiggest();
                if(stare <= i) {
                    qDebug() << "There is no more NULL TrackNodes.";
                    iTRnodes = stare;
                    return false;
                }
                trackNodes[i] = trackNodes[stare];
                trackNodes[stare] = NULL;
                qDebug() << i << "Replaced by: " << stare;
                
                for(int j = 0; j < 3; j++){
                    if(trackNodes[i]->TrPinS[j] == 0) 
                        continue;
                    if(trackNodes[trackNodes[i]->TrPinS[j]] == NULL)
                        qDebug() << "Fail, unexpected NULL TrackNode found!" << trackNodes[i]->TrPinS[j];
                    else
                        trackNodes[trackNodes[i]->TrPinS[j]]->podmienTrPin(stare, i);
                }
                replaceSignalDirJunctionId(stare, i);
                return true;
            }
        }
        qDebug() << "There is no more NULL TrackNodes.";
        return false;
    }

void TDB::sortItemRefs(){
    StaticTrackItems = &trackItems;
    for(int i = 1; i <= iTRnodes; i++){
        if(trackNodes[i] == NULL)
                continue;
        if(trackNodes[i]->iTri > 0){
            int *pointer = trackNodes[i]->trItemRef;
            int len = trackNodes[i]->iTri;
            std::sort(pointer, pointer + len, SortItemRefsCompare );
        }
    }
}

bool TDB::SortItemRefsCompare(int a, int b){
    return TDB::StaticTrackItems[0][a]->getTrackPosition() < TDB::StaticTrackItems[0][b]->getTrackPosition();
}

void TDB::replaceSignalDirJunctionId(int oldId, int newId){
    for (int i = 0; i <= this->iTRitems; i++) {
        if(trackItems[i] == NULL) continue;
        if(trackItems[i]->trSignalDir != NULL){
            for(int j = 0; j < trackItems[i]->trSignalDirs*4; j+=4){
                if(trackItems[i]->trSignalDir[j+0] == oldId){
                    trackItems[i]->trSignalDir[j+0] = newId;
                    qDebug() << "trSignalDir trndoe id replaced: "<<oldId<<" "<<newId;
                }
            }
        }
    }
}

int TDB::findBiggest() {
        for(int i = iTRnodes; i > 0; i--){
            if(trackNodes[i] != NULL)
                return i;
        }
        return 1;
    }

void TDB::saveEmpty(bool road) {    
    QString sh;
    QString path;
    QString extension = "tdb";
    if(road) extension = "rdb";
    path = Game::root + "/routes/" + Game::route + "/" + Game::routeName + "." + extension;
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setRealNumberPrecision(8);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0T0t______\n\n";
    out << "TrackDB (\n";
    out << "	Serial ( 0 )\n";
    out << ")";
    file.close();
}
    
void TDB::updateTrNode(int nid){
    
}

void TDB::updateTrItem(int iid){
    
}

void TDB::updateTrackSection(int id){
    
}

void TDB::updateTrackShape(int id){
    
}

void TDB::save() {
    if(!Game::writeEnabled) return;
    if(!Game::writeTDB) return;
    
    while(deleteNulls());
    sortItemRefs();
    this->isInitLines = false;
    
    QString sh;
    QString path;
    QString extension = "tdb";
    if(this->road) extension = "rdb";
    path = Game::root + "/routes/" + Game::route + "/" + Game::routeName + "." + extension;
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setRealNumberPrecision(8);
    //out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0T0t______\n\n";
    saveToStream(out);
    file.close();

    saveTit();
    if(!this->road) 
        this->tsection->saveRoute();
    qDebug() << "Route Saved";
}

int TDB::updateTrNodeData(FileBuffer *data){
    QString sh;
    int nid = 0;

    TRnode *nowy = NULL;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("id")) {
            nid = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("remove")) {
            qDebug() << "remove trnode" << nid;
            trackNodes[nid] = NULL;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("tracknode")) {
            //objloaded = false;
            nowy = new TRnode();
            nowy->loadUtf16Data(data);
            trackNodes[nid] = nowy;
            ParserX::SkipToken(data);
            continue;
        }
        
        ParserX::SkipToken(data);
        continue;
    }
    return nid;
}

TRitem *TDB::updateTrItemData(FileBuffer *data){
    QString sh;
    int nid = 0;

    TRitem *nowy = new TRitem();
    if(this->road)
        nowy->tdbId = 1;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("id")) {
            nid = ParserX::GetNumber(data);
            nowy->trItemId = nid;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("remove")) {
            qDebug() << "remove tritem" << nid;
            trackItems[nid] = NULL;
            ParserX::SkipToken(data);
            continue;
        }
        if(!nowy->init(sh)){
            qDebug() << "#TDB TrItemTable undefined token " << sh;
            ParserX::SkipToken(data);
            continue;
        } else {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                nowy->set(sh, data);
                ParserX::SkipToken(data);
            }
            this->trackItems[nowy->trItemId] = nowy;
            ParserX::SkipToken(data);
            continue;
        }
        
        ParserX::SkipToken(data);
        continue;
    }
    return nowy;
}

void TDB::updateTrackShapeData(FileBuffer *data){
    QString sh;
    TrackShape *nowy = new TrackShape();
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("trackshape")) {
            nowy->loadUtf16Data(data);
            this->tsection->shape[nowy->id] = nowy;
            if(nowy->id >= tsection->routeShapes)
                tsection->routeShapes = nowy->id + 1;
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
        continue;
    }
}

void TDB::updateTrackSectionData(FileBuffer *data){
    QString sh;
    TSection *nowy = new TSection();
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        qDebug() << sh;
        if (sh == ("tracksection")) {
            nowy->loadUtf16Data(data);
            this->tsection->sekcja[nowy->id] = nowy;
            if(nowy->id >= tsection->routeMaxIdx){
                tsection->routeMaxIdx = nowy->id + 1;
                if(tsection->routeMaxIdx % 2 == 1)
                    tsection->routeMaxIdx++;
            }
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
        continue;
    }
}

void TDB::saveToStream(QTextStream &out){
    out << "TrackDB (\n";
    out << "	Serial ( " << this->serial << " )\n";
    if(this->iTRnodes > 0){
    out << "	TrackNodes ( " << (this->iTRnodes) << "\n";

    for (int i = 1; i <= this->iTRnodes; i++) {
        if (trackNodes[i] == NULL) 
            continue;
        out << "		TrackNode ( " << i << "\n";
        switch (trackNodes[i]->typ) {
            case 0:
                out << "			TrEndNode ( " << trackNodes[i]->args[0] << " )\n";
                out << "			UiD ( ";
                for (int j = 0; j < 12; j++) {
                    out << trackNodes[i]->UiD[j] << " ";
                }
                out << ")\n";
                out << "			TrPins ( 1 0\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[0] << " " << trackNodes[i]->TrPinK[0] << " )\n";
                out << "			)\n";
                break;
            case 1:
                out << "			TrVectorNode (\n";
                out << "				TrVectorSections ( " << trackNodes[i]->iTrv << "";
                for (int j = 0; j < trackNodes[i]->iTrv; j++) {
                    for (int jj = 0; jj < 7; jj++) {
                        out << " " << trackNodes[i]->trVectorSection[j].param[jj];
                    }
                    out << " 00";
                    for (int jj = 8; jj < 16; jj++) {
                        out << " " << trackNodes[i]->trVectorSection[j].param[jj];
                    }
                    if (j % 11 == 0 && j > 0 && j < trackNodes[i]->iTrv - 1)
                        out << "\n					";
                }
                out << " )\n";
                if(trackNodes[i]->trItemRef != 0 && trackNodes[i]->iTri > 0){
                    out << "				TrItemRefs ( "<<trackNodes[i]->iTri<<"\n";
                    for(int j = 0; j<trackNodes[i]->iTri; j++){
                        out << "					TrItemRef ( "<<trackNodes[i]->trItemRef[j]<<" )\n";
                    }
                    out << "				)\n";
                }
                out << "			)\n";
                out << "			TrPins ( 1 1\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[0] << " " << trackNodes[i]->TrPinK[0] << " )\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[1] << " " << trackNodes[i]->TrPinK[1] << " )\n";
                out << "			)\n";
                break;
            case 2:
                out << "			TrJunctionNode ( " << trackNodes[i]->args[0] << " " << trackNodes[i]->args[1] << " " << trackNodes[i]->args[2] << " )\n";
                out << "			UiD ( ";
                for (int j = 0; j < 12; j++) {
                    out << trackNodes[i]->UiD[j] << " ";
                }
                out << ")\n";
                out << "			TrPins ( 1 2\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[0] << " " << trackNodes[i]->TrPinK[0] << " )\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[1] << " " << trackNodes[i]->TrPinK[1] << " )\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[2] << " " << trackNodes[i]->TrPinK[2] << " )\n";
                out << "			)\n";
                break;
        }

        out << "		)\n";
    }
    out << "	)\n";
    }
    
    if(this->iTRitems > 0){
        out << "	TrItemTable ( " << (this->iTRitems) << "\n";
        for (int i = 0; i <= this->iTRitems; i++) {
            if(this->trackItems[i] != NULL)
                this->trackItems[i]->save(&out);
        }
        out << "	)\n";
    }

    out << ")";

}

void TDB::saveTit() {
    if(!Game::writeEnabled) return;
    if(!Game::writeTDB) return;
    
    QString path;
    QString extension = "tit";
    if(this->road) extension = "rit";
    path = Game::root + "/routes/" + Game::route + "/" + Game::routeName + "." + extension;
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setRealNumberPrecision(8);
    //out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0T0t______\n\n";

    bool tit = true;
    if(this->iTRitems > 0){
        out << "TrItemTable ( " << (this->iTRitems) << "\n";
        for (int i = 0; i <= this->iTRitems; i++) {
            if(this->trackItems[i] != NULL)
                this->trackItems[i]->save(&out, tit);
        }
        out << ")";
    }
    

    file.close();

    //qDebug() << "TIT Saved";
}

void TDB::checkSignals(){
    int trtype[4];
    trtype[0] = 0;
    trtype[1] = 0;
    trtype[2] = 0;
    trtype[3] = 0;
    if(this->iTRitems > 0){
        int tid = 0;
        TRnode* n;
        for (int i = 0; i < this->iTRitems; i++) {
            if(trackItems[i] == NULL) continue;
            if(trackItems[i]->trSignalDir != NULL){
                for(int j = 0; j < trackItems[i]->trSignalDirs*4; j+=4){
                    tid = trackItems[i]->trSignalDir[j+0];
                    n = trackNodes[tid];
                    if(n == NULL)
                        trtype[3]++;
                    else
                        trtype[n->typ]++;
                }
            }
        }
    }
    qDebug() << "suma: "<<trtype[0]<<" "<<trtype[1]<<" "<<trtype[2]<<" "<<trtype[3];
}

TDB::TDB(const TDB& o) {
    
    // simple copy
    tsection = o.tsection; 
    sigCfg = o.sigCfg;
    speedPostDAT = o.speedPostDAT;
    endIdObj = o.endIdObj;
    junctIdObj = o.junctIdObj;
    
    collisionLineBuffer = o.collisionLineBuffer;
    collisionLineLength = o.collisionLineLength;
    collisionLineHash = o.collisionLineHash;
    
    // deep copy
    loaded = o.loaded;
    iTRnodes = o.iTRnodes;
    iTRitems = o.iTRitems;
    serial = o.serial;
    defaultEnd = o.defaultEnd;
    wysokoscSieci = o.wysokoscSieci;
    road = o.road;
    
    for (auto it = o.trackItems.begin(); it != o.trackItems.end(); ++it ){
        if(it->second == NULL)
            continue;
        trackItems[it->first] = new TRitem(*(it->second));
    }
    
    for (auto it = o.trackNodes.begin(); it != o.trackNodes.end(); ++it ){
        if(it->second == NULL)
            continue;
        trackNodes[it->first] = new TRnode(*(it->second));
    }
}

TDB::~TDB() {
    for (auto it = trackItems.begin(); it != trackItems.end(); ++it ){
        if(it->second == NULL)
            continue;
        delete it->second;
    }
    
    for (auto it = trackNodes.begin(); it != trackNodes.end(); ++it ){
        if(it->second == NULL)
            continue;
        delete it->second;
    }
}

void TDB::getUsedTileList(QMap<int, QPair<int, int>*> &tileList, int radius, int step){
    if (!loaded) return;
    
    QMap<int, QPair<int, int>*> tileList2;
    for (int i = 0; i < trackNodes.size(); i++ ) {
        if(trackNodes[i] == NULL)
            continue;
        if(trackNodes[i]->typ == 1)
            continue;
        if(tileList2[trackNodes[i]->UiD[4]*10000 + trackNodes[i]->UiD[5]] == NULL)
            tileList2[trackNodes[i]->UiD[4]*10000 + trackNodes[i]->UiD[5]] = new QPair<int, int>(trackNodes[i]->UiD[4], trackNodes[i]->UiD[5]);
        }
    
    QMapIterator<int, QPair<int, int>*> i(tileList2);
    int x, z;
    radius *= step;
    qDebug() << "radius" << radius;
    while (i.hasNext()) {
        i.next();
        if(i.value() == NULL)
            continue;
        x = i.value()->first;
        z = i.value()->second;
        for(int i = -radius; i <= radius; i+=step)
            for(int j = -radius; j <= radius; j+=step){
                if(tileList[(x+i)*10000+(z+j)] == NULL){
                    tileList[(x+i)*10000+(z+j)] = new QPair<int, int>(x+i, z+j);
                }
            }
    }
}

void TDB::checkDatabase(){
    // Variables
    QHash<int, QVector<WorldObj*>> objects;

    float *drawPosition = new float[7];
    bool isPosition = false;

    // Build WorldFile data
    if(Game::loadAllWFiles){
        Game::currentRoute->fillWorldObjectsByTrackItemIds(objects, tdbId);
    }
        

    for (int i = 0; i < this->iTRitems; i++) {
        if(trackItems[i] == NULL) 
            continue;
        
        isPosition = false;
        
        if (trackItems[i]->type != "emptyitem"){
            QVector<int> ids;
            int id = findTrItemNodeIds(i, ids);
            if (ids.size() == 0) {
                ErrorMessage *e = new ErrorMessage(
                        ErrorMessage::Type_Error, 
                        tdbName, 
                        QString("Item has no trackNode: ") + QString::number(i) + ". Type: " + trackItems[i]->type,
                        "Interactive item is not placed on any track. Should it be removed?");
                e->setObject((GameObj*)trackItems[i]);
                ErrorMessagesLib::PushErrorMessage(e);
                if(Game::autoFix){
                    e->type = ErrorMessage::Type_AutoFix;
                    e->action += "\nAutoFix: Item removed by TSRE.";
                    this->deleteTrItem(i);
                }
            } else if (ids.size() > 1) {
                ErrorMessage *e = new ErrorMessage(
                        ErrorMessage::Type_Error, 
                        tdbName, 
                        QString("Item referenced in more than one trackNode: ") + QString::number(i) + ". Type: " + trackItems[i]->type,
                        "Interactive item is placed on more than one track.\nMay cause fatal errors and Open Rails crash. ");
                e->setObject((GameObj*)trackItems[i]);
                ErrorMessagesLib::PushErrorMessage(e);
                if(Game::autoFix){
                    e->type = ErrorMessage::Type_AutoFix;
                    e->action += "\nAutoFix: Item removed by TSRE.";
                    this->deleteTrItem(i);
                }
            }
            
            if(id >= 0 ){
                isPosition = getDrawPositionOnTrNode(drawPosition, id, trackItems[i]->getTrackPosition());
                if(!isPosition){
                    ErrorMessage *e = new ErrorMessage(
                            ErrorMessage::Type_Error, 
                            tdbName, 
                            QString("Item has no position: ") + QString::number(i) + ". Type: " + trackItems[i]->type,
                            "Item is placed on a track but at incorrect position. Should it be removed?");
                    e->setObject((GameObj*)trackItems[i]);
                    ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Item removed by TSRE.";
                        this->deleteTrItem(i);
                    }
                }
            }
        }
        
        if(trackItems[i]->type == "signalitem"){
            if(trackItems[i]->trSignalDirs == 1){
                int jid = trackItems[i]->trSignalDir[0];
                TRnode* n = trackNodes[jid];
                if(n == NULL) { 
                    ErrorMessage *e = new ErrorMessage(
                            ErrorMessage::Type_Error, 
                            tdbName, 
                            QString("Signal Link broken. Must be linked to junction, but no track found: ") + QString::number(i) + ". Type: " + trackItems[i]->type
                            );
                    e->setObject((GameObj*)trackItems[i]);
                    if(isPosition)
                        e->setLocationXYZ(drawPosition[5], drawPosition[6], drawPosition[0], drawPosition[1], drawPosition[2]);
                    ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Broken Signal Link removed by TSRE.";
                        trackItems[i]->trSignalDirs = 0;
                        trackItems[i]->trSignalDir = NULL;
                        trackItems[i]->trSignalRDir = NULL;
                    }
                }
                if(n->typ != 2) {
                    ErrorMessage *e = new ErrorMessage(
                            ErrorMessage::Type_Error, 
                            tdbName, 
                            QString("Signal Link broken. Id: ") + QString::number(i) + ". Type: " + trackItems[i]->type,
                            "Signal must be linked to a junction, but other trackNode was found. \nFatal error. Causes Open Rails to crash."
                            );
                    e->setObject((GameObj*)trackItems[i]);
                    if(isPosition)
                        e->setLocationXYZ(drawPosition[5], drawPosition[6], drawPosition[0], drawPosition[1], drawPosition[2]);
                    ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Broken Signal Link removed by TSRE.";
                        trackItems[i]->trSignalDirs = 0;
                        trackItems[i]->trSignalDir = NULL;
                        trackItems[i]->trSignalRDir = NULL;
                    }
                }
            }
        }
        
        if(trackItems[i]->type == "crossoveritem"){
            if(trackItems[i]->crossoverTrItemData == NULL){
                    ErrorMessage *e = new ErrorMessage(
                            ErrorMessage::Type_Error, 
                            tdbName, 
                            QString("CrossoverItem no data: ") + QString::number(i) + ". Type: " + trackItems[i]->type,
                            "Item broken. Should be removed."
                            );
                    e->setObject((GameObj*)trackItems[i]);
                    if(isPosition)
                        e->setLocationXYZ(drawPosition[5], drawPosition[6], drawPosition[0], drawPosition[1], drawPosition[2]);
                    ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Item removed by TSRE.";
                        this->deleteTrItem(i);
                    }
            } else {
                int iid1 = trackItems[i]->crossoverTrItemData[0];
                if(trackItems[iid1]->crossoverTrItemData == NULL){
                    ErrorMessage *e = new ErrorMessage(
                                ErrorMessage::Type_Error, 
                                tdbName, 
                                QString("CrossoverItem second item does not match: ") + QString::number(i) + ". Type: " + trackItems[i]->type
                                );
                        e->setObject((GameObj*)trackItems[i]);
                        if(isPosition)
                            e->setLocationXYZ(drawPosition[5], drawPosition[6], drawPosition[0], drawPosition[1], drawPosition[2]);
                        ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Item removed by TSRE.";
                        this->deleteTrItem(i);
                    }
                } else {
                    int iid2 = trackItems[iid1]->crossoverTrItemData[0];
                    if(iid2 != i){
                        ErrorMessage *e = new ErrorMessage(
                                ErrorMessage::Type_Error, 
                                tdbName, 
                                QString("CrossoverItems data does not match: ") + QString::number(i) + ". Type: " + trackItems[i]->type
                                );
                        e->setObject((GameObj*)trackItems[i]);
                        if(isPosition)
                            e->setLocationXYZ(drawPosition[5], drawPosition[6], drawPosition[0], drawPosition[1], drawPosition[2]);
                        ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Item removed by TSRE.";
                        this->deleteTrItem(i);
                    }
                    }
                }
            }
            
            // delete all crossoveritems
            //this->deleteTrItem(i);
        }
        
        if(Game::loadAllWFiles){
            if (trackItems[i]->type == "crossoveritem" || trackItems[i]->type == "emptyitem"){
                if(objects[i].size() > 0){
                    ErrorMessage *e = new ErrorMessage(
                            ErrorMessage::Type_Error, 
                            tdbName, 
                            QString("Item wrongly referenced in W files. Id: ") + QString::number(i) + ". Type: " + trackItems[i]->type
                            );
                    e->setObject((GameObj*)trackItems[i]);
                    if(isPosition)
                        e->setLocationXYZ(drawPosition[5], drawPosition[6], drawPosition[0], drawPosition[1], drawPosition[2]);
                    ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Item removed by TSRE.";
                        this->deleteTrItem(i);
                    }
                }
            } else {
                if(objects[i].size() == 0){
                    ErrorMessage *e = new ErrorMessage(
                            ErrorMessage::Type_Error, 
                            tdbName, 
                            QString("Item not referenced in W files. Id: ") + QString::number(i) + ". Type: " + trackItems[i]->type,
                            "Interactive Item was not found in World Tile database. \n"
                            "Enable 'TrackDB Items' View. "
                            "Jump to it's location and check if it should be removed, "
                            "or if World File is broken."
                            );
                    e->setObject((GameObj*)trackItems[i]);
                    if(isPosition)
                        e->setLocationXYZ(drawPosition[5], drawPosition[6], drawPosition[0], drawPosition[1], drawPosition[2]);
                    ErrorMessagesLib::PushErrorMessage(e);
                    if(Game::autoFix){
                        e->type = ErrorMessage::Type_AutoFix;
                        e->action += "\nAutoFix: Item removed by TSRE.";
                        this->deleteTrItem(i);
                    }
                }
            }
        }
    }

        /*if(Game::loadAllWFiles){
            if(Game::currentRoute == NULL)
                continue;
            
            QVector<WorldObj*> objects;
            int tdbId = 0;
            if(this->road)
                tdbId = 1;
            Game::currentRoute->fillWorldObjectsByTrackItemId(objects, tdbId, i);
            
            if (trackItems[i]->type == "crossoveritem" || trackItems[i]->type == "emptyitem"){
                if(objects.size() > 0){
                    ErrorMessage *e = new ErrorMessage("error", "TrackDB", QString("Track Item wrongly referenced in W files: ") + i + " type: " + trackItems[i]->type );
                    ErrorMessagesLib::PushErrorMessage(e);
                }
            } else {
                if(objects.size() == 0){
                    ErrorMessage *e = new ErrorMessage("error", "TrackDB", QString("Track Item not referenced in W files: ") + i + " type: " + trackItems[i]->type );
                    ErrorMessagesLib::PushErrorMessage(e);
                }
            }
        }*/
        /*if(trackItems[i]->trSignalDir != NULL){
            for(int j = 0; j < trackItems[i]->trSignalDirs*4; j+=4){
                tid = trackItems[i]->trSignalDir[j+0];
                n = trackNodes[tid];
                if(n == NULL){
                    
                }
            }
        }*/
    
    for(int i = 1; i <= iTRnodes; i++){
        TRnode* n = trackNodes[i];
        if (n == NULL) 
            continue;
        if (n->typ == -1) 
            continue;
        if (n->typ == 1) {
            if(n->iTrv == 0){
                ErrorMessage *e = new ErrorMessage(
                    ErrorMessage::Type_Error, 
                    tdbName, 
                    QString("TrackNode: ") + QString::number(i) + ". Trvectorsection is empty." 
                );
                ErrorMessagesLib::PushErrorMessage(e);
            }
        }
        if (n->typ == 2) {
            int originId = n->TrPinS[0];
            if(originId == 0){
                    ErrorMessage *e = new ErrorMessage(
                        ErrorMessage::Type_Warning, 
                        tdbName, 
                        QString("TrackNode: ") + QString::number(i) + ". Junction origin linked to nothing.",
                        "Junction is not finished. Junction origin must be linked to a track section, otherwise OR simulation will crash."
                    );
                    e->setLocationXYZ(n->UiD[4], n->UiD[5], n->UiD[6], n->UiD[7], n->UiD[8]);
                    ErrorMessagesLib::PushErrorMessage(e);
            } else {
                TRnode *origin = trackNodes[originId];
                if(origin == NULL) {
                    ErrorMessage *e = new ErrorMessage(
                        ErrorMessage::Type_Error, 
                        tdbName, 
                        QString("TrackNode: ") + QString::number(i) + ". Junction origin linked to a NULL TrackNode.",
                        "There might be a fatal error inside Track Database."
                    );
                    e->setLocationXYZ(n->UiD[4], n->UiD[5], n->UiD[6], n->UiD[7], n->UiD[8]);
                    ErrorMessagesLib::PushErrorMessage(e);
                } else if(!origin->isLikedTo(i)){
                    ErrorMessage *e = new ErrorMessage(
                        ErrorMessage::Type_Error, 
                        tdbName, 
                        QString("TrackNode: ") + QString::number(i) + ". Link Error.",
                        QString("Fatal Track Database error. Junction origin linked to TrackNode ") + QString::number(originId) + ", but this TrackNode is not linked to this Junction."
                    );
                    e->setLocationXYZ(n->UiD[4], n->UiD[5], n->UiD[6], n->UiD[7], n->UiD[8]);
                    ErrorMessagesLib::PushErrorMessage(e);
                } else if(origin->typ == 2){
                    ErrorMessage *e = new ErrorMessage(
                        ErrorMessage::Type_Error, 
                        tdbName, 
                        QString("TrackNode: ") + QString::number(i) + ".  Junction linked to another Junction.",
                        QString("Fatal Track Database error. Junction origin linked to another Junction origin. ") + QString::number(originId)
                    );
                    e->setLocationXYZ(n->UiD[4], n->UiD[5], n->UiD[6], n->UiD[7], n->UiD[8]);
                    ErrorMessagesLib::PushErrorMessage(e);
                }
            }
        }
        /*for(int j = 0; j < trackNodes[i]->iTri; j++){
            if(j > 0 && old > trackItems[trackNodes[i]->trItemRef[j]]->getTrackPosition())
                qDebug() << "--fail!--"<< old << trackItems[trackNodes[i]->trItemRef[j]]->getTrackPosition();
            old = trackItems[trackNodes[i]->trItemRef[j]]->getTrackPosition();
        }*/
    }
    //
    //ErrorMessage *e = new ErrorMessage();
    //ErrorMessagesLib::PushErrorMessage(e);
}