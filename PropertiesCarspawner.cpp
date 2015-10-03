#include "PropertiesCarspawner.h"
#include "WorldObj.h"

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
    /*this->fileName.setText(obj->fileName);
    
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->posX.setText(QString::number(obj->position[0], 'G', 4));
    this->posY.setText(QString::number(obj->position[1], 'G', 4));
    this->posZ.setText(QString::number(obj->position[2], 'G', 4));
    this->quat.setText(
            QString::number(obj->qDirection[0], 'G', 4) + " " +
            QString::number(obj->qDirection[1], 'G', 4) + " " +
            QString::number(obj->qDirection[2], 'G', 4) + " " +
            QString::number(obj->qDirection[3], 'G', 4)
            );
    
    
    int l;
    QString flags;
    flags = QString::number(obj->staticFlags, 16);
    l = flags.length();
    for(int i=0; i<8-l; i++)
        flags = "0"+flags;
    this->flags.setText(flags);*/
}

bool PropertiesCarspawner::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "carspawner")
        return true;
    return false;
}