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
    
    QObject::connect(&disablePlatform, SIGNAL(stateChanged(int)),
                      this, SLOT(disablePlatformEnabled(int)));
    QObject::connect(&namePlatform, SIGNAL(textChanged(QString)),
                      this, SLOT(namePlatformEnabled(QString)));
}

PropertiesSiding::~PropertiesSiding() {
}

void PropertiesSiding::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    this->infoLabel->setText("Object: "+obj->type);
    pobj = (PlatformObj*)obj;
    this->namePlatform.setText(pobj->getPlatformName());
    this->disablePlatform.setChecked(pobj->getDisabled());
}

void PropertiesSiding:: disablePlatformEnabled(int state){
    if(pobj == NULL) return;
    if(state == Qt::Checked)
        pobj->setDisabled(true);
    else
        pobj->setDisabled(false);
}

void PropertiesSiding::namePlatformEnabled(QString val){
    if(pobj == NULL) return;
    pobj->setPlatformName(val);
}

bool PropertiesSiding::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "siding")
        return true;
    return false;
}