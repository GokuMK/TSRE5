/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesConsist.h"
#include "Game.h"
#include "Consist.h"

PropertiesConsist::PropertiesConsist() {
   QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Consist:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);

    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesConsist::~PropertiesConsist() {
}

void PropertiesConsist::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    consistObj = (Consist*)obj;
    
    this->infoLabel->setText("Object: Consist");

}

void PropertiesConsist::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    consistObj = (Consist*)obj;

}

bool PropertiesConsist::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj == GameObj::consistobj)
        return true;
    return false;
}
