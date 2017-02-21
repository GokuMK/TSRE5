/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesSoundRegion.h"
#include "SoundRegionObj.h"
#include "SoundList.h"
#include "Game.h"

PropertiesSoundRegion::PropertiesSoundRegion() {
    
    QDoubleValidator* doubleValidator = new QDoubleValidator(-10000, 10000, 6, this); 
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("SoundRegion:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->uid.setDisabled(true);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlist->addRow("UiD:",&this->uid);
    vlist->addRow("Tile X:",&this->tX);
    vlist->addRow("Tile Z:",&this->tY);
    vbox->addItem(vlist);
    
    QLabel * label2 = new QLabel("Position:");
    label2->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("X:",&this->posX);
    vlist->addRow("Y:",&this->posY);
    vlist->addRow("Z:",&this->posZ);
    vbox->addItem(vlist);

    QPushButton *button = new QPushButton("Flip", this);
    vbox->addWidget(button);
    connect(button, SIGNAL(released()), this, SLOT(flip()));

    QLabel *label = new QLabel("Region name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->sName);
    // Edit
    label = new QLabel("Change name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->sources);
    this->sources.setStyleSheet("combobox-popup: 0;");
    QObject::connect(&this->sources, SIGNAL(activated(QString)),
        this, SLOT(sourcesListSelected(QString)));
    
    label = new QLabel("Track Items:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    
    QPushButton *bDeleteSelected = new QPushButton("Delete Selected");
    vbox->addWidget(bDeleteSelected);
    QObject::connect(bDeleteSelected, SIGNAL(released()),
                      this, SLOT(bDeleteSelectedEnabled()));
    QPushButton *bExpandSelected = new QPushButton("Expand");
    vbox->addWidget(bExpandSelected);
    QObject::connect(bExpandSelected, SIGNAL(released()),
                      this, SLOT(bExpandEnabled()));
    
    label = new QLabel("Global settings:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(new QLabel("Max placing radius:"));
    vbox->addWidget(&eMaxPlacingDistance);
    eMaxPlacingDistance.setValidator(doubleValidator);
    QObject::connect(&eMaxPlacingDistance, SIGNAL(textEdited(QString)), this, SLOT(eMaxPlacingDistanceEnabled(QString)));

    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
}

PropertiesSoundRegion::~PropertiesSoundRegion() {
}

void PropertiesSoundRegion::sourcesListSelected(QString val){
    if(sobj == NULL)
        return;
    if(Game::soundList == NULL)
        return;
    Undo::StateBegin();
    Undo::PushGameObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->set("update_type", Game::soundList->regions[val]->id);
    Undo::StateEnd();
    sobj->modified = true;
    this->sName.setText(val);
}

void PropertiesSoundRegion::flip(){
    if(sobj == NULL)
        return;
    Undo::StateBegin();
    Undo::PushGameObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->flip();
    Undo::StateEnd();
}

void PropertiesSoundRegion::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = (WorldObj*)obj;
    sobj = (SoundRegionObj*)obj;
    
    this->infoLabel->setText("Object: "+sobj->type);
    
    this->uid.setText(QString::number(sobj->UiD, 10));
    this->tX.setText(QString::number(sobj->x, 10));
    this->tY.setText(QString::number(-sobj->y, 10));
    this->posX.setText(QString::number(sobj->position[0], 'G', 6));
    this->posY.setText(QString::number(sobj->position[1], 'G', 6));
    this->posZ.setText(QString::number(-sobj->position[2], 'G', 6));
    this->quat.setText(
            QString::number(sobj->qDirection[0], 'G', 4) + " " +
            QString::number(sobj->qDirection[1], 'G', 4) + " " +
            QString::number(-sobj->qDirection[2], 'G', 4) + " " +
            QString::number(sobj->qDirection[3], 'G', 4)
            );
    
    this->sources.clear();
    if(Game::soundList != NULL)
        foreach (SoundListItem* it, Game::soundList->regions){
        //for (auto it = Game::soundList->regions.begin(); it != Game::soundList->regions.end(); ++it ){
            if(it->id == sobj->getSoundregionTrackType())
                this->sName.setText(it->name);
            this->sources.addItem(it->name);
        }
        
    this->eMaxPlacingDistance.setText(QString::number(sobj->MaxPlacingDistance));
}

bool PropertiesSoundRegion::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj != GameObj::worldobj)
        return false;
    if(((WorldObj*)obj)->type == "soundregion")
        return true;
    return false;
}

void PropertiesSoundRegion::eMaxPlacingDistanceEnabled(QString val){
    if(sobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        if(fval > 0)
            sobj->MaxPlacingDistance = fval;
    }
}

void PropertiesSoundRegion::bDeleteSelectedEnabled(){
    if(sobj == NULL)
        return;
    Undo::StateBegin();
    Undo::PushGameObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    sobj->deleteSelectedTrItem();
    Undo::StateEnd();
}

void PropertiesSoundRegion::bExpandEnabled(){
    if(sobj == NULL)
        return;
    Undo::StateBegin();
    Undo::PushGameObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    sobj->expandTrItems();
    Undo::StateEnd();
}