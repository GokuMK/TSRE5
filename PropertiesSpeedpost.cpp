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
#include "Game.h"

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
    vbox->addWidget(&kmm);
    kmm.setStyleSheet("combobox-popup: 0;");
    kmm.addItem("Kilometers");
    kmm.addItem("Miles");
    label2 = new QLabel("Speed for:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    vbox->addWidget(&ptb);
    ptb.addItem("Passenger");
    ptb.addItem("Freight");
    ptb.addItem("Both");
    ptb.setStyleSheet("combobox-popup: 0;");
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
    
    QObject::connect(&speed, SIGNAL(textEdited(QString)),
                      this, SLOT(speedEnabled(QString)));
    QObject::connect(&number, SIGNAL(textEdited(QString)),
                      this, SLOT(numberEnabled(QString)));
    QObject::connect(&kmm, SIGNAL(activated(int)),
        this, SLOT(kmmListSelected(int)));
    QObject::connect(&ptb, SIGNAL(activated(int)),
        this, SLOT(ptbListSelected(int)));
}

PropertiesSpeedpost::~PropertiesSpeedpost() {
}

void PropertiesSpeedpost::speedEnabled(QString val){
    if(sobj == NULL) return;
    int speedval = this->speed.text().toInt(0,10);
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->setSpeed(speedval);
    Undo::StateEnd();
}

void PropertiesSpeedpost::numberEnabled(QString val){
    if(sobj == NULL) return;
    float numberval = this->number.text().toInt(0,10);
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->setNumber(numberval);
    Undo::StateEnd();
}

void PropertiesSpeedpost::kmmListSelected(int val){
    if(sobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->setSpeedUnitId(val);
    Undo::StateEnd();
}

void PropertiesSpeedpost::ptbListSelected(int val){
    if(sobj == NULL) return;

}

void PropertiesSpeedpost::flipSignal(){
    if(sobj == NULL)
        return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->flip(chFlipShape.isChecked());
    Undo::StateEnd();
}

void PropertiesSpeedpost::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = obj;
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
        this->kmm.hide();
        this->number.setDisabled(false);
        this->speed.setText("");
        this->number.setText(QString::number(sobj->getNumber(), 'G', 4));
    }
    if(stype == "warning" || stype == "speedsign"){
        this->speed.setDisabled(false);
        this->kmm.show();
        this->number.setDisabled(true);
        this->number.setText("");
        this->speed.setText(QString::number(sobj->getSpeed(), 'G', 4));
        this->kmm.setCurrentIndex(sobj->getSpeedUnitId());
    }
    if(stype == "resume"){
        this->speed.setDisabled(true);
        this->kmm.hide();
        this->number.setDisabled(true);
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