/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TDBClient.h"
#include "RouteEditorClient.h"
#include "TRitem.h"
#include "TSectionDAT.h"

TDBClient::TDBClient(TSectionDAT* tsection, bool road) : TDB(tsection, road){

}

TDBClient::TDBClient(const TDBClient& o) : TDB(o){

}

void TDBClient::updateTrNode(int nid){
    if(nid < 0)
        return;
    Game::serverClient->updateTrackNodeData(nid, this->tdbId, this->trackNodes[nid]);
}

void TDBClient::updateTrItem(int iid){
    if(iid < 0)
        return;
    Game::serverClient->updateTrackItemData(iid, this->tdbId, this->trackItems[iid]);
}

void TDBClient::updateTrackSection(int id){
    Game::serverClient->updateTrackSectionData(this->tsection->sekcja[id]);
}

void TDBClient::updateTrackShape(int id){
    Game::serverClient->updateTrackShapeData(this->tsection->shape[id]);
}
int TDBClient::getNextItrNode(){
    if(road)
        Game::serverClient->sendUtf16Message("increase_rdb_itrnode ( ) \n");
    else
        Game::serverClient->sendUtf16Message("increase_tdb_itrnode ( ) \n");
    return ++this->iTRnodes;
}

int TDBClient::getNewTRitemId(){
    for (int i = 0; i < this->iTRitems; i++) {
        if(this->trackItems[i] == NULL)
            continue;
        if(Game::useTdbEmptyItems)
            if(this->trackItems[i]->type == "emptyitem"){
                return i;
            }
    }
    if(road)
        Game::serverClient->sendUtf16Message("increase_rdb_itritem ( ) \n");
    else
        Game::serverClient->sendUtf16Message("increase_tdb_itritem ( ) \n");
    return this->iTRitems++;
}

TDBClient::~TDBClient() {
    //TDB::~TDB();
}

