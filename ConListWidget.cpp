/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ConListWidget.h"
#include "ConLib.h"
#include "Consist.h"
#include "Game.h"
#include "ActLib.h"
#include "Activity.h"

ConListWidget::ConListWidget() : QWidget(){
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Total:",&totalVal);
    QLabel *lshow = new QLabel("Show:");
    lshow->setMinimumWidth(50);
    QLabel *lroute = new QLabel("Route:");
    lroute->setMinimumWidth(50);
    QLabel *lfilter = new QLabel("Filter:");
    lfilter->setMinimumWidth(50);
    vlist->addRow(lshow,&conShow);
    vbox->addItem(vlist);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow(lfilter,&conType);
    conTypeList.setLayout(vlist);
    vbox->addWidget(&conTypeList);
    vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow(lroute,&routeShow);
    vlist->addRow("Activity:",&actShow);
    actTypeList.setLayout(vlist);
    vbox->addWidget(&actTypeList);
    vbox->addWidget(&items);
    //vbox->addStretch(1);
    this->setLayout(vbox);

    routeShow.setStyleSheet("combobox-popup: 0;");
    actShow.setStyleSheet("combobox-popup: 0;");
    conShow.setStyleSheet("combobox-popup: 0;");
    conShow.addItem("Consists");
    conShow.addItem("Activity Consists");
    
    conType.setStyleSheet("combobox-popup: 0;");
    conType.addItem("ALL");
    conType.addItem("Broken");
    conType.addItem("Unsaved");
    conType.addItem("Last Query");
    this->setMinimumWidth(250);
    
    actTypeList.hide();

    QObject::connect(&items, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(itemsSelected(QListWidgetItem*)));
    
    QObject::connect(&conType, SIGNAL(activated(QString)),
                      this, SLOT(conFChan(QString)));
    QObject::connect(&conShow, SIGNAL(activated(QString)),
                      this, SLOT(conTChan(QString)));
    QObject::connect(&routeShow, SIGNAL(activated(QString)),
                      this, SLOT(routeTChan(QString)));
    QObject::connect(&actShow, SIGNAL(activated(QString)),
                      this, SLOT(actTChan(QString)));
    
    items.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    totalVal.setEnabled(false);
}

ConListWidget::~ConListWidget() {
}

bool ConListWidget::isActivity(){
    if(conShow.currentIndex() == 0)
        return false;
    return true;
}

int ConListWidget::getCurrentActivityId(){
    return actShow.currentData().toInt();
}

void ConListWidget::newConsist(){
    ConLib::con[ConLib::jestcon] = new Consist();
    ConLib::con[ConLib::jestcon]->setNewConsistFlag();
    ConLib::con[ConLib::jestcon]->showName = "new";
    items.setCurrentItem(new QListWidgetItem ( ConLib::con[ConLib::jestcon]->showName, &items, ConLib::jestcon));
    emit conListSelected(ConLib::jestcon);
    ConLib::jestcon++;
}

void ConListWidget::newConsist(Consist * con){
    ConLib::con[ConLib::jestcon] = new Consist(con);
    ConLib::con[ConLib::jestcon]->setNewConsistFlag();
    if(ConLib::con[ConLib::jestcon]->showName.length() < 1)
        ConLib::con[ConLib::jestcon]->showName = "new";
    items.setCurrentItem(new QListWidgetItem ( ConLib::con[ConLib::jestcon]->showName, &items, ConLib::jestcon));
    emit conListSelected(ConLib::jestcon);
    ConLib::jestcon++;
}

void ConListWidget::conTChan(QString n){
    if(conShow.currentIndex() == 0){
        conTypeList.show();
        actTypeList.hide();
        conFChan("");
        currentConType = 0;
    } else {
        conTypeList.hide();
        actTypeList.show();
        routeFill();
        currentConType = 1;
    }
}

void ConListWidget::routeFill(){
    routeShow.clear();
    items.clear();
    //foreach(int id, ActLib::route){
    //    routeShow.addItem(ActLib::act[id]->hname);
    //}
    for ( auto it = ActLib::route.begin(); it != ActLib::route.end(); ++it ){
        routeShow.addItem(QString::fromStdString(it->first));
    }
}

