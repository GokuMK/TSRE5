/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityServiceTools.h"
#include "Game.h"
#include "Service.h"
#include "ConLib.h"
#include "Consist.h"
#include "Path.h"

ActivityServiceTools::ActivityServiceTools() : QDialog(){
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    QLabel *label = new QLabel("Service Info:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("File Name:",&eFileName);
    vlist->addRow("Display Name:",&eDisplayName);
    vbox->addItem(vlist);
    
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Expected Player Performance:",&ePlayerPerformance);
    vlist->addRow("Start Speed:",&eStartSpeed);
    vlist->addRow("End Speed:",&eEndSpeed);
    vbox->addItem(vlist);
    
    label = new QLabel("Consist:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cConFiles);
    cConFiles.setStyleSheet("combobox-popup: 0;");
    cConFiles.setMaxVisibleItems(35);
    cConFiles.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    label = new QLabel("Path:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cPath);
    cPath.setStyleSheet("combobox-popup: 0;");
    cPath.setMaxVisibleItems(35);
    cPath.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    QPushButton * bOk = new QPushButton("OK");
    QObject::connect(bOk, SIGNAL(released()), this, SLOT(bOkEnabled()));
    QPushButton * bCancel = new QPushButton("Cancel");
    QObject::connect(bCancel, SIGNAL(released()), this, SLOT(bCancelEnabled()));
    QGridLayout *vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(1,0,3,0);
    vlist1->addWidget(bOk, 0, 0);
    vlist1->addWidget(bCancel, 0, 1);
    vbox->addItem(vlist1);
    
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vbox);
    
}

ActivityServiceTools::~ActivityServiceTools() {
    
}

void ActivityServiceTools::setData(Service *s, QVector<Path*> &path){
    service = s;
    
    ConLib::loadSimpleList(Game::root);
    foreach(QString name, ConLib::conFileList){
        cConFiles.addItem(name.section('/', -1), QVariant(name));
    }
        cPath.clear();
    for(int i = 0; i < path.size(); i++ )
        cPath.addItem(path[i]->displayName, QVariant(i));
        
    eFileName.setText(s->nameId);
    eDisplayName.setText(s->displayName);
    ePlayerPerformance.setText(QString::number(s->efficiency));
    eStartSpeed.setText(QString::number(s->startingSpeed));
    eEndSpeed.setText(QString::number(s->endingSpeed));
}

void ActivityServiceTools::bCancelEnabled(){
    this->changed = false;
    this->close();
}
void ActivityServiceTools::bOkEnabled(){
    this->changed = true;
    service->setNameId(eFileName.text());
    service->displayName = eDisplayName.text();
    service->efficiency = ePlayerPerformance.text().toFloat();
    service->startingSpeed = eStartSpeed.text().toFloat();
    service->endingSpeed = eEndSpeed.text().toFloat();
    this->close();
}