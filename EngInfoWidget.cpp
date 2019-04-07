/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "EngInfoWidget.h"
#include "Game.h"
#include "Eng.h"

EngInfoWidget::EngInfoWidget(QWidget* parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    
    QGridLayout *engInfoForm = new QGridLayout;
    engInfoForm->setSpacing(2);
    engInfoForm->setContentsMargins(1,1,1,1);    
    engInfoForm->addWidget(new QLabel("Name:"),0,0);
    engInfoForm->addWidget(new QLabel("File Name:"),1,0);
    engInfoForm->addWidget(new QLabel("Dir Name:"),2,0);
    engInfoForm->addWidget(new QLabel("Shape:"),3,0);
    engInfoForm->addWidget(new QLabel("Type:"),0,2);
    engInfoForm->addWidget(new QLabel("Brakes:"),1,2);
    engInfoForm->addWidget(new QLabel("Couplings:"),2,2);
    engInfoForm->addWidget(new QLabel("Size:"),3,2);
    engInfoForm->addWidget(new QLabel("Mass:"),0,4);
    engInfoForm->addWidget(new QLabel("Max. Speed:"),1,4);
    engInfoForm->addWidget(new QLabel("Max. Force:"),2,4);
    engInfoForm->addWidget(new QLabel("Max. Power:"),3,4);    
    
    engInfoForm->addWidget(&eName,0,1);
    engInfoForm->addWidget(&eFileName,1,1);
    engInfoForm->addWidget(&eDirName,2,1);
    engInfoForm->addWidget(&eShape,3,1);
    engInfoForm->addWidget(&eType,0,3);
    engInfoForm->addWidget(&eBrakes,1,3);
    engInfoForm->addWidget(&eCouplings,2,3);
    engInfoForm->addWidget(&eSize,3,3);
    engInfoForm->addWidget(&eMass,0,5);
    engInfoForm->addWidget(&eMaxSpeed,1,5);
    engInfoForm->addWidget(&eMaxForce,2,5);
    engInfoForm->addWidget(&eMaxPower,3,5);
    eMass.setMaximumWidth(70);
    eMaxSpeed.setMaximumWidth(70);
    eMaxForce.setMaximumWidth(70);
    eMaxPower.setMaximumWidth(70);
    setLayout(engInfoForm);
}

EngInfoWidget::~EngInfoWidget() {
}

void EngInfoWidget::setInfo(Eng* eng){
    if(eng == NULL)
        return;

    eName.setText(eng->displayName);
    eFileName.setText(eng->name);
    eDirName.setText(eng->path.split("/").last());
    QString ttype = eng->type;
    if(eng->engType.length() > 1)
        ttype += " ( "+eng->engType+" )";
    eType.setText(ttype);
    //eBrakes;
    //eCouplings;
    eMass.setText(QString::number(eng->mass) + " t");
    if(eng->wagonTypeId >= 4){
        eMaxSpeed.setText(QString::number((int)eng->maxSpeed) + " km/h");
        eMaxForce.setText(QString::number((int)eng->maxForce / 1000.0) + " kN");
        eMaxPower.setText(QString::number((int)eng->maxPower ) + " kW");
    } else {
        eMaxSpeed.setText("--");
        eMaxForce.setText("--");
        eMaxPower.setText("--");
    }
    eShape.setText(eng->shape.name);
    eSize.setText(QString::number(eng->sizex)+" "+QString::number(eng->sizey)+" "+QString::number(eng->sizez)+" ");
    eCouplings.setText(eng->getCouplingsName());
    eBrakes.setText(eng->brakeSystemType);
}