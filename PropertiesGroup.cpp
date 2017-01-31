/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesGroup.h"
#include "GroupObj.h"
#include "ParserX.h"
#include "Game.h"

PropertiesGroup::PropertiesGroup() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Group:");
    infoLabel->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    
    QLabel *label = new QLabel("Position & Rotation:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    this->quat.setDisabled(true);
    this->quat.setAlignment(Qt::AlignCenter);
    vlist->addRow("Rot:",&this->quat);
    vbox->addItem(vlist);
    
    QGridLayout *posRotList = new QGridLayout;
    posRotList->setSpacing(2);
    posRotList->setContentsMargins(0,0,0,0);    

    QPushButton *copyPos = new QPushButton("Copy Pos", this);
    QObject::connect(copyPos, SIGNAL(released()),
                      this, SLOT(copyPEnabled()));
    QPushButton *pastePos = new QPushButton("Paste", this);
    QObject::connect(pastePos, SIGNAL(released()),
                      this, SLOT(pastePEnabled()));
    QPushButton *copyQrot = new QPushButton("Copy Rot", this);
    QObject::connect(copyQrot, SIGNAL(released()),
                      this, SLOT(copyREnabled()));
    QPushButton *pasteQrot = new QPushButton("Paste", this);
    QObject::connect(pasteQrot, SIGNAL(released()),
                      this, SLOT(pasteREnabled()));
    QPushButton *copyPosRot = new QPushButton("Copy Pos+Rot", this);
    QObject::connect(copyPosRot, SIGNAL(released()),
                      this, SLOT(copyPREnabled()));
    QPushButton *pastePosRot = new QPushButton("Paste", this);
    QObject::connect(pastePosRot, SIGNAL(released()),
                      this, SLOT(pastePREnabled()));
    QPushButton *resetQrot = new QPushButton("Reset Rot", this);
    QObject::connect(resetQrot, SIGNAL(released()),
                      this, SLOT(resetRotEnabled()));
    QPushButton *qRot90 = new QPushButton("Rot Y 90°", this);
    QObject::connect(qRot90, SIGNAL(released()),
                      this, SLOT(rotYEnabled()));
    QPushButton *transform = new QPushButton("Transform ...", this);
    QObject::connect(transform, SIGNAL(released()),
                      this, SLOT(transformEnabled()));
    chSeparateRotation.setText("Separate Rotation");
    QObject::connect(&chSeparateRotation, SIGNAL(stateChanged(int)),
                      this, SLOT(chIndividualRotationEdited(int)));
    
    posRotList->addWidget(copyPos, 0, 0);
    posRotList->addWidget(pastePos, 0, 1);
    posRotList->addWidget(copyQrot, 1, 0);
    posRotList->addWidget(pasteQrot, 1, 1);
    posRotList->addWidget(copyPosRot, 2, 0);
    posRotList->addWidget(pastePosRot, 2, 1);
    posRotList->addWidget(resetQrot, 3, 0);
    posRotList->addWidget(qRot90, 3, 1);
    posRotList->addWidget(transform, 4, 0, 1, 2);
    posRotList->addWidget(&chSeparateRotation, 5, 0, 1, 2);
    vbox->addItem(posRotList);
    
    label = new QLabel("Detail Level:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    this->defaultDetailLevel.setDisabled(true);
    this->defaultDetailLevel.setAlignment(Qt::AlignCenter);
    this->enableCustomDetailLevel.setText("Custom");
    QCheckBox* defaultDetailLevelLabel = new QCheckBox("Default", this);
    defaultDetailLevelLabel->setDisabled(true);
    defaultDetailLevelLabel->setChecked(true);
    QObject::connect(&enableCustomDetailLevel, SIGNAL(stateChanged(int)),
                      this, SLOT(enableCustomDetailLevelEnabled(int)));
    this->customDetailLevel.setDisabled(true);
    this->customDetailLevel.setAlignment(Qt::AlignCenter);
    QObject::connect(&customDetailLevel, SIGNAL(textEdited(QString)),
                      this, SLOT(customDetailLevelEdited(QString)));
    QGridLayout *detailLevelView = new QGridLayout;
    detailLevelView->setSpacing(2);
    detailLevelView->setContentsMargins(0,0,0,0);    
    detailLevelView->addWidget(defaultDetailLevelLabel, 0, 0);
    detailLevelView->addWidget(&defaultDetailLevel, 0, 1);
    detailLevelView->addWidget(&enableCustomDetailLevel, 1, 0);
    detailLevelView->addWidget(&customDetailLevel, 1, 1);
    vbox->addItem(detailLevelView);
    
    label = new QLabel("Flags:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
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
    QObject::connect(&cShadowType, SIGNAL(activated(int)),
                      this, SLOT(cShadowTypeEdited(int)));

    vbox->addStretch(1);
    this->setLayout(vbox);
    
}

PropertiesGroup::~PropertiesGroup() {
}

void PropertiesGroup::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = obj;
    GroupObj *gobj = (GroupObj*)worldObj;
    
    float *qDirection = obj->getQuatRotation();
    float *position = obj->getPosition();
    this->posX.setText(QString::number(position[0], 'G', 4));
    this->posY.setText(QString::number(position[1], 'G', 4));
    this->posZ.setText(QString::number(-position[2], 'G', 4));
    this->quat.setText (
        QString::number(qDirection[0], 'G', 4) + " " +
        QString::number(qDirection[1], 'G', 4) + " " +
        QString::number(-qDirection[2], 'G', 4) + " " +
        QString::number(qDirection[3], 'G', 4)
        );
    
    defaultDetailLevel.setText(QString::number(obj->getDefaultDetailLevel()));
    enableCustomDetailLevel.blockSignals(true);
    if(obj->customDetailLevelEnabled()){
        enableCustomDetailLevel.setChecked(true);
        customDetailLevel.setText(QString::number(obj->getCustomDetailLevel()));
        customDetailLevel.setEnabled(true);
    } else {
        enableCustomDetailLevel.setChecked(false);
        customDetailLevel.setText("");
        customDetailLevel.setEnabled(false);
    }
    enableCustomDetailLevel.blockSignals(false);
    
    this->flags.setText(ParserX::MakeFlagsString(obj->staticFlags));
    this->checkboxAnim.blockSignals(true);
    this->checkboxTerrain.blockSignals(true);
    this->cShadowType.blockSignals(true);
    this->chSeparateRotation.blockSignals(true);
    this->checkboxAnim.setChecked(obj->isAnimated());
    this->checkboxTerrain.setChecked(obj->isTerrainObj());
    this->cShadowType.setCurrentIndex((int)obj->getShadowType());
    this->chSeparateRotation.setChecked(gobj->isIndividualRotation());
    this->checkboxAnim.blockSignals(false);
    this->checkboxTerrain.blockSignals(false);
    this->cShadowType.blockSignals(false);
    this->chSeparateRotation.blockSignals(false);
}

void PropertiesGroup::updateObj(WorldObj* obj){
    if(obj == NULL){
        return;
    }
    
    if(!posX.hasFocus() && !posY.hasFocus() && !posZ.hasFocus() && !quat.hasFocus()){
        float *qDirection = obj->getQuatRotation();
        float *position = obj->getPosition();
        this->posX.setText(QString::number(position[0], 'G', 4));
        this->posY.setText(QString::number(position[1], 'G', 4));
        this->posZ.setText(QString::number(-position[2], 'G', 4));
        this->quat.setText (
            QString::number(qDirection[0], 'G', 4) + " " +
            QString::number(qDirection[1], 'G', 4) + " " +
            QString::number(-qDirection[2], 'G', 4) + " " +
            QString::number(qDirection[3], 'G', 4)
            );
    }
}

bool PropertiesGroup::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "groupobject")
        return true;
    return false;
}

