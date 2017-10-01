/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityServiceProperties.h"
#include "Game.h"
#include "Service.h"
#include "ConLib.h"
#include "Consist.h"
#include "Path.h"

ActivityServiceProperties::ActivityServiceProperties(QWidget* parent) : QWidget(parent) {
    //this->setMinimumHeight(400);
    setMinimumWidth(280);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    
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
    QObject::connect(&stationList, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
                      this, SLOT(stationListSelected(QTreeWidgetItem*, int)));

    label = new QLabel("Used by:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    list.clear();
    list.append("Type:");
    list.append("Name:");
    lUsedBy.setColumnCount(2);
    lUsedBy.setHeaderLabels(list);
    lUsedBy.setRootIsDecorated(false);
    lUsedBy.header()->resizeSection(0,50);    
    lUsedBy.setFixedHeight(100);
    vbox->addWidget(&lUsedBy);
    
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vbox);
    
}

ActivityServiceProperties::~ActivityServiceProperties() {
    
}

void ActivityServiceProperties::setPaths(QVector<Path*>& path){
    paths = path;
}

void ActivityServiceProperties::showService(Service *s){
    service = s;
    cConFiles.clear();
    ConLib::loadSimpleList(Game::root);
    cConFiles.addItem("UNDEFINED", "#");
    foreach(QString name, ConLib::conFileList){
        cConFiles.addItem(name.section('/', -1), QVariant(name.section('/', -1)));
    }
    if(s->trainConfig.length() > 0){ 
        cConFiles.setCurrentIndex(cConFiles.findData(s->trainConfig.toLower()+".con"));
    } else {
        cConFiles.setCurrentIndex(0);
    }
    
    cPath.clear();
    cPath.addItem("UNDEFINED", "#");
    for(int i = 0; i < paths.size(); i++ )
        cPath.addItem(paths[i]->displayName, /*QVariant(i)*/QVariant(paths[i]->trPathName));
    if(s->pathId.length() > 0){
        cPath.setCurrentIndex(cPath.findData(s->pathId));
    } else {
        cPath.setCurrentIndex(0);
    }
    
    eFileName.setText(s->nameId);
    eDisplayName.setText(s->displayName);
    ePlayerPerformance.setText(QString::number(s->efficiency));
    eStartSpeed.setText(QString::number(s->startingSpeed));
    eEndSpeed.setText(QString::number(s->endingSpeed));
    
    if(cPath.currentIndex() >= 0)
        setStationList();
    setUsedByList();
}

void ActivityServiceProperties::cPathEnabled(int val){
    if(cPath.currentIndex() <= 0){
        service->setNewPath("");
        setStationList();
        return;
    }
    Path *p = paths[cPath.currentIndex()-1];
    if(p == NULL){
        qDebug() << "p = null 1";
        service->setNewPath("");
        setStationList();
        return;
    }
    service->setNewPath(p->trPathName);
    setStationList();
}

void ActivityServiceProperties::cConFilesEnabled(int val){
    if(cConFiles.currentIndex() <= 0){
        service->trainConfig = "";
        return;
    }
    service->trainConfig = cConFiles.currentData().toString().section(".", 0, -2);
}
/*
void ActivityServiceProperties::bOkEnabled(){
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
*/
void ActivityServiceProperties::setStationList(){
    stationList.clear();
    QStringList list;
    QList<QTreeWidgetItem *> items;
    if(cPath.currentIndex() <= 0){
        return;
    }
    Path *p = paths[cPath.currentIndex()-1];
    if(p == NULL)
        return;
    p->init3dShapes(false);
    qDebug() << cPath.currentIndex()-1;
    qDebug() << p->pathid;
    qDebug() << p->displayName;
    qDebug() << p->nameId;
    qDebug() << p->pathObjects.size();

    int count = -1;
    foreach(Path::PathObject* i, p->pathObjects){
        count++;
        if(i == NULL)
            continue;
        qDebug() << i->name;
        qDebug() << i->distanceDownPath;
        list.clear();
        list.append(QString(""));
        list.append(i->name);
        float dist = i->distanceDownPath;
        dist = 0.01*((int)(dist/10));
        list.append(QString::number(dist));
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, count );
        item->setCheckState(0, Qt::Unchecked);
        for(int ii = 0; ii < service->stationStop.size(); ii++ ){
            if(service->stationStop[ii].skipCount == count)
                item->setCheckState(0, Qt::Checked);
        }
        items.append(item);
    }
    stationList.blockSignals(true);
    stationList.insertTopLevelItems(0, items);
    stationList.blockSignals(false);
}

void ActivityServiceProperties::setUsedByList(){
    lUsedBy.clear();
    QStringList list;
    QList<QTreeWidgetItem *> items;
    
    for(int i = 0; i < 1; i++){
        list.clear();
        list.append(QString("Activity"));
        list.append(QString("ActivityName"));
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, i );
        items.append(item);
    }  
    lUsedBy.insertTopLevelItems(0, items);
}

void ActivityServiceProperties::stationListSelected(QTreeWidgetItem* item, int column){
    if(item->checkState(0) == Qt::Checked){
        qDebug() << "item->checkState(0) == Qt::Checked";
        service->enableStationStop(item->type());
    }else{
        qDebug() << "item->checkState(0) ! Qt::Checked";
        service->disableStationStop(item->type());
    }
}