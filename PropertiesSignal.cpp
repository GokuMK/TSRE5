/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesSignal.h"
#include "SignalWindow.h"
#include "SignalObj.h"
#include "Game.h"
#include "TDB.h"
#include "SigCfg.h"
#include "SignalShape.h"
#include "ParserX.h"
#include "Game.h"

PropertiesSignal::PropertiesSignal() {
    signalWindow = new SignalWindow();
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Signal:");
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
    QLabel* label = new QLabel("Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&name);
    label = new QLabel("Description:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&description);
    QPushButton *button = new QPushButton("Flip", this);
    vbox->addWidget(button);
    connect(button, SIGNAL(released()), this, SLOT(flipSignal()));
    chFlipShape.setText("Flip Shape");
    chFlipShape.setChecked(true);
    vbox->addWidget(&chFlipShape);
    label = new QLabel("SubObjects:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    button = new QPushButton("Show list", this);
    vbox->addWidget(button);
    connect(button, SIGNAL(released()), this, SLOT(showSubObjList()));
    
    label = new QLabel("Flags:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    this->flags.setDisabled(true);
    this->flags.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->flags);
    QGridLayout *flagslView = new QGridLayout;
    flagslView->setSpacing(2);
    flagslView->setContentsMargins(0,0,0,0);    
    QPushButton *copyFlags = new QPushButton("Copy Flags", this);
    QObject::connect(copyFlags, SIGNAL(released()),
                      this, SLOT(copyFEnabled()));
    QPushButton *pasteFlags = new QPushButton("Paste", this);
    QObject::connect(pasteFlags, SIGNAL(released()),
                      this, SLOT(pasteFEnabled()));
    flagslView->addWidget(copyFlags,0,0);
    flagslView->addWidget(pasteFlags,0,1);
    vbox->addItem(flagslView);
    checkboxAnim.setText("Animate Object");
    checkboxTerrain.setText("Terrain Object");
    vbox->addWidget(&checkboxAnim);
    QObject::connect(&checkboxAnim, SIGNAL(stateChanged(int)),
                      this, SLOT(checkboxAnimEdited(int)));
    vbox->addWidget(&checkboxTerrain);
    QObject::connect(&checkboxTerrain, SIGNAL(stateChanged(int)),
                      this, SLOT(checkboxTerrainEdited(int)));
    cShadowType.addItem("No Shadow");
    cShadowType.addItem("Round Shadow");
    cShadowType.addItem("Rect. Shadow");
    cShadowType.addItem("Treeline Shadow");
    cShadowType.addItem("Dynamic Shadow");
    cShadowType.setStyleSheet("combobox-popup: 0;");
    vbox->addWidget(&cShadowType);
    QObject::connect(&cShadowType, SIGNAL(currentIndexChanged(int)),
                      this, SLOT(cShadowTypeEdited(int)));
    /*for(int i = 0; i < maxSubObj; i++){
        this->chSub[i].setText("");
        vSub[i].setSpacing(2);
        vSub[i].setContentsMargins(3,0,1,0);    
        dSub[i].setEnabled(false);
        vSub[i].addWidget(&this->chSub[i],0,0);
        vSub[i].addWidget(&this->bSub[i],0,1);
        vSub[i].addWidget(&this->dSub[i],1,0,1,2);
        wSub[i].setLayout(&vSub[i]);
        vbox->addWidget(&wSub[i]);
        wSub[i].hide();

        signalsChSect.setMapping(&chSub[i], i);
        connect(&chSub[i], SIGNAL(clicked()), &signalsChSect, SLOT(map()));
    }*/

    //QObject::connect(&signalsChSect, SIGNAL(mapped(int)),
    //    this, SLOT(chSubEnabled(int)));
    
    QObject::connect(signalWindow, SIGNAL(sendMsg(QString,QString)),
        this, SLOT(msg(QString,QString)));   
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesSignal::~PropertiesSignal() {
}

void PropertiesSignal::msg(QString name, QString val){
    if(name == "enableTool"){
        emit enableTool(val);
    }
}

void PropertiesSignal::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = obj;
    sobj = (SignalObj*)obj;
    this->infoLabel->setText("Object: "+obj->type);
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    
    /*for (int i = 0; i < maxSubObj; i++) {
        this->wSub[i].hide();
        this->chSub[i].setChecked(false);
        this->bSub[i].hide();
        this->bSub[i].setEnabled(false);
    }*/
    
    TDB* tdb = Game::trackDB;
    SignalShape* signalShape = tdb->sigCfg->signalShape[sobj->fileName.toStdString()];
    if(signalShape == NULL){ 
        infoLabel->setText("NULL");
        return;
    }
    
    this->name.setText(sobj->fileName);
    this->description.setText(signalShape->desc);

    signalWindow->showObj(sobj);
    
    QRect rec = QApplication::desktop()->screenGeometry();
    signalWindow->move(rec.width()/2-signalWindow->width()/2 ,rec.height()/2-signalWindow->height()/2);
    
    this->flags.setText(ParserX::MakeFlagsString(obj->staticFlags));
    this->checkboxAnim.blockSignals(true);
    this->checkboxTerrain.blockSignals(true);
    this->cShadowType.blockSignals(true);
    this->checkboxAnim.setChecked(obj->isAnimated());
    this->checkboxTerrain.setChecked(obj->isTerrainObj());
    this->cShadowType.setCurrentIndex((int)obj->getShadowType());
    this->checkboxAnim.blockSignals(false);
    this->checkboxTerrain.blockSignals(false);
    this->cShadowType.blockSignals(false);
}

void PropertiesSignal::updateObj(WorldObj* obj){
    if(sobj == NULL){
        return;
    }
    
    this->signalWindow->updateObj(sobj);
    
}

void PropertiesSignal::flipSignal(){
    if(sobj == NULL)
        return;
    Undo::StateBegin();
    Undo::PushWorldObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB);
    sobj->flip(chFlipShape.isChecked());
    Undo::StateEnd();
}

void PropertiesSignal::showSubObjList(){
    this->signalWindow->show();
}

bool PropertiesSignal::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "signal")
        return true;
    return false;
}

void PropertiesSignal::checkboxAnimEdited(int val){
    if(worldObj == NULL)
        return;
    Undo::SinglePushWorldObjData(worldObj);
    if(val == 2){
        worldObj->setAnimated(true);
    } else {
        worldObj->setAnimated(false);
    }
    this->flags.setText(ParserX::MakeFlagsString(worldObj->staticFlags));
}

void PropertiesSignal::checkboxTerrainEdited(int val){
    if(worldObj == NULL)
        return;
    Undo::SinglePushWorldObjData(worldObj);
    if(val == 2){
        worldObj->setTerrainObj(true);
    } else {
        worldObj->setTerrainObj(false);
    }
    this->flags.setText(ParserX::MakeFlagsString(worldObj->staticFlags));
}

void PropertiesSignal::cShadowTypeEdited(int val){
    if(worldObj == NULL)
        return;
    Undo::SinglePushWorldObjData(worldObj);
    worldObj->setShadowType((WorldObj::ShadowType)val);
    this->flags.setText(ParserX::MakeFlagsString(worldObj->staticFlags));
}