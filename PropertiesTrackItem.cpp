/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesTrackItem.h"
#include "TRitem.h"
#include "Game.h"

PropertiesTrackItem::PropertiesTrackItem() {
   QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Terrain:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QLabel *label = new QLabel("Type:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&eItemType);
    eItemType.setDisabled(true);
    label = new QLabel("Id:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&eItemId);
    eItemId.setDisabled(true);
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesTrackItem::~PropertiesTrackItem() {
}


void PropertiesTrackItem::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    itemObj = (TRitem*)obj;
    
    infoLabel->setText("Object: TrackItem");
    eItemType.setText(itemObj->type);
    eItemId.setText(QString::number(itemObj->trItemId));
    
}

void PropertiesTrackItem::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    itemObj = (TRitem*)obj;

}

bool PropertiesTrackItem::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj == GameObj::tritemobj)
        return true;
    return false;
}