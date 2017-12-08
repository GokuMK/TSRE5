/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PropertiesLevelCr.h"
#include "LevelCrObj.h"
#include "GuiFunct.h"
#include "Game.h"

PropertiesLevelCr::PropertiesLevelCr() {

    QDoubleValidator* doubleValidator = new QDoubleValidator(-10000, 10000, 6, this); 
    doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("LevelCr:");
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
    
    label = new QLabel("Filename:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    fileName.setDisabled(true);
    fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&fileName);
    
    label = new QLabel("Level Crossing Sensitivity:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    //QFormLayout *vlist = new QFormLayout;
    //vlist->setSpacing(2);
    //vlist->setContentsMargins(3,0,3,0);
    vbox->addWidget(new QLabel("Activate LevelCr by [s]:"));
    vbox->addWidget(&eActivateLevelCrossing);
    eActivateLevelCrossing.setValidator(doubleValidator);
    QObject::connect(&eActivateLevelCrossing, SIGNAL(textEdited(QString)), this, SLOT(eActivateLevelCrossingEnabled(QString)));
    vbox->addWidget(new QLabel("Min activation distance [m]:"));
    vbox->addWidget(&eMinActDist);
    eMinActDist.setValidator(doubleValidator);
    QObject::connect(&eMinActDist, SIGNAL(textEdited(QString)), this, SLOT(eMinActDistEnabled(QString)));
    //vbox->addItem(vlist);
    label = new QLabel("Level Crossing Timing:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    //vlist = new QFormLayout;
    //vlist->setSpacing(2);
    //vlist->setContentsMargins(3,0,3,0);
    vbox->addWidget(new QLabel("Initial warning phase [s]:"));
    vbox->addWidget(&eInitialWarning);
    eInitialWarning.setValidator(doubleValidator);
    QObject::connect(&eInitialWarning, SIGNAL(textEdited(QString)), this, SLOT(eInitialWarningEnabled(QString)));
    vbox->addWidget(new QLabel("Serious warning phase [s]:"));
    vbox->addWidget(&eMoreWarning);
    eMoreWarning.setValidator(doubleValidator);
    QObject::connect(&eMoreWarning, SIGNAL(textEdited(QString)), this, SLOT(eMoreWarningEnabled(QString)));
    vbox->addWidget(new QLabel("Gate animation length [s]:"));
    vbox->addWidget(&eGateAnimLength);
    eGateAnimLength.setValidator(doubleValidator);
    QObject::connect(&eGateAnimLength, SIGNAL(textEdited(QString)), this, SLOT(eGateAnimLengthEnabled(QString)));
    //vbox->addItem(vlist);

    label = new QLabel("More Options:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    //vlist = new QFormLayout;
    //vlist->setSpacing(2);
    //vlist->setContentsMargins(3,0,3,0);
    vbox->addWidget(new QLabel("Crash probability:"));
    vbox->addWidget(&eCrashProbability);
    eCrashProbability.setValidator(doubleValidator);
    QObject::connect(&eCrashProbability, SIGNAL(textEdited(QString)), this, SLOT(eCrashProbabilityEnabled(QString)));
    //vbox->addItem(vlist);
    vbox->addWidget(&chInvisible);
    QObject::connect(&chInvisible, SIGNAL(stateChanged(int)),
                      this, SLOT(chInvisibleEnabled(int)));
    vbox->addWidget(&chSilentHax);
    QObject::connect(&chSilentHax, SIGNAL(stateChanged(int)),
                      this, SLOT(chSilentHaxEnabled(int)));
    chInvisible.setText("Crossing is invisible");
    chSilentHax.setText("Silent crossing MSTS HAX");
    
    label = new QLabel("Track Items:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    
    QPushButton *bDeleteSelected = new QPushButton("Delete Selected");
    vbox->addWidget(bDeleteSelected);
    QObject::connect(bDeleteSelected, SIGNAL(released()),
                      this, SLOT(bDeleteSelectedEnabled()));
    
    label = new QLabel("Sound File:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    cSoundType.addItem("DEFAULT");
    cSoundType.addItem("CUSTOM");
    cSoundType.setStyleSheet("combobox-popup: 0;");
    QObject::connect(&cSoundType, SIGNAL(currentIndexChanged(int)),
            this, SLOT(cSoundTypeEnabled(int)));
    vbox->addWidget(&cSoundType);
    vbox->addWidget(&eSoundName);
    QObject::connect(&eSoundName, SIGNAL(textEdited(QString)), this, SLOT(eSoundNameEnabled(QString)));

    
    label = new QLabel("Global settings:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(new QLabel("Max placing radius:"));
    vbox->addWidget(&eMaxPlacingDistance);
    eMaxPlacingDistance.setValidator(doubleValidator);
    QObject::connect(&eMaxPlacingDistance, SIGNAL(textEdited(QString)), this, SLOT(eMaxPlacingDistanceEnabled(QString)));
    
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesLevelCr::~PropertiesLevelCr() {
}

void PropertiesLevelCr::showObj(GameObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    worldObj = (WorldObj*)obj;
    lobj = (LevelCrObj*)obj;
    
    this->infoLabel->setText("Object: "+lobj->type);
    this->fileName.setText(lobj->fileName);

    this->uid.setText(QString::number(lobj->UiD, 10));
    this->tX.setText(QString::number(lobj->x, 10));
    this->tY.setText(QString::number(-lobj->y, 10));
    this->posX.setText(QString::number(lobj->position[0], 'G', 6));
    this->posY.setText(QString::number(lobj->position[1], 'G', 6));
    this->posZ.setText(QString::number(-lobj->position[2], 'G', 6));
    this->quat.setText(
            QString::number(lobj->qDirection[0], 'G', 4) + " " +
            QString::number(lobj->qDirection[1], 'G', 4) + " " +
            QString::number(-lobj->qDirection[2], 'G', 4) + " " +
            QString::number(lobj->qDirection[3], 'G', 4)
            );
    
    this->eActivateLevelCrossing.setText(QString::number(lobj->getSensitivityActivateLevel()));
    this->eMinActDist.setText(QString::number(lobj->getSensitivityMinimunDistance()));
    this->eInitialWarning.setText(QString::number(lobj->getTimingInitialWarning()));
    this->eMoreWarning.setText(QString::number(lobj->getTimingSeriousWarning()));
    this->eGateAnimLength.setText(QString::number(lobj->getTimingAnimationLength()));
    this->eCrashProbability.setText(QString::number(lobj->getCrashProbability()));
    this->eMaxPlacingDistance.setText(QString::number(lobj->MaxPlacingDistance));
    this->chInvisible.blockSignals(true);
    this->chInvisible.setChecked(lobj->isInvisibleEnabled());
    this->chInvisible.blockSignals(false);
    this->chSilentHax.blockSignals(true);
    this->chSilentHax.setChecked(lobj->isSilentMstsHaxEnabled());
    this->chSilentHax.blockSignals(false);
    
    QString sname = lobj->getSoundFileName();
    cSoundType.blockSignals(true);
    if(sname.length() < 1){
        cSoundType.setCurrentIndex(0);
        eSoundName.hide();
    } else {
        cSoundType.setCurrentIndex(1);
        eSoundName.show();
        eSoundName.setText(sname);
    }
    cSoundType.blockSignals(false);
}

void PropertiesLevelCr::updateObj(GameObj* obj){
    if(obj == NULL){
        return;
    }
    lobj = (LevelCrObj*)obj;
    if(!posX.hasFocus() && !posY.hasFocus() && !posZ.hasFocus() && !quat.hasFocus()){
        this->uid.setText(QString::number(lobj->UiD, 10));
        this->tX.setText(QString::number(lobj->x, 10));
        this->tY.setText(QString::number(-lobj->y, 10));
        this->posX.setText(QString::number(lobj->position[0], 'G', 6));
        this->posY.setText(QString::number(lobj->position[1], 'G', 6));
        this->posZ.setText(QString::number(-lobj->position[2], 'G', 6));
        this->quat.setText(
                QString::number(lobj->qDirection[0], 'G', 4) + " " +
                QString::number(lobj->qDirection[1], 'G', 4) + " " +
                QString::number(-lobj->qDirection[2], 'G', 4) + " " +
                QString::number(lobj->qDirection[3], 'G', 4)
                );
    }
}

void PropertiesLevelCr::eActivateLevelCrossingEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        lobj->setSensitivityActivateLevel(fval);
    }
}

void PropertiesLevelCr::eSoundNameEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    if(val.endsWith(".sms", Qt::CaseInsensitive))
        lobj->setSoundFileName(val);
}

void PropertiesLevelCr::cSoundTypeEnabled(int val){
    if(lobj == NULL){
        return;
    }  
    if(val == 0){
        lobj->setSoundFileName("");
        eSoundName.hide();
    } else {
        eSoundName.show();
        if(eSoundName.text().endsWith(".sms", Qt::CaseInsensitive))
        lobj->setSoundFileName(eSoundName.text());
    }
}
    
void PropertiesLevelCr::eMaxPlacingDistanceEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        if(fval > 0)
            lobj->MaxPlacingDistance = fval;
    }
}

void PropertiesLevelCr::eMinActDistEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        lobj->setSensitivityMinimunDistance(fval);
    }
}

