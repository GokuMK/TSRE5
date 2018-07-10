/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Ruch.h"
#include "TRnode.h"
#include "TSectionDAT.h"
#include "Game.h"
#include "TRitem.h"

Ruch::Ruch() {

}

void Ruch::set(int nid, int m, int tdirection, QMap<int, int>* jDirections) {
    nodeIdx = nid;
    nodeDist = m;
    kierunek = tdirection;
    direction = -(kierunek - 0.5)*2;
    junctionDirections = jDirections;
    if(junctionDirections == NULL)
        junctionDirections = new QMap<int, int>();

    TDB *tdb = Game::trackDB;
    if (tdb->trackNodes[nodeIdx]->typ == 0 || tdb->trackNodes[nodeIdx]->typ == 2) {
        kierunek = tdb->trackNodes[nodeIdx]->TrPinK[0];
        nodeIdx = tdb->trackNodes[nodeIdx]->TrPinS[0];
        nodeLength = Game::trackDB->getVectorSectionLength(nodeIdx);
        if (kierunek == 1) {
            nodeDist = 0;
        } else {
            nodeDist = nodeLength;
        }
        direction = -(kierunek - 0.5)*2;
    } else {
        nodeLength = Game::trackDB->getVectorSectionLength(nodeIdx);
    }
}

void Ruch::next(float m) {
    if(m < 0){
        back(m);
    } else {
        distanceDownPath += m;
        while(m > 0){
            if(m > 0.5){
                toNext(0.5);
                m -= 0.5;
            } else {
                toNext(m);
                m = 0;
            }
        }
    }
    //nodeDist += m*direction;
    //checkNode();
    if(trackItems)
        checkPassingItems();
}

void Ruch::back(float m) {
    if(m > 0){
        next(m);
    } else {
        m = -m;
        distanceDownPath += m;
        while(m > 0){
            if(m > 0.5){
                toNext(-0.5);
                m -= 0.5;
            } else {
                toNext(-m);
                m = 0;
            }
        }
    }
    
    if(trackItems)
        checkPassingItems();
}

void Ruch::toNext(float m){
    lastNodeDist = nodeDist;
    nodeDist += m*direction;
    int sign = 1;
    if(m < 0)
        sign = -1;
    checkNode(sign);
}

void Ruch::checkNode(int mSign) {
    TDB *tdb = Game::trackDB;
    int kier, nodeId;
    //qDebug() << "modeDist" << nodeDist << nodeLength;
    float nodeDistLeft = 0;
    if (nodeDist >= nodeLength) {
        nodeDistLeft = nodeDist - nodeLength;
        kier = tdb->trackNodes[nodeIdx]->TrPinK[1];
        nodeId = tdb->trackNodes[nodeIdx]->TrPinS[1];
    } else if (nodeDist < 0) {
        nodeDistLeft = -nodeDist;
        kier = tdb->trackNodes[nodeIdx]->TrPinK[0];
        nodeId = tdb->trackNodes[nodeIdx]->TrPinS[0];
    } else {
        return;
    }

    TRnode *n = tdb->trackNodes[nodeId];
    if (n == NULL)
        return;
    if (n->typ == 2) {
        int u = 0;// n->TrP1-1;
        onJunction = 2; // just info
        if (kier == 1){
            onJunction = 1; // just info
            u = 1+(*junctionDirections)[nodeId];//n->TrP1;//+n->TrP2-1;
        }
        
        // todo if u > 1 allow junction switch
        //if(u > 1)
        //    u++;
        //u = n->TrP1*kierunek;
        kierunek = n->TrPinK[u];
        nodeIdx = n->TrPinS[u];
        nodeLength = Game::trackDB->getVectorSectionLength(nodeIdx);

    } else if (n->typ == 0) {
        kierunek = n->TrPinK[0];
        nodeIdx = n->TrPinS[0];
        nodeLength = Game::trackDB->getVectorSectionLength(nodeIdx);

    } else if (n->typ == 1) {
        nodeIdx = nodeId;
        kierunek = kier;
        nodeLength = Game::trackDB->getVectorSectionLength(nodeIdx);
    }
    
    if (kierunek == 1) {
        nodeDist = nodeDistLeft;
    } else {
        nodeDist = nodeLength-nodeDistLeft;
    }
    lastNodeDist = nodeDist;
    direction = (kierunek - 0.5)*2*mSign;
}

