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
    vlist->addRow("Name:",&this->fileName);
    vbox->addItem(vlist);
    QPushButton *bShaderEditor = new QPushButton("Shader Editor...", this);
    QObject::connect(bShaderEditor, SIGNAL(released()),
                      this, SLOT(bShaderEditorEnabled()));
    QPushButton *bWaterEditor = new QPushButton("Water Height...", this);
    QObject::connect(bWaterEditor, SIGNAL(released()),
                      this, SLOT(bWaterEditorEnabled()));
    vbox->addWidget(bShaderEditor);
    vbox->addWidget(bWaterEditor);
    
    QLabel* label = new QLabel("Selected Terrain Patch(s):");
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
    QPushButton *bRemoveAllGaps = new QPushButton("Remove All Gaps", this);
    QObject::connect(bRemoveAllGaps, SIGNAL(released()),
                      this, SLOT(bRemoveAllGapsEnabled()));
    QPushButton *bShowWater = new QPushButton("Show", this);
    QObject::connect(bShowWater, SIGNAL(released()), this, SLOT(bShowWaterEnabled()));
    QPushButton *bShowDraw = new QPushButton("Show", this);
    QObject::connect(bShowDraw, SIGNAL(released()), this, SLOT(bShowDrawEnabled()));
    QPushButton *bHideWater = new QPushButton("Hide", this);
    QObject::connect(bHideWater, SIGNAL(released()), this, SLOT(bHideWaterEnabled()));
    QPushButton *bHideDraw = new QPushButton("Hide", this);
    QObject::connect(bHideDraw, SIGNAL(released()), this, SLOT(bHideDrawEnabled()));

    QGridLayout *vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(3,0,3,0);
    int row = 0;
    vlist1->addWidget(new QLabel("Water:"), row, 0);
    vlist1->addWidget(bShowWater, row, 1);
    vlist1->addWidget(bHideWater, row++, 2);
    vlist1->addWidget(new QLabel("Draw:"), row, 0);
    vlist1->addWidget(bShowDraw, row, 1);
    vlist1->addWidget(bHideDraw, row++, 2);
    vlist1->addWidget(new QLabel("Gaps:"), row, 0);
    vlist1->addWidget(bRemoveAllGaps, row++, 1, 1, 2);
    vbox->addItem(vlist1);
    
    label = new QLabel("Texture Transformation:");
    //label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(3,0,3,0);
    row = 0;
    QPushButton *bCopy = new QPushButton("Copy", this);
    QObject::connect(bCopy, SIGNAL(released()), this, SLOT(bCopyEnabled()));
    vlist1->addWidget(bCopy, row, 0);
    QPushButton *bPaste = new QPushButton("Paste", this);
    QObject::connect(bPaste, SIGNAL(released()), this, SLOT(bPasteEnabled()));
    vlist1->addWidget(bPaste, row++, 1);
    QPushButton *bRotate = new QPushButton("Rotate 90°", this);
    QObject::connect(bRotate, SIGNAL(released()), this, SLOT(bRotateEnabled()));
    vlist1->addWidget(bRotate, row, 0);
    QPushButton *bScale = new QPushButton("Scale...", this);
    QObject::connect(bScale, SIGNAL(released()), this, SLOT(bScaleEnabled()));
    vlist1->addWidget(bScale, row++, 1);
    QPushButton *bMirrorX = new QPushButton("Mirror Y", this);
    QObject::connect(bMirrorX, SIGNAL(released()), this, SLOT(bMirrorXEnabled()));
    vlist1->addWidget(bMirrorX, row, 0);
    QPushButton *bMirrorY = new QPushButton("Mirror X", this);
    QObject::connect(bMirrorY, SIGNAL(released()), this, SLOT(bMirrorYEnabled()));
    vlist1->addWidget(bMirrorY, row++, 1);
    QPushButton *bReset = new QPushButton("Reset", this);
    QObject::connect(bReset, SIGNAL(released()), this, SLOT(bResetEnabled()));
    vlist1->addWidget(bReset, row++, 0, 1, 2);
    vbox->addItem(vlist1);
    
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
    this->fileName.setText(terrainObj->getTileName());
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

void PropertiesTerrain::bMirrorXEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->mirrorXPatchTexture();
}

void PropertiesTerrain::bMirrorYEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->mirrorYPatchTexture();
}

void PropertiesTerrain::bScaleEnabled(){
    if(terrainObj == NULL){
        return;
    }
    float val = 0;
    bool ok = false;
    val = QInputDialog::getDouble(this, "Scale ...", "Enter scale factor:", terrainObj->getPatchScaleTex(), -100.0, 100.0, 2, &ok);
    if(!ok)
        return;
    
    terrainObj->scalePatchTexCoords(val);
}

void PropertiesTerrain::bCopyEnabled(){
    if(terrainObj == NULL){
        return;
    }
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(terrainObj->getPatchTexTransformString());
}

void PropertiesTerrain::bPasteEnabled(){
    if(terrainObj == NULL){
        return;
    }
    QClipboard *clipboard = QApplication::clipboard();
    terrainObj->setPatchTexTransform(clipboard->text());
}

void PropertiesTerrain::bResetEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->resetPatchTexCoords();
}

void PropertiesTerrain::bRemoveAllGapsEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->removeAllGaps();
}

void PropertiesTerrain::bShowWaterEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->setWaterDraw();
}

void PropertiesTerrain::bShowDrawEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->setDraw();
}

void PropertiesTerrain::bHideWaterEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->hideWaterDraw();
}

void PropertiesTerrain::bHideDrawEnabled(){
    if(terrainObj == NULL){
        return;
    }
    terrainObj->hideDraw();
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