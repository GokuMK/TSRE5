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
#include "ParserX.h"
#include "FileBuffer.h"
#include "GLMatrix.h"
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
    id = orig.id;
    objectType = orig.objectType;
    parentActivity = orig.parentActivity;
    selected = orig.selected;
    tile[0] = orig.tile[0];
    tile[1] = orig.tile[1];
    tile[2] = orig.tile[2];
    tile[3] = orig.tile[3];
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

void ActivityObject::setParentActivity(Activity* a){
    parentActivity = a;
}

QString ActivityObject::getParentName(){
    if(parentActivity == NULL)
        return "";
    return parentActivity->header->name;
}

void ActivityObject::remove(){
    if(parentActivity == NULL)
        return;
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
    
    if(con != NULL){
        if (!con->isOnTrack)
            con->initOnTrack(tile, direction);
        con->renderOnTrack(gluu, playerT, selectionColor);
    }
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
        qDebug() << "#activityObject - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void ActivityObject::save(QTextStream* out) {
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