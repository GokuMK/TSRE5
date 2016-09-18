/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesStatic.h"
#include "WorldObj.h"
#include "StaticObj.h"
#include "ParserX.h"

PropertiesStatic::PropertiesStatic(){
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Static:");
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
    
    QLabel * label1 = new QLabel("FileName:");
    label1->setContentsMargins(3,0,0,0);
    vbox->addWidget(label1);
    this->fileName.setDisabled(true);
    this->fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->fileName);
    QPushButton *copyF = new QPushButton("Copy FileName", this);
    vbox->addWidget(copyF);
    
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
    QPushButton *copyPos = new QPushButton("Copy Position", this);
    QPushButton *pastePos = new QPushButton("Paste Position", this);
    vbox->addWidget(copyPos);
    vbox->addWidget(pastePos);
    QLabel * label3 = new QLabel("QDirection:");
    label3->setStyleSheet("QLabel { color : #999999; }");
    label3->setContentsMargins(3,0,0,0);
    vbox->addWidget(label3);
    this->quat.setDisabled(true);
    this->quat.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->quat);
    QPushButton *copyQrot = new QPushButton("Copy Rotation", this);
    QObject::connect(copyQrot, SIGNAL(released()),
                      this, SLOT(copyREnabled()));
    QPushButton *pasteQrot = new QPushButton("Paste Rotation", this);
    QObject::connect(pasteQrot, SIGNAL(released()),
                      this, SLOT(pasteREnabled()));
    QPushButton *resetQrot = new QPushButton("Reset Rotation", this);
    QObject::connect(resetQrot, SIGNAL(released()),
                      this, SLOT(resetRotEnabled()));
    QPushButton *qRot90 = new QPushButton("Rotate Y 90°", this);
    QObject::connect(qRot90, SIGNAL(released()),
                      this, SLOT(rotYEnabled()));
    vbox->addWidget(copyQrot);
    vbox->addWidget(pasteQrot);
    vbox->addWidget(resetQrot);
    vbox->addWidget(qRot90);
    
    QLabel * label4 = new QLabel("Detail Level:");
    label4->setStyleSheet("QLabel { color : #999999; }");
    label4->setContentsMargins(3,0,0,0);
    vbox->addWidget(label4);
    this->defaultDetailLevel.setDisabled(true);
    this->defaultDetailLevel.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->defaultDetailLevel);
    this->enableCustomDetailLevel.setText("Custom");
    vbox->addWidget(&enableCustomDetailLevel);
    this->customDetailLevel.setDisabled(true);
    this->customDetailLevel.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->customDetailLevel);
    
    label4 = new QLabel("Flags:");
    label4->setStyleSheet("QLabel { color : #999999; }");
    label4->setContentsMargins(3,0,0,0);
    vbox->addWidget(label4);
    this->flags.setDisabled(true);
    this->flags.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->flags);
    QPushButton *copyFlags = new QPushButton("Copy Flags", this);
    QPushButton *pasteFlags = new QPushButton("Paste Flags", this);
    vbox->addWidget(copyFlags);
    vbox->addWidget(pasteFlags);
    checkboxAnim.setText("Animations");
    checkboxTerrain.setText("Terrain Object");
    vbox->addWidget(&checkboxAnim);
    vbox->addWidget(&checkboxTerrain);
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(copyF, SIGNAL(released()),
                      this, SLOT(copyFEnabled()));
    
    QObject::connect(&enableCustomDetailLevel, SIGNAL(stateChanged(int)),
                      this, SLOT(enableCustomDetailLevelEnabled(int)));
    
    QObject::connect(&customDetailLevel, SIGNAL(textEdited(QString)),
                      this, SLOT(customDetailLevelEdited(QString)));
    
}

PropertiesStatic::~PropertiesStatic() {
}

void PropertiesStatic::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    staticObj = (StaticObj*) obj;
    this->infoLabel->setText("Object: "+obj->type);
    this->fileName.setText(obj->fileName);
    
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
}

bool PropertiesStatic::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "static")
        return true;
    if(obj->type == "gantry")
        return true;
    return false;
}

void PropertiesStatic::copyFEnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    //QString originalText = clipboard->text();
    clipboard->setText(this->fileName.text());
}

void PropertiesStatic::resetRotEnabled(){
    if(staticObj == NULL)
        return;
    staticObj->setNewQdirection();
    staticObj->modified = true;
    staticObj->setMartix();
}

void PropertiesStatic::copyREnabled(){
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->quat.text());
}

void PropertiesStatic::rotYEnabled(){
    if(staticObj == NULL)
        return;
    staticObj->rotate(0,M_PI/2,0);
    staticObj->modified = true;
    staticObj->setMartix();
}

void PropertiesStatic::pasteREnabled(){
    if(staticObj == NULL)
        return;
    QClipboard *clipboard = QApplication::clipboard();
    QStringList args = clipboard->text().split(" ");
    if(args.length() != 4)
        return;
    float nq[4];
    nq[0] = args[0].toFloat();
    nq[1] = args[1].toFloat();
    nq[2] = args[2].toFloat();
    nq[3] = args[3].toFloat();
    staticObj->setQdirection((float*)&nq);
    staticObj->modified = true;
    staticObj->setMartix();
    quat.setText(clipboard->text());
}

void PropertiesStatic::enableCustomDetailLevelEnabled(int val){
    if(staticObj == NULL)
        return;
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

void PropertiesStatic::customDetailLevelEdited(QString val){
    if(staticObj == NULL)
        return;
    bool ok = false;
    int level = val.toInt(&ok);
    qDebug() << "aaaaaaaaaa";
    if(ok){
        staticObj->setCustomDetailLevel(level);
    }
}