/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ConInfoWidget.h"
#include "Game.h"

ConInfoWidget::ConInfoWidget(QWidget* parent) : QWidget(parent) {
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

    setLayout(engInfoForm);
}

ConInfoWidget::~ConInfoWidget() {
}

