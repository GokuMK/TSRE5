#include "PropertiesSiding.h"
#include "WorldObj.h"
#include "PlatformObj.h"

PropertiesSiding::PropertiesSiding() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Platform:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    
    // name
    QLabel* label = new QLabel("Siding Name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->namePlatform);
    // misc
    label = new QLabel("Misc:");
    label->setStyleSheet("QLabel { color : #999999; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    disablePlatform.setText("Disable Platform");
    vbox->addWidget(&disablePlatform);
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesSiding::~PropertiesSiding() {
}

void PropertiesSiding::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    this->infoLabel->setText("Object: "+obj->type);
    PlatformObj* pobj = (PlatformObj*)obj;
    this->namePlatform.setText(pobj->getPlatformName());
    this->disablePlatform.setChecked(pobj->getDisabled());
}

bool PropertiesSiding::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "siding")
        return true;
    return false;
}