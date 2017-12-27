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
    infoLabel = new QLabel("ActivityConsist:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Speed:",&eSpeed);
    QObject::connect(&eSpeed, SIGNAL(textEdited(QString)),
                      this, SLOT(eSpeedEnabled(QString)));
    //vlist->addRow("Id:",&eId);
    //vlist->addRow("eId:",&eEid);
    //eObjectType.setDisabled(true);
    //eId.setDisabled(true);
    //eEid.setDisabled(true);
    vbox->addItem(vlist);
    
    QPushButton *bCam = new QPushButton("Get Camera View");
    QObject::connect(bCam, SIGNAL(released()), this, SLOT(bCamEnabled()));
    vbox->addWidget(bCam);
    
        
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
    conObj = (Consist*)obj;
    
    infoLabel->setText("Object: Activity Consist");
    eSpeed.setText(QString::number(conObj->getTrainSpeed()));
}

void PropertiesConsist::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    //conObj = (Consist*)obj;
}


bool PropertiesConsist::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj == GameObj::consistobj)
        return true;
    return false;
}

void PropertiesConsist::bCamEnabled(){
    emit cameraObject((GameObj*)conObj);
}

void PropertiesConsist::eSpeedEnabled(QString val){
    if(conObj == NULL)
        return;
    bool ok = false;
    float speed = val.toFloat(&ok);
    if(ok)
        conObj->setTrainSpeed(speed/3.6);
}