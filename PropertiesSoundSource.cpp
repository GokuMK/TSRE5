/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesSoundSource.h"
#include "SoundSourceObj.h"
#include "Game.h"
#include "SoundList.h"
#include "Undo.h"

PropertiesSoundSource::PropertiesSoundSource() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Sound Source:");
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
    // Info
    QLabel* label = new QLabel("FileName:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->fileName);
    label = new QLabel("Source name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->sName);
    // Edit
    label = new QLabel("Change source:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->sources);
    this->sources.setStyleSheet("combobox-popup: 0;");
    QObject::connect(&this->sources, SIGNAL(activated(QString)),
        this, SLOT(sourcesListSelected(QString)));
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesSoundSource::~PropertiesSoundSource() {
}

void PropertiesSoundSource::sourcesListSelected(QString val){
    if(sobj == NULL)
        return;
    if(Game::soundList == NULL)
        return;
    Undo::SinglePushWorldObjData(worldObj);
    sobj->set("filename", Game::soundList->sources[val]->file1);
    sobj->modified = true;
    this->sName.setText(val);
}

void PropertiesSoundSource::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = (WorldObj*)obj;
    sobj = (SoundSourceObj*)obj;
    this->fileName.setText(sobj->fileName);
    this->uid.setText(QString::number(sobj->UiD, 10));
    this->tX.setText(QString::number(sobj->x, 10));
    this->tY.setText(QString::number(-sobj->y, 10));
    this->infoLabel->setText("Object: "+sobj->type);
    
    this->sources.clear();
    if(Game::soundList != NULL)
        foreach (SoundListItem* it, Game::soundList->sources){
        //for (auto it = Game::soundList->sources.begin(); it != Game::soundList->sources.end(); ++it ){
            if(it->file1.toLower() == sobj->fileName.toLower())
                this->sName.setText(it->name);
            this->sources.addItem(it->name);
        }
}

bool PropertiesSoundSource::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj != GameObj::worldobj)
        return false;
    if(((WorldObj*)obj)->type == "soundsource")
        return true;
    return false;
}