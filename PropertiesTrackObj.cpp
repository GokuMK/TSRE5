/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesTrackObj.h"
#include "WorldObj.h"
#include "TrackObj.h"
#include <math.h>
#include "GLMatrix.h"
#include "ParserX.h"
#include "EditFileNameDialog.h"
#include "Undo.h"
#include "Game.h"
#include "Route.h"

PropertiesTrackObj::PropertiesTrackObj(){
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("TrackObj:");
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
    
    QLabel * label = new QLabel("FileName:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    this->fileName.setDisabled(true);
    this->fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->fileName);
    QGridLayout *filenameList = new QGridLayout;
    filenameList->setSpacing(2);
    filenameList->setContentsMargins(0,0,0,0);    
    QPushButton *copyF = new QPushButton("Copy", this);
    QObject::connect(copyF, SIGNAL(released()),
                      this, SLOT(copyFileNameEnabled()));
    QPushButton *editF = new QPushButton("Edit", this);
    QObject::connect(editF, SIGNAL(released()),
                      this, SLOT(editFileNameEnabled()));
    filenameList->addWidget(copyF, 0, 0);
    filenameList->addWidget(editF, 0, 1);
    vbox->addItem(filenameList);
    
    label = new QLabel("Position & Rotation:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("X:",&this->posX);
    vlist->addRow("Y:",&this->posY);
    vlist->addRow("Z:",&this->posZ);
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
    
    posRotList->addWidget(copyPos, 0, 0);
    posRotList->addWidget(pastePos, 0, 1);
    posRotList->addWidget(copyQrot, 1, 0);
    posRotList->addWidget(pasteQrot, 1, 1);
    posRotList->addWidget(copyPosRot, 2, 0);
    posRotList->addWidget(pastePosRot, 2, 1);
    posRotList->addWidget(resetQrot, 3, 0);
    posRotList->addWidget(qRot90, 3, 1);
    posRotList->addWidget(transform, 4, 0, 1, 2);
    vbox->addItem(posRotList);
    
    label = new QLabel("Detail Level:");
    label->setStyleSheet("QLabel { color : #999999; }");
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
    
    label = new QLabel("Elevation:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    QDoubleValidator* doubleValidator = new QDoubleValidator(-10000, 10000, 6, this); 
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    QDoubleValidator* doubleValidator1 = new QDoubleValidator(-1000, 1000, 6, this); 
    doubleValidator1->setNotation(QDoubleValidator::StandardNotation);
    vlist->addRow("‰",&this->elevProm);
    this->elevProm.setValidator(doubleValidator1);
    QObject::connect(&this->elevProm, SIGNAL(textEdited(QString)), this, SLOT(elevPromEnabled(QString)));
    vlist->addRow("1 in 'x' m",&this->elev1inXm);
    this->elev1inXm.setValidator(doubleValidator);
    QObject::connect(&this->elev1inXm, SIGNAL(textEdited(QString)), this, SLOT(elev1inXmEnabled(QString)));
    vbox->addItem(vlist);
    label = new QLabel("MSTS Collision:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&eCollisionFlags);
    eCollisionFlags.setDisabled(true);
    eCollisionFlags.setAlignment(Qt::AlignCenter);
    cCollisionType.addItem("Disabled");
    cCollisionType.addItem("Immovable");
    cCollisionType.addItem("Buffer");
    cCollisionType.setStyleSheet("combobox-popup: 0;");
    vbox->addWidget(&cCollisionType);
    QObject::connect(&cCollisionType, SIGNAL(currentIndexChanged(int)),
                      this, SLOT(cCollisionTypeEdited(int)));
    QPushButton *resetFlags = new QPushButton("Reset Flags", this);
    QObject::connect(resetFlags, SIGNAL(released()),
                      this, SLOT(copyFEnabled()));
    vbox->addWidget(resetFlags);
    label = new QLabel("Advanced:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    QPushButton *fixJNodePosn = new QPushButton("Fix JNodePosn", this);
    QObject::connect(fixJNodePosn, SIGNAL(released()),
                      this, SLOT(fixJNodePosnEnabled()));
    vbox->addWidget(label);
    vbox->addWidget(fixJNodePosn);
    
    QPushButton *hacks = new QPushButton("Hacks", this);
    QObject::connect(hacks, SIGNAL(released()),
                      this, SLOT(hacksButtonEnabled()));
    vbox->addWidget(hacks);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(copyF, SIGNAL(released()),
                      this, SLOT(copyFileNameEnabled()));
}

PropertiesTrackObj::~PropertiesTrackObj() {
}

void PropertiesTrackObj::fixJNodePosnEnabled(){
    if(trackObj == NULL){
        return;
    }
    trackObj->fillJNodePosn();
}

void PropertiesTrackObj::hacksButtonEnabled(){
    if(trackObj == NULL){
        return;
    }
    
    QDialog d;
    d.setMinimumWidth(400);
    d.setWindowTitle("TrackObj Hacks");
    QVBoxLayout *vbox = new QVBoxLayout;
    QLabel *label = new QLabel("These functions will damage your route if you don't know what you are doing.");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    label->setWordWrap(true);
    
    QPushButton *haxRemoveTDBVector = new QPushButton("Remove TDB Vector ( remove TrItems first )", this);
    QObject::connect(haxRemoveTDBVector, SIGNAL(released()),
                      this, SLOT(haxRemoveTDBVectorEnabled()));
    vbox->addWidget(haxRemoveTDBVector);
    
    QPushButton *haxRemoveTDBTree = new QPushButton("Remove TDB Tree ( remove TrItems first; max 1000 nodes )", this);
    QObject::connect(haxRemoveTDBTree, SIGNAL(released()),
                      this, SLOT(haxRemoveTDBTreeEnabled()));
    vbox->addWidget(haxRemoveTDBTree);


    vbox->setSpacing(2);
    vbox->setContentsMargins(3,3,3,3);
    vbox->addStretch(1);
    d.setLayout(vbox);
    d.exec();
}

void PropertiesTrackObj::haxRemoveTDBVectorEnabled(){
    if(trackObj == NULL)
        return;
    if(Game::currentRoute == NULL)
        return;
    
    Game::currentRoute->deleteTDBVector(trackObj);
}

void PropertiesTrackObj::haxRemoveTDBTreeEnabled(){
    if(trackObj == NULL)
        return;
    if(Game::currentRoute == NULL)
        return;
    
    Game::currentRoute->deleteTDBTree(trackObj);
}

void PropertiesTrackObj::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = obj;
    trackObj = (TrackObj*) obj;
    
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
    
    ///////////
    TrackObj* track = (TrackObj*)obj;
    float * q = track->qDirection;
    float vect[3];
    vect[0] = 0; vect[1] = 0; vect [2] = 1000;
    Vec3::transformQuat(vect, vect, q);
    vect[1] = -vect[1];
    
    float oneInXm = 0;
    //if(vect[1] > 0)
        oneInXm = 1000.0/vect[1];
    this->elevProm.setText(QString::number(vect[1]));
    this->elev1inXm.setText(QString::number(oneInXm));
    /*float pitch = asin(2*(q[0]*q[2] - q[1]*q[3]));
    
    if(vect[2] < 0)
        pitch = M_PI - pitch;
    if(vect[2] == 0 && vect[0] < 0)
        pitch = M_PI/2;
    if(vect[2] == 0 && vect[0] > 0)
        pitch = -M_PI/2;*/

    //float elev = tan((vect[1]/10.0));
    //qe[1] = pitch;
    //qe[2] = 0;
    eCollisionFlags.setText(QString::number(worldObj->getCollisionFlags()));
    int collisionType = worldObj->getCollisionType();
    this->cCollisionType.blockSignals(true);
    this->cCollisionType.setCurrentIndex(collisionType);
    this->cCollisionType.blockSignals(false);
}

void PropertiesTrackObj::updateObj(WorldObj* obj){
    if(obj == NULL){
        return;
    }
    TrackObj* track = (TrackObj*)obj;
    float * q = track->qDirection;
    float vect[3];
    vect[0] = 0; vect[1] = 0; vect [2] = 1000;
    Vec3::transformQuat(vect, vect, q);
    vect[1] = -vect[1];
    
    float oneInXm = 0;

    oneInXm = 1000.0/vect[1];
    if(!posX.hasFocus() && !posY.hasFocus() && !posZ.hasFocus() && !quat.hasFocus()){
        this->posX.setText(QString::number(obj->position[0], 'G', 4));
        this->posY.setText(QString::number(obj->position[1], 'G', 4));
        this->posZ.setText(QString::number(-obj->position[2], 'G', 4));
        this->quat.setText(
                QString::number(obj->qDirection[0], 'G', 4) + " " +
                QString::number(obj->qDirection[1], 'G', 4) + " " +
                QString::number(-obj->qDirection[2], 'G', 4) + " " +
                QString::number(obj->qDirection[3], 'G', 4)
                );
    }
    if(!this->elevProm.hasFocus() && !this->elev1inXm.hasFocus()){
        this->elevProm.setText(QString::number(vect[1]));
        this->elev1inXm.setText(QString::number(oneInXm));
    }
    eCollisionFlags.setText(QString::number(worldObj->getCollisionFlags()));
}

void PropertiesTrackObj::elevPromEnabled(QString val){
    if(trackObj == NULL){
        return;
    }
    bool ok = false;
    float prom = val.toFloat(&ok);
    if(!ok) return;
    if(fabs(prom) > 700)
        return;
    float oneInXm = 1000.0/prom;
    qDebug () << "prom" << prom;
    this->elev1inXm.setText(QString::number(oneInXm));
    
    Undo::SinglePushWorldObjData(trackObj);
    trackObj->setElevation(prom);
}

void PropertiesTrackObj::elev1inXmEnabled(QString val){
    if(trackObj == NULL){
        return;
    }
    bool ok = false;
    float oneInXm = val.toFloat(&ok);
    if(!ok) return;
    //qDebug () << "oneInXm" << oneInXm;
    float prom = 1000.0/oneInXm;
    if(fabs(prom) > 700)
        return;
    qDebug () << "prom" << prom;
    this->elevProm.setText(QString::number(prom));
    
    Undo::SinglePushWorldObjData(trackObj);
    trackObj->setElevation(prom);
}

bool PropertiesTrackObj::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "trackobj")
        return true;
    return false;
}

void PropertiesTrackObj::cCollisionTypeEdited(int val){
    if(worldObj == NULL)
        return;
    worldObj->setCollisionType(val-1);
}

void PropertiesTrackObj::editFileNameEnabled(){
    if(worldObj == NULL)
        return;
    EditFileNameDialog eWindow;
    eWindow.name.setText(worldObj->fileName);
    eWindow.exec();
    //qDebug() << waterWindow->changed;
    if(eWindow.isOk){
        worldObj->fileName = eWindow.name.text();
        worldObj->position[2] = -worldObj->position[2];
        worldObj->qDirection[2] = -worldObj->qDirection[2];
        worldObj->load(worldObj->x, worldObj->y);
        worldObj->modified = true;
    }
}