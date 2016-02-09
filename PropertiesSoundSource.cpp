#include "PropertiesSoundSource.h"
#include "SoundSourceObj.h"
#include "Game.h"
#include "SoundList.h"

PropertiesSoundSource::PropertiesSoundSource() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    infoLabel = new QLabel("Sound Source:");
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
    // Info
    QLabel* label = new QLabel("FileName:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->fileName);
    label = new QLabel("Source name:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->sName);
    // Edit
    label = new QLabel("Change source:");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&this->sources);
    this->sources.setStyleSheet("combobox-popup: 0;");
    QPushButton *set = new QPushButton("Set", this);
    vbox->addWidget(set);
    vbox->addStretch(1);
    this->setLayout(vbox);
}

PropertiesSoundSource::~PropertiesSoundSource() {
}

void PropertiesSoundSource::showObj(WorldObj* obj){
    if(obj == NULL){
        infoLabel->setText("NULL");
        return;
    }
    sobj = (SoundSourceObj*)obj;
    this->fileName.setText(sobj->fileName);
    this->uid.setText(QString::number(obj->UiD, 10));
    this->tX.setText(QString::number(obj->x, 10));
    this->tY.setText(QString::number(-obj->y, 10));
    this->infoLabel->setText("Object: "+obj->type);
    
    if(Game::soundList != NULL)
        for (auto it = Game::soundList->sources.begin(); it != Game::soundList->sources.end(); ++it ){
            if(it->second->file1.toLower() == sobj->fileName.toLower())
                this->sName.setText(it->second->name);
            this->sources.addItem(it->second->name);
        }
}

bool PropertiesSoundSource::support(WorldObj* obj){
    if(obj == NULL)
        return false;
    if(obj->type == "soundsource")
        return true;
    return false;
}