/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesRuler.h"
#include "RulerObj.h"

PropertiesRuler::PropertiesRuler() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Ruler:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
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
    QLabel *label = new QLabel("Length:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);

    vlist->addRow("Meters: ",&this->lengthM);
    vbox->addItem(vlist);

    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesRuler::~PropertiesRuler() {
}

void PropertiesRuler::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = obj;
    RulerObj* robj = (RulerObj*)obj;
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    lengthM.setText(QString::number(robj->getLength(), 'G', 4));
}

void PropertiesRuler::updateObj(WorldObj* obj){
    if(obj == NULL){
        return;
    }
    RulerObj* robj = (RulerObj*)obj;

    if(!lengthM.hasFocus())
        lengthM.setText(QString::number(robj->getLength(), 'G', 4));

}

bool PropertiesRuler::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "ruler")
        return true;
    return false;
}

