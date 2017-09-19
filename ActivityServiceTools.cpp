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
    QObject::connect(&cConFiles, SIGNAL(activated(int)),
                      this, SLOT(cConFilesEnabled(int)));
    
    label = new QLabel("Path:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cPath);
    cPath.setStyleSheet("combobox-popup: 0;");
    cPath.setMaxVisibleItems(35);
    cPath.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QObject::connect(&cPath, SIGNAL(activated(int)),
                      this, SLOT(cPathEnabled(int)));

    QStringList list;
    list.append("Stop:");
    list.append("Station Name:");
    list.append("Distance (km):");
    stationList.setColumnCount(3);
    stationList.setHeaderLabels(list);
    stationList.setRootIsDecorated(false);
    stationList.header()->resizeSection(0,30);
    stationList.header()->resizeSection(1,150);    
    stationList.header()->resizeSection(2,50);    
    vbox->addWidget(&stationList);
    
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
    paths = path;
    cConFiles.clear();
    ConLib::loadSimpleList(Game::root);
    foreach(QString name, ConLib::conFileList){
        cConFiles.addItem(name.section('/', -1), QVariant(name.section('/', -1)));
    }
    
    cPath.clear();
    for(int i = 0; i < path.size(); i++ )
        cPath.addItem(path[i]->displayName, /*QVariant(i)*/QVariant(path[i]->nameId));
        
    if(s->pathId.length() > 0)
        cPath.setCurrentIndex(cPath.findData(s->pathId));
    
    eFileName.setText(s->nameId);
    eDisplayName.setText(s->displayName);
    ePlayerPerformance.setText(QString::number(s->efficiency));
    eStartSpeed.setText(QString::number(s->startingSpeed));
    eEndSpeed.setText(QString::number(s->endingSpeed));
    
    setStationList();
}

void ActivityServiceTools::bCancelEnabled(){
    this->changed = false;
    this->close();
}

void ActivityServiceTools::cPathEnabled(int val){
    setStationList();
}

void ActivityServiceTools::cConFilesEnabled(int val){

}

void ActivityServiceTools::bOkEnabled(){
    this->changed = true;
    service->setNameId(eFileName.text());
    service->displayName = eDisplayName.text();
    service->efficiency = ePlayerPerformance.text().toFloat();
    service->startingSpeed = eStartSpeed.text().toFloat();
    service->endingSpeed = eEndSpeed.text().toFloat();
    service->trainConfig = cConFiles.currentData().toString().section('.', 0, -2);
    service->pathId = cPath.currentData().toString();
    this->close();
}

void ActivityServiceTools::setStationList(){
    stationList.clear();
    QStringList list;
    QList<QTreeWidgetItem *> items;
    Path *p = paths[cPath.currentIndex()];
    if(p == NULL)
        return;
    p->init3dShapes(false);
    qDebug() << cPath.currentIndex();
    qDebug() << p->pathid;
    qDebug() << p->displayName;
    qDebug() << p->nameId;
    qDebug() << p->pathObjects.size();
    QMapIterator<float, Path::PathObject*> i(p->pathObjects);
    
    
    while (i.hasNext()) {
        i.next();
        if(i.value() == NULL)
            continue;
        qDebug() << i.key();
        qDebug() << i.value();
        qDebug() << i.value()->name;
        qDebug() << i.value()->distanceDownPath;
        list.clear();
        list.append(QString(""));
        list.append(i.value()->name);
        float dist = i.value()->distanceDownPath;
        dist = 0.01*((int)(dist/10));
        list.append(QString::number(dist));
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, i.key() );
        item->setCheckState(0, Qt::Checked);
        items.append(item);
    }  
    stationList.insertTopLevelItems(0, items);
}