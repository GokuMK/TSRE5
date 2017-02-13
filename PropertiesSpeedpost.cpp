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
    
    QDoubleValidator* doubleValidator = new QDoubleValidator(-10000, 10000, 6, this); 
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    QDoubleValidator* doubleValidator2 = new QDoubleValidator(-10000, 10000, 2, this); 
    doubleValidator2->setNotation(QDoubleValidator::StandardNotation);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("SpeedPost:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->uid.setDisabled(true);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlist->addRow("Type:",&this->speedpostType);
    speedpostType.setDisabled(true);
    vlist->addRow("UiD:",&this->uid);
    vlist->addRow("Tile X:",&this->tX);
    vlist->addRow("Tile Z:",&this->tY);
    vbox->addItem(vlist);
    
    QLabel * label = new QLabel("Position:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("X:",&this->posX);
    vlist->addRow("Y:",&this->posY);
    vlist->addRow("Z:",&this->posZ);
    vbox->addItem(vlist);
    speedlabel = new QLabel("Speed:");
    speedlabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    speedlabel->setContentsMargins(3,0,0,0);
    chCustomSpeed.setText("Speed instead of Number");
    vbox->addWidget(speedlabel);
    vbox->addWidget(&chCustomSpeed);
    vbox->addWidget(&speed);
    vbox->addWidget(&kmm);
    kmm.setStyleSheet("combobox-popup: 0;");
    kmm.addItem("Kilometers");
    kmm.addItem("Miles");
    lSpeedFor = new QLabel("Speed for:");
    lSpeedFor->setContentsMargins(3,0,0,0);
    vbox->addWidget(lSpeedFor);
    vbox->addWidget(&ptb);
    ptb.addItem("Passenger");
    ptb.addItem("Freight");
    ptb.addItem("Both");
    ptb.setStyleSheet("combobox-popup: 0;");
    numberlabel = new QLabel("Number:");
    numberlabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    numberlabel->setContentsMargins(3,0,0,0);
    chCustomNumber.setText("Number instead of Speed");
    vbox->addWidget(numberlabel);
    vbox->addWidget(&chCustomNumber);
    vbox->addWidget(&number);
    number.setValidator(doubleValidator2);
    chNumberDot.setText("Show Dot");
    vbox->addWidget(&chNumberDot);

    label = new QLabel("Track Items:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *button = new QPushButton("Flip", this);
    vbox->addWidget(button);
    connect(button, SIGNAL(released()), this, SLOT(flipSignal()));
    chFlipShape.setText("Flip Shape");
    chFlipShape.setChecked(true);
    vbox->addWidget(&chFlipShape);
    QPushButton *bDeleteSelected = new QPushButton("Delete Selected");
    vbox->addWidget(bDeleteSelected);
    QObject::connect(bDeleteSelected, SIGNAL(released()),
                      this, SLOT(bDeleteSelectedEnabled()));
    QPushButton *bExpandSelected = new QPushButton("Expand");
    vbox->addWidget(bExpandSelected);
    QObject::connect(bExpandSelected, SIGNAL(released()),
                      this, SLOT(bExpandEnabled()));
    
    label = new QLabel("Global settings:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(new QLabel("Max placing radius:"));
    vbox->addWidget(&eMaxPlacingDistance);
    eMaxPlacingDistance.setValidator(doubleValidator);
    QObject::connect(&eMaxPlacingDistance, SIGNAL(textEdited(QString)), this, SLOT(eMaxPlacingDistanceEnabled(QString)));
    
    
    QObject::connect(&speed, SIGNAL(textEdited(QString)),
                      this, SLOT(speedEnabled(QString)));
    QObject::connect(&number, SIGNAL(textEdited(QString)),
                      this, SLOT(numberEnabled(QString)));
    QObject::connect(&chNumberDot, SIGNAL(stateChanged(int)),
                      this, SLOT(numberDotEnabled(int)));
    QObject::connect(&chCustomNumber, SIGNAL(stateChanged(int)),
                      this, SLOT(chCustomNumberEnabled(int)));
    QObject::connect(&chCustomSpeed, SIGNAL(stateChanged(int)),
                      this, SLOT(chCustomSpeedEnabled(int)));
    QObject::connect(&kmm, SIGNAL(activated(int)),
        this, SLOT(kmmListSelected(int)));
    QObject::connect(&ptb, SIGNAL(activated(int)),
        this, SLOT(ptbListSelected(int)));
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesSpeedpost::~PropertiesSpeedpost() {
}

void PropertiesSpeedpost::speedEnabled(QString val){
    if(sobj == NULL) return;
    bool ok = false;
    int speedval = this->speed.text().toInt(&ok);
    if(!ok)
        return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->setSpeed(speedval);
    Undo::StateEnd();
}

void PropertiesSpeedpost::numberEnabled(QString val){
    if(sobj == NULL) return;
    bool ok = false;
    float numberval = this->number.text().toFloat(&ok);
    if(!ok)
        return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->setNumber(numberval);
    Undo::StateEnd();
}

void PropertiesSpeedpost::numberDotEnabled(int val){
    if(sobj == NULL) return;
    qDebug()<<"aaa";
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    if(val == 2)
        sobj->setNumberDot(true);
    else
        sobj->setNumberDot(false);
    Undo::StateEnd();
}

void PropertiesSpeedpost::chCustomNumberEnabled(int val){
    if(sobj == NULL) return;
    qDebug()<<"aaa";
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    if(val == 2){
        sobj->setNumberInsteadSpeed(true);
        this->number.show();
        this->chNumberDot.show();
        this->numberlabel->show();
    }else{
        sobj->setNumberInsteadSpeed(false);
        this->number.hide();
        this->chNumberDot.hide();
        this->numberlabel->hide();
    }
    Undo::StateEnd();
}

void PropertiesSpeedpost::chCustomSpeedEnabled(int val){
    if(sobj == NULL) return;
    qDebug()<<"aaa";
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    if(val == 2){
        sobj->setSpeedInsteadNumber(true);
        this->speedlabel->show();
        this->speed.show();
        this->kmm.show();
    }else{
        sobj->setSpeedInsteadNumber(false);
        this->speedlabel->hide();
        this->speed.hide();
        this->kmm.hide();
    }
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
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    sobj->setTrainType(ptb.currentIndex());
    Undo::StateEnd();
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
    this->posX.setText(QString::number(obj->position[0], 'G', 6));
    this->posY.setText(QString::number(obj->position[1], 'G', 6));
    this->posZ.setText(QString::number(-obj->position[2], 'G', 6));
    this->quat.setText(
            QString::number(obj->qDirection[0], 'G', 4) + " " +
            QString::number(obj->qDirection[1], 'G', 4) + " " +
            QString::number(-obj->qDirection[2], 'G', 4) + " " +
            QString::number(obj->qDirection[3], 'G', 4)
            );
    
    this->chCustomNumber.blockSignals(true);
    this->chCustomSpeed.blockSignals(true);
    this->chNumberDot.blockSignals(true);
    
    this->ptb.setCurrentIndex(sobj->getTrainType());
    this->eMaxPlacingDistance.setText(QString::number(sobj->MaxPlacingDistance));
    this->speed.setText(QString::number(sobj->getSpeed(), 'G', 4));
    this->number.setText(QString::number(sobj->getNumber(), 'G', 4));
    this->kmm.setCurrentIndex(sobj->getSpeedUnitId());
    this->chNumberDot.setChecked(sobj->isNumberDot());
    this->chCustomSpeed.setChecked(sobj->getSpeedInsteadNumber());
    this->chCustomNumber.setChecked(sobj->getNumberInsteadSpeed());
    this->chCustomSpeed.hide();
    this->speed.hide();
    this->kmm.hide();
    this->chCustomNumber.hide();
    this->number.hide();
    this->chNumberDot.hide();
    this->speedlabel->hide();
    this->numberlabel->hide();
    this->lSpeedFor->hide();
    this->ptb.hide();
    
    if(stype == "milepost"){
        if(sobj->getSpeedInsteadNumber()){
            this->speedlabel->show();
            this->speed.show();
            this->kmm.show();
        }
        this->chCustomSpeed.show();
        this->number.show();
        this->chNumberDot.show();
        this->numberlabel->show();
    }
    if(stype == "warning" || stype == "speedsign"){
        this->speed.show();
        this->kmm.show();
        if(sobj->getNumberInsteadSpeed()){
            this->number.show();
            this->chNumberDot.show();
            this->numberlabel->show();
        }
        this->chCustomNumber.show();
        this->speedlabel->show();
        this->lSpeedFor->show();
        this->ptb.show();
    }
    if(stype == "resume"){
        if(sobj->getNumberInsteadSpeed()){
            this->number.show();
            this->chNumberDot.show();
            this->numberlabel->show();
        }
        this->chCustomNumber.show();
    }
    this->chNumberDot.blockSignals(false);
    this->chCustomNumber.blockSignals(false);
    this->chCustomSpeed.blockSignals(false);
}

void PropertiesSpeedpost::updateObj(WorldObj* obj){
    if(obj == NULL){
        return;
    }

    if(!posX.hasFocus() && !posY.hasFocus() && !posZ.hasFocus() && !quat.hasFocus()){
        this->uid.setText(QString::number(obj->UiD, 10));
        this->tX.setText(QString::number(obj->x, 10));
        this->tY.setText(QString::number(-obj->y, 10));
        this->posX.setText(QString::number(obj->position[0], 'G', 6));
        this->posY.setText(QString::number(obj->position[1], 'G', 6));
        this->posZ.setText(QString::number(-obj->position[2], 'G', 6));
        this->quat.setText(
                QString::number(obj->qDirection[0], 'G', 4) + " " +
                QString::number(obj->qDirection[1], 'G', 4) + " " +
                QString::number(-obj->qDirection[2], 'G', 4) + " " +
                QString::number(obj->qDirection[3], 'G', 4)
                );
    }
    

}

bool PropertiesSpeedpost::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "speedpost")
        return true;
    return false;
}

void PropertiesSpeedpost::eMaxPlacingDistanceEnabled(QString val){
    if(sobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        if(fval > 0)
            sobj->MaxPlacingDistance = fval;
    }
}

void PropertiesSpeedpost::bDeleteSelectedEnabled(){
    if(sobj == NULL)
        return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    sobj->deleteSelectedTrItem();
    Undo::StateEnd();
}

void PropertiesSpeedpost::bExpandEnabled(){
    if(sobj == NULL)
        return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    sobj->expandTrItems();
    Undo::StateEnd();
}