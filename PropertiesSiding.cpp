/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesSiding.h"
#include "WorldObj.h"
#include "PlatformObj.h"
#include "Game.h"

PropertiesSiding::PropertiesSiding() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Platform:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->uid.setDisabled(true);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    this->lengthPlatform.setDisabled(true);
    vlist->addRow("UiD:",&this->uid);
    vlist->addRow("Tile X:",&this->tX);
    vlist->addRow("Tile Z:",&this->tY);
    vlist->addRow("Length:",&this->lengthPlatform);
    vbox->addItem(vlist);
    // name
    QLabel* label = new QLabel("Siding Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->namePlatform);
    // misc
    label = new QLabel("Misc:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    disablePlatform.setText("Disable Platform");
    vbox->addWidget(&disablePlatform);
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&disablePlatform, SIGNAL(stateChanged(int)),
                      this, SLOT(disablePlatformEnabled(int)));
    QObject::connect(&namePlatform, SIGNAL(textEdited(QString)),
                      this, SLOT(namePlatformEnabled(QString)));
}

PropertiesSiding::~PropertiesSiding() {
}

void PropertiesSiding::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = obj;
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->infoLabel->setText("Object: "+obj->type);
    pobj = (PlatformObj*)obj;
    this->lengthPlatform.setText(QString::number(pobj->getLength())+" m");
    this->namePlatform.setText(pobj->getPlatformName());
    this->disablePlatform.setChecked(pobj->getDisabled());
}

void PropertiesSiding:: disablePlatformEnabled(int state){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    if(state == Qt::Checked)
        pobj->setDisabled(true);
    else
        pobj->setDisabled(false);
    Undo::StateEnd();
}

void PropertiesSiding::namePlatformEnabled(QString val){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    pobj->setPlatformName(val);
    Undo::StateEnd();
}

bool PropertiesSiding::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "siding")
        return true;
    return false;
}