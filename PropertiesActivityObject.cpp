/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesActivityObject.h"
#include "Game.h"
#include "ActivityObject.h"
#include "Activity.h"

PropertiesActivityObject::PropertiesActivityObject() {
   QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("ActivityObject:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Type:",&eObjectType);
    vlist->addRow("Id:",&eId);
    eObjectType.setDisabled(true);
    eId.setDisabled(true);
    vbox->addItem(vlist);
    
    QPushButton *bDelete = new QPushButton("Delete");
    QObject::connect(bDelete, SIGNAL(released()), this, SLOT(bDeleteEnabled()));
    vbox->addWidget(bDelete);
    
    QLabel *label = new QLabel("Owned by:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&eActivityName);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesActivityObject::~PropertiesActivityObject() {
}

void PropertiesActivityObject::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    actObj = (ActivityObject*)obj;
    
    infoLabel->setText("Object: ActivityObject");
    eObjectType.setText(actObj->objectType);
    eId.setText(QString::number(actObj->id));
    eActivityName.setText(actObj->getParentName());

}

void PropertiesActivityObject::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    actObj = (ActivityObject*)obj;

}

void PropertiesActivityObject::bDeleteEnabled(){
    if(actObj == NULL){
        return;
    }
    actObj->remove();
}

bool PropertiesActivityObject::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj == GameObj::activityobj)
        return true;
    return false;
}
