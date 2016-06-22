#include "PropertiesForest.h"
#include "WorldObj.h"
#include "ForestObj.h"

PropertiesForest::PropertiesForest() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    infoLabel = new QLabel("Forest:");
    infoLabel->setStyleSheet("QLabel { color : #999999; }");
    infoLabel->setContentsMargins(3,0,0,0);
    
   
    vbox->addWidget(infoLabel);
    QFormLayout *vlistt = new QFormLayout;
    vlistt->setSpacing(2);
    vlistt->setContentsMargins(3,0,3,0);
    this->tX.setDisabled(true);
    this->tY.setDisabled(true);
    vlistt->addRow("Tile X:",&this->tX);
    vlistt->addRow("Tile Z:",&this->tY);
    vbox->addItem(vlistt);
    
    QLabel * label0 = new QLabel("Texture:");
    label0->setStyleSheet("QLabel { color : #999999; }");
    label0->setContentsMargins(3,0,0,0);
    vbox->addWidget(label0);
    QLabel * label1 = new QLabel("FileName:");
    label1->setContentsMargins(3,0,0,0);
    vbox->addWidget(label1);
    this->fileName.setDisabled(true);
    this->fileName.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->fileName);
    QPushButton *copyF = new QPushButton("Copy FileName", this);
    vbox->addWidget(copyF);
    
    QLabel * label12 = new QLabel("Size:");
    label12->setStyleSheet("QLabel { color : #999999; }");
    label12->setContentsMargins(3,0,0,0);
    vbox->addWidget(label12);
    QFormLayout *vlist0 = new QFormLayout;
    vlist0->setSpacing(2);
    vlist0->setContentsMargins(3,0,3,0);
    vlist0->addRow("Width:",&this->sizeX);
    vlist0->addRow("Height:",&this->sizeY);
    
    sizeX.setValidator( new QDoubleValidator(0, 1000, 2, this) );
    QObject::connect(&sizeX, SIGNAL(textEdited(QString)),
                      this, SLOT(sizeEnabled(QString)));
    sizeY.setValidator( new QDoubleValidator(0, 1000, 2, this) );
    QObject::connect(&sizeY, SIGNAL(textEdited(QString)),
                      this, SLOT(sizeEnabled(QString)));
    
    vlist0->addRow("Population:",&this->population);
    population.setValidator( new QIntValidator(0, 1000000, this) );
    QObject::connect(&population, SIGNAL(textEdited(QString)),
                      this, SLOT(populationEnabled(QString)));
    
    vlist0->addRow("Density/KM:",&this->densitykm);
    densitykm.setValidator( new QIntValidator(0, 1000000, this) );
    QObject::connect(&densitykm, SIGNAL(textEdited(QString)),
                      this, SLOT(densitykmEnabled(QString)));
    vbox->addItem(vlist0);
    
    QLabel * label2 = new QLabel("Position:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    vbox->addWidget(label2);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("X:",&this->posX);
    vlist->addRow("Y:",&this->posY);
    vlist->addRow("Z:",&this->posZ);
    vbox->addItem(vlist);
    QPushButton *copyPos = new QPushButton("Copy Position", this);
    QPushButton *pastePos = new QPushButton("Paste Position", this);
    vbox->addWidget(copyPos);
    vbox->addWidget(pastePos);
    
    QLabel * label3 = new QLabel("QDirection:");
    label3->setStyleSheet("QLabel { color : #999999; }");
    label3->setContentsMargins(3,0,0,0);
    vbox->addWidget(label3);
    this->quat.setDisabled(true);
    this->quat.setAlignment(Qt::AlignCenter);
    vbox->addWidget(&this->quat);
    QPushButton *copyQrot = new QPushButton("Copy Rotation", this);
    QPushButton *pasteQrot = new QPushButton("Paste Rotation", this);
    QPushButton *resetQrot = new QPushButton("Reset Rotation", this);
    QPushButton *qRot90 = new QPushButton("Rotate Y 90°", this);
    vbox->addWidget(copyQrot);
    vbox->addWidget(pasteQrot);
    vbox->addWidget(resetQrot);
    vbox->addWidget(qRot90);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesForest::~PropertiesForest() {
}

void PropertiesForest::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    forestObj = (ForestObj*)obj;
    ForestObj* tobj = (ForestObj*)obj;
        
    this->infoLabel->setText("Object: "+obj->type);
    this->fileName.setText(tobj->treeTexture);
        
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->sizeX.setText(QString::number(tobj->areaX, 'G', 4));
    this->sizeY.setText(QString::number(tobj->areaZ, 'G', 4));
    this->population.setText(QString::number((int)tobj->population, 10));
    this->densitykm.setText(QString::number((int)(tobj->population*(1000000.0/(tobj->areaX*tobj->areaZ))), 10));
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

void PropertiesForest::sizeEnabled(QString val){
    if(forestObj == NULL)
        return;
    forestObj->set("areaX", sizeX.text().toFloat());
    forestObj->set("areaZ", sizeY.text().toFloat());
    forestObj->modified = true;
    forestObj->deleteVBO();
}

void PropertiesForest::populationEnabled(QString val){
    if(forestObj == NULL)
        return;
    forestObj->set("population", population.text().toInt());
    this->densitykm.setText(QString::number((int)(forestObj->population*(1000000.0/(forestObj->areaX*forestObj->areaZ))), 10));
    forestObj->modified = true;
    forestObj->deleteVBO();
}

void PropertiesForest::densitykmEnabled(QString val){
    if(forestObj == NULL)
        return;
    this->population.setText(QString::number((int)(densitykm.text().toUInt()/(1000000.0/(forestObj->areaX*forestObj->areaZ))), 10));
    forestObj->set("population", population.text().toInt());
    forestObj->modified = true;
    forestObj->deleteVBO();
}

bool PropertiesForest::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "forest")
        return true;
    return false;
}
