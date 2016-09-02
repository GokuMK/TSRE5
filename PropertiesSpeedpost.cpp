/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesSpeedpost.h"
#include "SpeedpostObj.h"
#include "TRitem.h"

PropertiesSpeedpost::PropertiesSpeedpost() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("SpeedPost:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    vbox->addWidget(&speedpostType);
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
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("X:",&this->posX);
    vlist->addRow("Y:",&this->posY);
    vlist->addRow("Z:",&this->posZ);
    vbox->addItem(vlist);
    label2 = new QLabel("Speed:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    vbox->addWidget(&speed);
    label2 = new QLabel("Speed for:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    vbox->addWidget(&ptb);
    ptb.addItem("Passenger");
    ptb.addItem("Freight");
    ptb.addItem("Both");
    label2 = new QLabel("Number:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    vbox->addWidget(&number);
    QPushButton *button = new QPushButton("Flip", this);
    vbox->addWidget(button);
    connect(button, SIGNAL(released()), this, SLOT(flipSignal()));
    chFlipShape.setText("Flip Shape");
    chFlipShape.setChecked(true);
    vbox->addWidget(&chFlipShape);
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&speed, SIGNAL(textChanged(QString)),
                      this, SLOT(speedEnabled(QString)));
    QObject::connect(&number, SIGNAL(textChanged(QString)),
                      this, SLOT(numberEnabled(QString)));
}

PropertiesSpeedpost::~PropertiesSpeedpost() {
}

void PropertiesSpeedpost::speedEnabled(QString val){
    if(sobj == NULL) return;
    int speedval = this->speed.text().toInt(0,10);
    sobj->setSpeed(speedval);
}

void PropertiesSpeedpost::numberEnabled(QString val){
    if(sobj == NULL) return;
    float numberval = this->number.text().toInt(0,10);
    sobj->setNumber(numberval);
}

void PropertiesSpeedpost::flipSignal(){
    if(sobj == NULL)
        return;
    sobj->flip(chFlipShape.isChecked());
}

void PropertiesSpeedpost::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    sobj = (SpeedpostObj*)obj;
    QString stype = sobj->getSpeedpostType();
    this->infoLabel->setText("Object: "+obj->type);
    this->speedpostType.setText(stype);
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->posX.setText(QString::number(obj->position[0], 'G', 4));
    this->posY.setText(QString::number(obj->position[1], 'G', 4));
    this->posZ.setText(QString::number(-obj->position[2], 'G', 4));
    this->quat.setText(
            QString::number(obj->qDirection[0], 'G', 4) + " " +
            QString::number(obj->qDirection[1], 'G', 4) + " " +
            QString::number(-obj->qDirection[2], 'G', 4) + " " +
            QString::number(obj->qDirection[3], 'G', 4)
            );
    
    if(stype == "milepost"){
        this->speed.setDisabled(true);
        this->number.setDisabled(false);
        this->speed.setText("");
        this->number.setText(QString::number(sobj->getNumber(), 'G', 4));
    }
    if(stype == "warning" || stype == "speedsign"){
        this->speed.setDisabled(false);
        this->number.setDisabled(true);
        this->number.setText("");
        this->speed.setText(QString::number(sobj->getSpeed(), 'G', 4));
    }
    if(stype == "resume"){
        this->speed.setDisabled(false);
        this->number.setDisabled(false);
        this->number.setText("");
        this->speed.setText("");
    }
}

bool PropertiesSpeedpost::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "speedpost")
        return true;
    return false;
}