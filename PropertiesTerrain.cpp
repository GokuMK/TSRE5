/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesTerrain.h"
#include "GameObj.h"
#include "Terrain.h"
#include "Game.h"

PropertiesTerrain::PropertiesTerrain() {
   QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Terrain:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlist->addRow("Tile X:",&this->tX);
    vlist->addRow("Tile Z:",&this->tY);
    vbox->addItem(vlist);
    QPushButton *bShaderEditor = new QPushButton("Shader Editor...", this);
    QObject::connect(bShaderEditor, SIGNAL(released()),
                      this, SLOT(bShaderEditorEnabled()));
    QPushButton *bWaterEditor = new QPushButton("Water Height...", this);
    QObject::connect(bWaterEditor, SIGNAL(released()),
                      this, SLOT(bWaterEditorEnabled()));
    vbox->addWidget(bShaderEditor);
    vbox->addWidget(bWaterEditor);
    QLabel* label = new QLabel("Terrain Patch:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlist->addRow("Selected:",&this->tP);
    vlist->addRow("Shader ID:",&this->tS);
    this->tP.setDisabled(true);
    this->tS.setDisabled(true);
    vbox->addItem(vlist);
    QPushButton *bRotate = new QPushButton("Rotate", this);
    QObject::connect(bRotate, SIGNAL(released()),
                      this, SLOT(bRotateEnabled()));
    QPushButton *bRemoveAllGaps = new QPushButton("Remove All Gaps", this);
    QObject::connect(bRemoveAllGaps, SIGNAL(released()),
                      this, SLOT(bRemoveAllGapsEnabled()));
    QPushButton *bToggleWater = new QPushButton("Toggle Water", this);
    QObject::connect(bToggleWater, SIGNAL(released()),
                      this, SLOT(bToggleWaterEnabled()));
    QPushButton *bToggleDraw = new QPushButton("Toggle Draw", this);
    QObject::connect(bToggleDraw, SIGNAL(released()),
                      this, SLOT(bToggleDrawEnabled()));
    QPushButton *bShowAdjacent = new QPushButton("Show Adjacent Patches", this);
    QObject::connect(bShowAdjacent, SIGNAL(released()),
                      this, SLOT(bShowAdjacentEnabled()));
    vbox->addWidget(bRotate);
    vbox->addWidget(bRemoveAllGaps);
    vbox->addWidget(bToggleWater);
    vbox->addWidget(bToggleDraw);
    vbox->addWidget(bShowAdjacent);
    label = new QLabel("MSTS Settings:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Error Bias:",&eBias);
    QObject::connect(&eBias, SIGNAL(textEdited(QString)),
                      this, SLOT(eBiasEnabled(QString)));
    vbox->addItem(vlist);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesTerrain::~PropertiesTerrain() {
}

void PropertiesTerrain::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    terrainObj = (Terrain*)obj;
    
    this->infoLabel->setText("Object: Terrain");

    this->tX.setText(QString::number(terrainObj->mojex));
    this->tY.setText(QString::number(-terrainObj->mojez));
    this->tP.setText(QString::number(terrainObj->getSelectedPathId()));
    this->tS.setText(QString::number(terrainObj->getSelectedShaderId()));
    this->eBias.setText(QString::number(terrainObj->getErrorBias()));
}

void PropertiesTerrain::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    terrainObj = (Terrain*)obj;
    if(!tP.hasFocus() && !eBias.hasFocus()){
        this->tP.setText(QString::number(terrainObj->getSelectedPathId()));
        this->tS.setText(QString::number(terrainObj->getSelectedShaderId()));
        this->eBias.setText(QString::number(terrainObj->getErrorBias()));
    }
}

bool PropertiesTerrain::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj == GameObj::terrainobj)
        return true;
    return false;
}

void PropertiesTerrain::bShaderEditorEnabled(){
    if(terrainObj == NULL){
        return;
    }
}

void PropertiesTerrain::bWaterEditorEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->setWaterLevelGui();
}

void PropertiesTerrain::bRotateEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->rotatePatchTexture();
}

void PropertiesTerrain::bRemoveAllGapsEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->removeAllGaps();
}

void PropertiesTerrain::bToggleWaterEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->setWaterDraw();
}

void PropertiesTerrain::bToggleDrawEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->setDraw();
}

void PropertiesTerrain::bShowAdjacentEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->setDrawAdjacent();
}

void PropertiesTerrain::eBiasEnabled(QString val){
    if(terrainObj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(!ok) return;
    terrainObj->setErrorBias(fval);
}