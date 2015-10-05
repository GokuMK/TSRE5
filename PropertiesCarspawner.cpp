#include "PropertiesCarspawner.h"
#include "WorldObj.h"
#include "PlatformObj.h"

PropertiesCarspawner::PropertiesCarspawner() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Carspawner:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    vbox->addWidget(infoLabel);
    
    // names
    QLabel * label = new QLabel("Car Number:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->carNumber);
    label = new QLabel("Car speed:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->carSpeed);

    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesCarspawner::~PropertiesCarspawner() {
}

void PropertiesCarspawner::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    this->infoLabel->setText("Object: "+obj->type);
    PlatformObj* pobj = (PlatformObj*)obj;
    this->carNumber.setText(QString::number(pobj->getCarNumber(),10));
    this->carSpeed.setText(QString::number(pobj->getCarSpeed(),10));
}

bool PropertiesCarspawner::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "carspawner")
        return true;
    return false;
}