void ConListWidget::routeTChan(QString n){
    actShow.clear();
    items.clear();
    foreach(int id, ActLib::route[n.toStdString()]){
        actShow.addItem(ActLib::act[id]->header->name, QVariant(id));
    }
}

void ConListWidget::actTChan(QString n){
    //actShow.clear();
    //foreach(int id, ActLib::route[n.toStdString()]){
    //    actShow.addItem(ActLib::act[id]->hname);
    //}
    fillConListAct();
}
void ConListWidget::conFChan(QString n){
    if(conType.currentIndex() == 0)
        n = "";
    if(n.toLower() == "last query"){
        fillConListLastQuery();
        return;
    }
    fillConList(n);
}

void ConListWidget::fillConList(){
    fillConList("");
}

void ConListWidget::fillConListLastQuery(){
    items.clear();
    Game::currentEngLib = englib;
    Consist * e;
    
    for (int i = 0; i < query.count(); i++){
        new QListWidgetItem ( query.item(i)->text(), &items, query.item(i)->type());
    }
    items.sortItems(Qt::AscendingOrder);
}

void ConListWidget::fillConListAct(){
    //std::string rname = this->routeShow.currentText();
    //std::string aname = this->actShow.currentText();
    int id = actShow.currentData().toInt();
    items.clear();
    Game::currentEngLib = englib;
    Consist * e;
    
    
    for (int i = 0; i < ActLib::act[id]->activityObjects.size(); i++){
        e = ActLib::act[id]->activityObjects[i].con;
        if(e == NULL) continue;
        new QListWidgetItem ( e->showName, &items, i);
    }
}

void ConListWidget::fillConList(QString n){
    totalVal.setText(QString::number(ConLib::jestcon));
    items.clear();
    Game::currentEngLib = englib;
    Consist * e;
    bool brokenf = false;
    bool unsavedf = false;

    if(n.toLower() == "broken")
        brokenf = true;
    if(n.toLower() == "unsaved")
        unsavedf = true;
    
    for (int i = 0; i < ConLib::jestcon; i++){
        e = ConLib::con[i];
        if(e == NULL) continue;
        if(e->loaded !=1) continue;
        if(brokenf)
            if(!e->isBroken()) continue;
        if(unsavedf)
            if(!e->isUnSaved()) continue;
        new QListWidgetItem ( e->showName, &items, i);
    }
    items.sortItems(Qt::AscendingOrder);
}

void ConListWidget::getUnsaed(std::vector<int> &unsavedConIds){
    unsavedConIds.clear();
    Game::currentEngLib = englib;
    Consist * e;
    for (int i = 0; i < ConLib::jestcon; i++){
        e = ConLib::con[i];
        if(e == NULL) continue;
        if(e->loaded !=1) continue;
        if(e->isUnSaved()){
            unsavedConIds.push_back(i);
        }
    }
}

void ConListWidget::getUnsaedAct(std::vector<int>& unsavedActIds){
    unsavedActIds.clear();
    Game::currentEngLib = englib;
    Activity * e;
    for (int i = 0; i < ActLib::jestact; i++){
        e = ActLib::act[i];
        if(e == NULL) continue;
        if(e->loaded != 1) continue;
        if(e->isUnSaved()){
            unsavedActIds.push_back(i);
        }
    }
}


void ConListWidget::findConsistsByEng(int id){
    query.clear();
    Game::currentEngLib = englib;
    Consist * e;
    
    for (int i = 0; i < ConLib::jestcon; i++){
        e = ConLib::con[i];
        if(e == NULL) continue;
        if(e->loaded !=1) continue;
        for (int j = 0; j < e->engItems.size(); j++){
            if(e->engItems[j].eng == id){
                new QListWidgetItem ( e->showName, &query, i);
                break;
            }
        }
    }
    query.sortItems(Qt::AscendingOrder);
    conType.setCurrentText("Last Query");
    fillConListLastQuery();
    
}

void ConListWidget::itemsSelected(QListWidgetItem * item){
    
    //qDebug() << item->type() << " " << item->text();
    if(currentConType == 0)
        emit conListSelected(item->type());
    if(currentConType == 1)
        emit conListSelected(actShow.currentData().toInt(), item->type());
}
