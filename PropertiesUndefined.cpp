/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#include "PropertiesUndefined.h"
#include "WorldObj.h"

PropertiesUndefined::PropertiesUndefined(){
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Select to see properties.");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel); 
    
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesUndefined::~PropertiesUndefined() {
}

void PropertiesUndefined::showObj(WorldObj* obj){
    if(obj == NULL)
        infoLabel->setText("Select to see properties.");
    else
        infoLabel->setText("Unsupported: "+obj->type);
}

bool PropertiesUndefined::support(WorldObj* obj){
    return true;
}