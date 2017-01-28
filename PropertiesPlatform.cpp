/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesPlatform.h"
#include "PlatformObj.h"
#include "Game.h"

PropertiesPlatform::PropertiesPlatform() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Platform:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->uid.setDisabled(true);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    this->lengthPlatform.setDisabled(true);
    vlist->addRow("UiD:",&this->uid);
    vlist->addRow("Tile X:",&this->tX);
    vlist->addRow("Tile Z:",&this->tY);
    vlist->addRow("Length:",&this->lengthPlatform);
    vbox->addItem(vlist);
    // names
    QLabel * label = new QLabel("Station Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->nameStation);
    label = new QLabel("Platform Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->namePlatform);
    // side
    label = new QLabel("Side:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    leftSide.setText("Left");
    rightSide.setText("Right");
    vbox->addWidget(&leftSide);
    vbox->addWidget(&rightSide);
    // wait
    label = new QLabel("Platform Wait:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Minutes:",&this->waitMin);
    vlist->addRow("Seconds:",&this->waitSec);
    vlist->addRow("Passengers:",&this->waitPas);
    vbox->addItem(vlist);
    // misc
    label = new QLabel("Misc:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    disablePlatform.setText("Disable Platform");
    vbox->addWidget(&disablePlatform);
    vbox->addStretch(1);
    this->setLayout(vbox);

    waitMin.setValidator( new QIntValidator(0, 100, this) );
    waitSec.setValidator( new QIntValidator(0, 60, this) );
    waitPas.setValidator( new QIntValidator(0, 999, this) );
    
    QObject::connect(&leftSide, SIGNAL(stateChanged(int)),
                      this, SLOT(leftSideEnabled(int)));
    QObject::connect(&rightSide, SIGNAL(stateChanged(int)),
                      this, SLOT(rightSideEnabled(int)));
    QObject::connect(&disablePlatform, SIGNAL(stateChanged(int)),
                      this, SLOT(disablePlatformEnabled(int)));
    QObject::connect(&nameStation, SIGNAL(textEdited(QString)),
                      this, SLOT(nameStationEnabled(QString)));
    QObject::connect(&namePlatform, SIGNAL(textEdited(QString)),
                      this, SLOT(namePlatformEnabled(QString)));
    QObject::connect(&waitMin, SIGNAL(textEdited(QString)),
                      this, SLOT(waitMinEnabled(QString)));
    QObject::connect(&waitSec, SIGNAL(textEdited(QString)),
                      this, SLOT(waitSecEnabled(QString)));
    QObject::connect(&waitPas, SIGNAL(textEdited(QString)),
                      this, SLOT(waitPasEnabled(QString)));
}

PropertiesPlatform::~PropertiesPlatform() {
}

void PropertiesPlatform::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = obj;
    pobj = (PlatformObj*)obj;
    this->infoLabel->setText("Object: "+obj->type);
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->lengthPlatform.setText(QString::number(pobj->getLength())+" m");
    this->nameStation.setText(pobj->getStationName());
    this->namePlatform.setText(pobj->getPlatformName());
    int sec = pobj->getPlatformMinWaitingTime();
    int min = sec/60;
    sec = sec - min*60;
    this->waitMin.setText(QString::number(min, 10));
    this->waitSec.setText(QString::number(sec, 10));
    this->waitPas.setText(QString::number(pobj->getPlatformNumPassengersWaiting(), 10));
    this->leftSide.blockSignals(true);
    this->leftSide.setChecked(pobj->getSideLeft());
    this->leftSide.blockSignals(false);
    this->rightSide.blockSignals(true);
    this->rightSide.setChecked(pobj->getSideRight());
    this->rightSide.blockSignals(false);
    this->disablePlatform.blockSignals(true);
    this->disablePlatform.setChecked(pobj->getDisabled());
    this->disablePlatform.blockSignals(false);
}

bool PropertiesPlatform::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "platform")
        return true;
    return false;
}

void PropertiesPlatform::leftSideEnabled(int state){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    if(state == Qt::Checked)
        pobj->setSideLeft(true);
    else
        pobj->setSideLeft(false);
    Undo::StateEnd();
}
void PropertiesPlatform::rightSideEnabled(int state){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    if(state == Qt::Checked)
        pobj->setSideRight(true);
    else
        pobj->setSideRight(false);
    Undo::StateEnd();
}
void PropertiesPlatform:: disablePlatformEnabled(int state){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    if(state == Qt::Checked)
        pobj->setDisabled(true);
    else
        pobj->setDisabled(false);
    Undo::StateEnd();
}
void PropertiesPlatform::nameStationEnabled(QString val){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    pobj->setStationName(val);
    Undo::StateEnd();
}
void PropertiesPlatform::namePlatformEnabled(QString val){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    pobj->setPlatformName(val);
    Undo::StateEnd();
}
void PropertiesPlatform::waitMinEnabled(QString val){
    if(pobj == NULL) return;
    int min = this->waitMin.text().toInt(0,10);
    int sec = this->waitSec.text().toInt(0,10);
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    pobj->setPlatformMinWaitingTime(min*60+sec);
    Undo::StateEnd();
}
void PropertiesPlatform::waitSecEnabled(QString val){
    if(pobj == NULL) return;
    int min = this->waitMin.text().toInt(0,10);
    int sec = this->waitSec.text().toInt(0,10);
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    pobj->setPlatformMinWaitingTime(min*60+sec);
    Undo::StateEnd();
}
void PropertiesPlatform::waitPasEnabled(QString val){
    if(pobj == NULL) return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    pobj->setPlatformNumPassengersWaiting(val.toInt(0,10));
    Undo::StateEnd();
}