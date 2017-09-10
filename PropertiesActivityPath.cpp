/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesActivityPath.h"
#include "Path.h"
#include "Game.h"

PropertiesActivityPath::PropertiesActivityPath() {
QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Path:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    //QFormLayout *vlist = new QFormLayout;
    vbox->addWidget(new QLabel("File Name"));
    vbox->addWidget(&this->ePathFName);
    vbox->addWidget(new QLabel("Display Name"));
    vbox->addWidget(&this->eName);
    vbox->addWidget(new QLabel("Start location"));
    vbox->addWidget(&this->ePathStart);
    vbox->addWidget(new QLabel("End Location"));
    vbox->addWidget(&this->ePathEnd);
    //vlist->setSpacing(2);
    //vlist->setContentsMargins(3,0,3,0);
    //vbox->addItem(vlist);

    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesActivityPath::~PropertiesActivityPath() {
}

void PropertiesActivityPath::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    pathObj = (Path*)obj;
    this->infoLabel->setText("Object: Path");
    this->ePathFName.setText(pathObj->trPathName);
    this->eName.setText(pathObj->displayName);
    this->ePathStart.setText(pathObj->trPathStart);
    this->ePathEnd.setText(pathObj->trPathEnd);
}

bool PropertiesActivityPath::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj == GameObj::activitypath)
        return true;
    return false;
}