/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesCarspawner.h"
#include "WorldObj.h"
#include "PlatformObj.h"

PropertiesCarspawner::PropertiesCarspawner() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Carspawner:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
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
    QLabel * label = new QLabel("Car Number:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->carNumber);
    label = new QLabel("Car speed:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->carSpeed);
    vbox->addWidget(&this->useCustomList);
    useCustomList.setText("Use custom car list.");
    vbox->addWidget(&this->carspawnList);
    carspawnList.setStyleSheet("combobox-popup: 0;");

    vbox->addStretch(1);
    this->setLayout(vbox);
    
    carNumber.setValidator( new QDoubleValidator(0, 100, 1, this) );
    carSpeed.setValidator( new QDoubleValidator(0, 100, 1, this) );
    
    QObject::connect(&carNumber, SIGNAL(textChanged(QString)),
                      this, SLOT(carNumberEnabled(QString)));
    QObject::connect(&carSpeed, SIGNAL(textChanged(QString)),
                      this, SLOT(carSpeedEnabled(QString)));
    QObject::connect(&useCustomList, SIGNAL(stateChanged(int)),
                      this, SLOT(useCustomListEnabled(int)));
    QObject::connect(&carspawnList, SIGNAL(activated(QString)),
                      this, SLOT(carspawnListSelected(QString)));
}

PropertiesCarspawner::~PropertiesCarspawner() {
}

void PropertiesCarspawner::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    this->infoLabel->setText("Object: "+obj->type);
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    cobj = (PlatformObj*)obj;
    this->lengthPlatform.setText(QString::number(cobj->getLength())+" m");
    this->carNumber.setText(QString::number(cobj->getCarNumber(),10));
    this->carSpeed.setText(QString::number(cobj->getCarSpeed(),10));
    
    carspawnList.clear();
    carspawnList.setDisabled(true);
    useCustomList.blockSignals(true);
    useCustomList.setChecked(false);
    QString clist = cobj->getCarListName();
    if(clist.length() > 0){
        useCustomList.setChecked(true);
        carspawnList.setEnabled(true);
        for(int i = 0; i < cobj->CarSpawnerList.length(); i++ ){
            carspawnList.addItem(cobj->CarSpawnerList[i], i);
            if(cobj->CarSpawnerList[i].toLower() == clist.toLower() )
                carspawnList.setCurrentIndex(i);
        }
    }
    useCustomList.blockSignals(false);
}

void PropertiesCarspawner::carNumberEnabled(QString val){
    if(cobj == NULL) return;
    cobj->setCarNumber(val.toInt());
}

void PropertiesCarspawner::carSpeedEnabled(QString val){
    if(cobj == NULL) return;
    cobj->setCarSpeed(val.toInt());
}

bool PropertiesCarspawner::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "carspawner")
        return true;
    return false;
}

void PropertiesCarspawner::carspawnListSelected(QString val){
    if(cobj == NULL)
        return;
    cobj->setCarListName(val);
}

void PropertiesCarspawner::useCustomListEnabled(int val){
    if(cobj == NULL)
        return;
    if(val == 2){
        if(cobj->CarSpawnerList.length() == 0)
            return;
        for(int i = 0; i < cobj->CarSpawnerList.length(); i++ ){
            carspawnList.addItem(cobj->CarSpawnerList[i], i);
        }
        cobj->setCarListName(carspawnList.currentText());
        carspawnList.setEnabled(true);
    } else {
        carspawnList.clear();
        carspawnList.setDisabled(true);
        cobj->setCarListName("");
    }
}