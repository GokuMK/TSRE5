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
#include "Undo.h"
#include "Game.h"

PropertiesRuler::PropertiesRuler() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Ruler:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
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
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);

    vlist->addRow("Meters: ",&this->lengthM);
    vbox->addItem(vlist);
    
    label = new QLabel("Default Settings:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    checkboxTwoPoint.setText("Only Two-Point Ruler");
    checkboxTwoPoint.setChecked(false);
    vbox->addWidget(&checkboxTwoPoint);
    QObject::connect(&checkboxTwoPoint, SIGNAL(stateChanged(int)),
                      this, SLOT(checkboxTwoPointEdited(int)));
    checkboxDrawPoints.setText("Render points");
    checkboxDrawPoints.setChecked(false);
    vbox->addWidget(&checkboxDrawPoints);
    QObject::connect(&checkboxDrawPoints, SIGNAL(stateChanged(int)),
                      this, SLOT(checkboxDrawPointsEdited(int)));
    label = new QLabel("Experimental:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *button = new QPushButton("Create Road Paths");
    vbox->addWidget(button);
    QObject::connect(button, SIGNAL(released()),
                      this, SLOT(createRoadPathsEdited()));
    button = new QPushButton("Remove Road Paths");
    vbox->addWidget(button);
    QObject::connect(button, SIGNAL(released()),
                      this, SLOT(removeRoadPathsEdited()));
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesRuler::~PropertiesRuler() {
}

void PropertiesRuler::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = (WorldObj*)obj;
    RulerObj* robj = (RulerObj*)obj;
    this->uid.setText(QString::number(robj->UiD, 10));
    this->tX.setText(QString::number(robj->x, 10));
    this->tY.setText(QString::number(-robj->y, 10));
    lengthM.setText(QString::number(robj->getLength(), 'G', 4));
}

void PropertiesRuler::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    worldObj = (WorldObj*)obj;
    RulerObj* robj = (RulerObj*)obj;

    if(!lengthM.hasFocus())
        lengthM.setText(QString::number(robj->getLength(), 'G', 4));

}

void PropertiesRuler::checkboxTwoPointEdited(int val){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    if(val == 2){
        robj->TwoPointRuler = true;
    } else {
        robj->TwoPointRuler = false;
    }
}

void PropertiesRuler::checkboxDrawPointsEdited(int val){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    if(val == 2){
        robj->DrawPoints = true;
    } else {
        robj->DrawPoints = false;
    }
}

void PropertiesRuler::createRoadPathsEdited(){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    //Undo::SinglePushWorldObjData(worldObj);
    robj->createRoadPaths();
}

void PropertiesRuler::removeRoadPathsEdited(){
    if(worldObj == NULL)
        return;
    RulerObj* robj = (RulerObj*)worldObj;
    //Undo::SinglePushWorldObjData(worldObj);
    robj->removeRoadPaths();
}
bool PropertiesRuler::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj != GameObj::worldobj)
        return false;
    if(((WorldObj*)obj)->type == "ruler")
        return true;
    return false;
}