void PropertiesGroup::enableCustomDetailLevelEnabled(int val){
    if(worldObj == NULL)
        return;
    GroupObj* staticObj = (GroupObj*) worldObj;
    if(val == 2){
        customDetailLevel.setEnabled(true);
        customDetailLevel.setText("0");
        staticObj->setCustomDetailLevel(0);
    } else {
        customDetailLevel.setEnabled(false);
        customDetailLevel.setText("");
        staticObj->setCustomDetailLevel(-1);
    }
}

void PropertiesGroup::customDetailLevelEdited(QString val){
    if(worldObj == NULL)
        return;
    GroupObj* staticObj = (GroupObj*) worldObj;
    bool ok = false;
    int level = val.toInt(&ok);
    //qDebug() << "aaaaaaaaaa";
    if(ok){
        staticObj->setCustomDetailLevel(level);
    }
}

void PropertiesGroup::checkboxAnimEdited(int val){
    if(worldObj == NULL)
        return;
    if(val == 2){
        worldObj->setAnimated(true);
    } else {
        worldObj->setAnimated(false);
    }
    this->flags.setText(ParserX::MakeFlagsString(worldObj->staticFlags));
}

void PropertiesGroup::checkboxTerrainEdited(int val){
    if(worldObj == NULL)
        return;
    if(val == 2){
        worldObj->setTerrainObj(true);
    } else {
        worldObj->setTerrainObj(false);
    }
    this->flags.setText(ParserX::MakeFlagsString(worldObj->staticFlags));
}

void PropertiesGroup::cShadowTypeEdited(int val){
    if(worldObj == NULL)
        return;
    worldObj->setShadowType((WorldObj::ShadowType)val);
    this->flags.setText(ParserX::MakeFlagsString(worldObj->staticFlags));
}

void PropertiesGroup::chIndividualRotationEdited(int val){
    if(worldObj == NULL)
        return;
    GroupObj *gobj = (GroupObj*)worldObj;
    if(val == 2){
        gobj->setIndividualRotation(true);
    } else {
        gobj->setIndividualRotation(false);
    }
}