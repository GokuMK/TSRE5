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
    
    carNumber.setValidator( new QDoubleValidator(0, 100, 1, this) );
    carSpeed.setValidator( new QDoubleValidator(0, 100, 1, this) );
    
    QObject::connect(&carNumber, SIGNAL(textChanged(QString)),
                      this, SLOT(carNumberEnabled(QString)));
    QObject::connect(&carSpeed, SIGNAL(textChanged(QString)),
                      this, SLOT(carSpeedEnabled(QString)));
}

PropertiesCarspawner::~PropertiesCarspawner() {
}

void PropertiesCarspawner::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    
    this->infoLabel->setText("Object: "+obj->type);
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    cobj = (PlatformObj*)obj;
    this->carNumber.setText(QString::number(cobj->getCarNumber(),10));
    this->carSpeed.setText(QString::number(cobj->getCarSpeed(),10));
}

void PropertiesCarspawner::carNumberEnabled(QString val){
    if(cobj == NULL) return;
    cobj->setCarNumber(val.toInt());
}

void PropertiesCarspawner::carSpeedEnabled(QString val){
    if(cobj == NULL) return;
    cobj->setCarSpeed(val.toInt());
}

bool PropertiesCarspawner::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "carspawner")
        return true;
    return false;
}