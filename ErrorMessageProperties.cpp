/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ErrorMessageProperties.h"
#include <QDebug>
#include "Game.h"
#include "ErrorMessage.h"
#include "GeoCoordinates.h"
#include "GameObj.h"

ErrorMessageProperties::ErrorMessageProperties(QWidget* parent) : QWidget(parent) {
    setFixedHeight(140);
    //setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(2,2,2,2);
    
    QLabel *label = new QLabel("Selected Message:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,1,0);    
    vlist->setAlignment(Qt::AlignTop);
    vlist->setColumnStretch(1,1);
    int row = 0;
    
    vlist->addWidget(&lMessage,row,0);
    vlist->addWidget(&eMessage,row,1);
    vlist->addWidget(&bSelect,row++,2);
    vlist->addWidget(&lAction,row,0);
    vlist->addWidget(&eAction,row++,1,1,2);
    vlist->addWidget(&lLocation,row,0);
    vlist->addWidget(&eLocation,row,1);
    vlist->addWidget(&bLocation,row++,2);
    lMessage.setText("Message:");
    lMessage.hide();
    eMessage.hide();
    lAction.hide();
    lAction.setText("Description:");
    lAction.setAlignment(Qt::AlignTop);
    eAction.hide();
    lLocation.setText("Location:");
    lLocation.hide();
    eLocation.hide();
    eLocation.setDisabled(true);
    bLocation.hide();
    bSelect.hide();
    
    vbox->addItem(vlist);
    vbox->addStretch(1);
    QObject::connect(&bLocation, SIGNAL(released()), this, SLOT(jumpToLocation()));
    QObject::connect(&bSelect, SIGNAL(released()), this, SLOT(bSelectReleased()));
    
    this->setLayout(vbox);
}

ErrorMessageProperties::~ErrorMessageProperties() {
}

void ErrorMessageProperties::showMessage(ErrorMessage* msg){
    currentMessage = msg;
    lMessage.hide();
    lAction.hide();
    eAction.hide();
    lLocation.hide();
    bLocation.hide();
    eLocation.hide();
    bSelect.hide();
    if(currentMessage == NULL){
        return;
    }

    bSelect.show();
    lMessage.show();
    eMessage.show();
    eMessage.setText(currentMessage->description);
    if(currentMessage->action.length() > 0){
        lAction.show();
        eAction.show();
        eAction.setPlainText(currentMessage->action);
    }
    
    if(currentMessage->obj != NULL){
        bSelect.setEnabled(true);
        bSelect.setText("Select Object");
    } else {
        bSelect.setDisabled(true);
        bSelect.setText("NO OBJECT");
    }
    
    if(currentMessage->coords != NULL){
        lLocation.show();
        eLocation.show();
        eLocation.setText(QString("Tile: ") + QString::number(currentMessage->coords->TileX) + " "+ QString::number(currentMessage->coords->TileZ) + " " + 
        ". Coordinates: " + QString::number(currentMessage->coords->wX) + " "+ QString::number(currentMessage->coords->wY) + " "+ QString::number(currentMessage->coords->wZ) + " ");
        bLocation.setText("Jump");
        bLocation.show();
    }
    
    
}

void ErrorMessageProperties::jumpToLocation(){
    emit jumpTo(currentMessage->coords);
}

void ErrorMessageProperties::bSelectReleased(){
    emit selectObject(currentMessage->obj);
}