float * Ruch::getCurrentPosition(float *sElev) {
    Game::trackDB->getDrawPositionOnTrNode(drawPosition, nodeIdx, nodeDist, sElev);
    if(kierunek == 0 && sElev != NULL)
        *sElev = -*sElev;
    return drawPosition;
}

int Ruch::getVectorDirection(){
    return kierunek;
}

float Ruch::getDistanceDownPath(){
    return distanceDownPath;
}

void Ruch::trackPassingItems(bool val){
    trackItems = val;
}

QString Ruch::getLastItemName(){
    return lastItemName;
}

void Ruch::checkPassingItems(){
    if(lastNodeDist == nodeDist)
        return;
    TDB *tdb = Game::trackDB;
    TRnode *n = tdb->trackNodes[nodeIdx];
    for(int i = 0; i < n->iTri; i++){
        int itemId = n->trItemRef[i];
        if(tdb->trackItems[itemId] == NULL)
            return;
        float dist = tdb->trackItems[itemId]->getTrackPosition();
        if((dist > lastNodeDist && dist < nodeDist) || (dist < lastNodeDist && dist > nodeDist)){
            lastItemId = itemId;
            lastItemName = tdb->trackItems[itemId]->getTrackItemName();
            qDebug() << "!!!!!!!!!!!!!!!!! item Id" << itemId << lastItemName;
        }
    }
    
}
/*
bool Ruch::next(){
        if(aktt==-1) return true;
        
        switch(trackDB->trackNodes[aktt]->typ){
            case 0:
                //System.out.print("byles na endpoint "+aktt);
                
                pozW.x = trackDB->trackNodes[aktt]->UiD[6];
                pozW.y = trackDB->trackNodes[aktt]->UiD[7];
                pozW.z = trackDB->trackNodes[aktt]->UiD[8];
                pozT.x = (int) trackDB->trackNodes[aktt]->UiD[4];
                pozT.z = (int) trackDB->trackNodes[aktt]->UiD[5];
                
                kierunek = trackDB->trackNodes[aktt]->TrPinK[0];
                aktt = trackDB->trackNodes[aktt]->TrPinS[0];
                //System.out.println(" przejdziesz do "+aktt);
                
                akticz = 0;
                
                if(kierunek==1) {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[0].param[0];
                    metry = 0;
                } else {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[trackDB->trackNodes[aktt]->iTrv-1].param[0];
                    metry = trackDB->tsection->sekcja.at(idx)->getDlugosc();
                }

                return false;
            case 1:
                u = trackDB->trackNodes[aktt]->iTrv;
                if(kierunek==1){
                    metry+=metrpp;
                    metrpp = 0;
                    if(metry>=trackDB->tsection->sekcja.at(idx)->getDlugosc()){
                         metrpp = metry - trackDB->tsection->sekcja.at(idx)->getDlugosc();
                         akticz++;
                         metry = 0;
                    }
                    i = akticz;
                    
                    if(i==0){
                        //System.out.println("Wjechales na sciezke "+aktt+" z: "+ u +" czesciami. Jedziesz od poczatku do konca");
                    }
                    if(i<u){
                        //System.out.println("czesc" + i);
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[0];
                        pozW.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[10];
                        pozW.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[11];
                        pozW.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[12];
                        pozO.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[13];
                        pozO.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[14];
                        pozO.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[15];
                        pozT.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[8];
                        pozT.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[9];
                        return true;
                    }
                    
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][1] = Math.abs(kierunek-1);
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][0] = aktt;
                            
                    kierunek = trackDB->trackNodes[aktt]->TrPinK[1];
                    aktt = trackDB->trackNodes[aktt]->TrPinS[1];
                    //System.out.println("zjezdzasz z toru na " + aktt);
                    return false;
                }
                if(kierunek==0){
                    metry-=metrpp;
                    metrpp = 0;
                         
                    if(metry<=0){
                         metrpp = -metry;
                         akticz++;
                         //System.out.println("++ ");
                         i=u-akticz-1;
                         if(i>=0){
                            idx = trackDB->trackNodes[aktt]->trVectorSection[i].param[0];
                            metry=trackDB->tsection->sekcja.at(idx)->getDlugosc();
                         }
                    }
                    i=u-akticz-1;
                    
                    if(i==u-1){
                        //System.out.println("Wjechales na sciezke "+aktt+" z: "+ u +" czesciami. Jedziesz od konca do poczatku");
                    }
                    
                    if(i>=0){
                        //System.out.println("czesc" + i + " m " + metry);
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[0];
                        pozW.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[10];
                        pozW.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[11];
                        pozW.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[12];
                        pozO.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[13];
                        pozO.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[14];
                        pozO.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[15];
                        pozT.x = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[8];
                        pozT.z = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[9];     
                        return true;
                    }
                    akticz = 0;
                    metry = 0;
                    
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][1] = Math.abs(kierunek-1);
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][0] = aktt;
                    
                    kierunek = trackDB->trackNodes[aktt]->TrPinK[0];
                    aktt = trackDB->trackNodes[aktt]->TrPinS[0];
                    //System.out.println("zjezdzasz z toru na " + aktt);
                    return false;
                }
                return false;
            case 2:
                if(kierunek==1){ u = trackDB->trackNodes[aktt]->TrP2;}
                if(kierunek==0){ u = trackDB->trackNodes[aktt]->TrP1;}
                akticz = 0;
                metry = 0;
                pozW.x = trackDB->trackNodes[aktt]->UiD[6];
                pozW.y = trackDB->trackNodes[aktt]->UiD[7];
                pozW.z = trackDB->trackNodes[aktt]->UiD[8];
                pozT.x = (int) trackDB->trackNodes[aktt]->UiD[4];
                pozT.z = (int) trackDB->trackNodes[aktt]->UiD[5];
                
                int kt, at;
                
                if(u==1){
                    //System.out.println("zjezdzasz z rozjazdu "+aktt);
                    kt = trackDB->trackNodes[aktt]->TrPinK[trackDB->trackNodes[aktt]->TrP1*kierunek];
                    at = trackDB->trackNodes[aktt]->TrPinS[trackDB->trackNodes[aktt]->TrP1*kierunek];
                    kierunek=kt; aktt=at;
                    
                    if(kierunek==1) {
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[0].param[0];
                        metry = 0;
                    } else {
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[trackDB->trackNodes[aktt]->iTrv-1].param[0];
                        metry = trackDB->tsection->sekcja.at(idx)->getDlugosc();
                    }
                    
                    return false;
                }
                //System.out.println("rozjazd "+aktt);
                //if(rozjazdy[aktt][0]!=0){
                //    kt = rozjazdy[aktt][1];
                //    at = rozjazdy[aktt][0];
                //    if(koniec) rozjazdy[aktt][0] = 0;
                //} else {
                    //if(rozjazd){
                        //rozjazdy[aktt][1] = 
                kt = trackDB->trackNodes[aktt]->TrPinK[trackDB->trackNodes[aktt]->TrP1*kierunek];
                        //rozjazdy[aktt][0] = 
                at = trackDB->trackNodes[aktt]->TrPinS[trackDB->trackNodes[aktt]->TrP1*kierunek];
                    //} else {
                    //    rozjazdy[aktt][1] = kt = trackDB.trackNodes[aktt].TrPinK[trackDB.trackNodes[aktt].TrP1*kierunek+1];
                    //    rozjazdy[aktt][0] = at = trackDB.trackNodes[aktt].TrPinS[trackDB.trackNodes[aktt].TrP1*kierunek+1];
                    //}
                //}
                kierunek = kt; aktt = at;
                if(kierunek==1) {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[0].param[0];
                    metry = 0;
                } else {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[trackDB->trackNodes[aktt]->iTrv-1].param[0];
                    metry = trackDB->tsection->sekcja.at(idx)->getDlugosc();
                    //System.out.println("metry "+metry);
                }
                return false;
        }
        return false;
}
*/