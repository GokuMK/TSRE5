/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesPickup.h"
#include "PickupObj.h"
#include "GuiFunct.h"

PropertiesPickup::PropertiesPickup() {
    cPickupType.addItem("Freight - grain");
    cPickupType.addItem("Freight - coal");
    cPickupType.addItem("Freight - gravel");
    cPickupType.addItem("Freight - sand");
    cPickupType.addItem("Fuel - water");
    cPickupType.addItem("Fuel - coal");
    cPickupType.addItem("Fuel - diesel");
    cPickupType.addItem("Special - mail");

    cAnimType.addItem("Activated animation only");
    cAnimType.addItem("Proximity animation only");
    cAnimType.addItem("Activated effects only");
    cAnimType.addItem("Activated anim. and effects");
    cAnimType.addItem("Proximity anim. and effects");
    cAnimType.addItem("On empty animation only");
    cAnimType.addItem("Fuel hose only");
            
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Pickup:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);

    QLabel *label = new QLabel("Filename:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    this->fileName.setDisabled(true);
    this->fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->fileName);
    label = new QLabel("Filename - broken:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    this->eBrokenFileName.setDisabled(true);
    this->eBrokenFileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->eBrokenFileName);
    label = new QLabel("Properties:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    label = new QLabel("Type:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cPickupType);
    vlist->addRow("Capacity:",&eCapacity);
    vlist->addRow("Content:",&eContent);
    vlist->addRow("Fill rate:",&eFill);
    vlist->addRow("Speed min:",&eSpeedMin);
    vlist->addRow("Speed max:",&eSpeedMax);
    vbox->addItem(vlist);
    label = new QLabel("Anim type:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cAnimType);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Anim lenght:",&eAnimLength);
    vbox->addItem(vlist);
    vbox->addWidget(&chInfinite);
    vbox->addWidget(&chBroken);
    chInfinite.setText("Infinite capacity");
    chBroken.setText("Broken by default");
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    cPickupType.setStyleSheet("combobox-popup: 0;");
    cAnimType.setStyleSheet("combobox-popup: 0;");
}

PropertiesPickup::~PropertiesPickup() {
}

void PropertiesPickup::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    pobj = (PickupObj*)obj;
    
    this->infoLabel->setText("Object: "+obj->type);
    this->fileName.setText(pobj->fileName);
    this->eBrokenFileName.setText(pobj->fileName.split('.')[0]+"_d.s");
    cPickupType.setCurrentIndex(pobj->pickupType[0]-1);
    eCapacity.setText(QString::number(pobj->pickupCapacity1, 10));
    eContent.setText(QString::number(pobj->getPickupContent()));
    eFill.setText(QString::number(pobj->pickupCapacity2));
    eSpeedMin.setText(QString::number(pobj->speedRange[0]));
    eSpeedMax.setText(QString::number(pobj->speedRange[1]));
    cAnimType.setCurrentIndex(pobj->pickupAnimData1);
    eAnimLength.setText(QString::number(pobj->pickupAnimData2));
    chInfinite.setChecked((pobj->pickupType[1] == 1));
    
}

bool PropertiesPickup::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "pickup")
        return true;
    return false;
}