void PropertiesLevelCr::eInitialWarningEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        lobj->setTimingInitialWarning(fval);
    }
}

void PropertiesLevelCr::eMoreWarningEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        lobj->setTimingSeriousWarning(fval);
    }
}

void PropertiesLevelCr::eGateAnimLengthEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        lobj->setTimingAnimationLength(fval);
    }
}

void PropertiesLevelCr::eCrashProbabilityEnabled(QString val){
    if(lobj == NULL){
        return;
    }
    bool ok = false;
    float fval = val.toFloat(&ok);
    if(ok){
        Undo::SinglePushWorldObjData(worldObj);
        lobj->setCrashProbability(fval);
    }
}

void PropertiesLevelCr::chInvisibleEnabled(int val){
    if(lobj == NULL){
        return;
    }
    Undo::SinglePushWorldObjData(worldObj);
    if(val == 2){
        lobj->setInvisible(true);
    } else {
        lobj->setInvisible(false);
    }
}

void PropertiesLevelCr::chSilentHaxEnabled(int val){
    if(lobj == NULL){
        return;
    }
    Undo::SinglePushWorldObjData(worldObj);
    if(val == 2){
        lobj->setSilentMstsHax(true);
    } else {
        lobj->setSilentMstsHax(false);
    }
}

void PropertiesLevelCr::bDeleteSelectedEnabled(){
    if(lobj == NULL){
        return;
    }
    Undo::StateBegin();
    Undo::PushGameObjData(worldObj);
    Undo::PushTrackDB(Game::trackDB, false);
    Undo::PushTrackDB(Game::roadDB, true);
    lobj->deleteSelectedTrItem();
    Undo::StateEnd();
}

bool PropertiesLevelCr::support(GameObj* obj){
    if(obj == NULL)
        return false;
    if(obj->typeObj != GameObj::worldobj)
        return false;
    if(((WorldObj*)obj)->type == "levelcr")
        return true;
    return false;
}