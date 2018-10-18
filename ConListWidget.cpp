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
#include "Eng.h"
#include "EngLib.h"
#include "OverwriteDialog.h"

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
    routeShow.setMaxVisibleItems(20);
    routeShow.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    actShow.setStyleSheet("combobox-popup: 0;");
    actShow.setMaxVisibleItems(20);
    actShow.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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
    ConLib::con[ConLib::jestcon] = new Consist(con, false);
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
    QStringList hash;
    QHashIterator<QString, QVector<int>> i(ActLib::route);
    while (i.hasNext()) {
        i.next();
        hash.push_back(i.key());
    }
    hash.sort(Qt::CaseInsensitive);
    hash.removeDuplicates();
    routeShow.addItems(hash);
}

void ConListWidget::routeTChan(QString n){
    actShow.clear();
    items.clear();
    foreach(int id, ActLib::route[n]){
        actShow.addItem(ActLib::Act[id]->header->name, QVariant(id));
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
    //Game::currentEngLib = englib;
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
    //Game::currentEngLib = englib;
    Consist * e;
    
    
    for (int i = 0; i < ActLib::Act[id]->activityObjects.size(); i++){
        e = ActLib::Act[id]->activityObjects[i]->con;
        if(e == NULL) continue;
        new QListWidgetItem ( QString::number(ActLib::Act[id]->activityObjects[i]->id) + " : " + e->showName, &items, i);
    }
}

void ConListWidget::fillConList(QString n){
    totalVal.setText(QString::number(ConLib::jestcon));
    items.clear();
    //Game::currentEngLib = englib;
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

void ConListWidget::updateCurrentCon(){
    if(conShow.currentIndex() > 0)
        return;
    Consist * e;
    int cid = items.currentItem()->type();
    e = ConLib::con[cid];
    if(e == NULL) return;
    if(e->showName.length() > 0)
        items.currentItem()->setText(e->showName);
}

void ConListWidget::deleteCurrentCon(){
    if(conShow.currentIndex() > 0)
        return;
    
    Consist * e;
    int cid = items.currentItem()->type();
    e = ConLib::con[cid];
    if(e == NULL) return;
    
    items.takeItem(items.currentRow());
    
    if(e->isNewConsist()){
        ConLib::con.erase(cid);
        delete e;
        return;
    }
    OverwriteDialog owerwriteDialog;
    owerwriteDialog.setWindowTitle("Delete?");
    owerwriteDialog.label->setText("Delete this consist from disk?\n\n"+
        e->pathid+"\n");
    owerwriteDialog.setFixedWidth(250);
    owerwriteDialog.exec();

    if(owerwriteDialog.changed == 1){
        QFile file (e->pathid);
        file.remove();
    }
    ConLib::con.erase(cid);
    delete e;
}

void ConListWidget::getUnsaed(QVector<int> &unsavedConIds){
    unsavedConIds.clear();
    //Game::currentEngLib = englib;
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

void ConListWidget::getUnsaedAct(QVector<int>& unsavedActIds){
    unsavedActIds.clear();
    //Game::currentEngLib = englib;
    Activity * e;
    for (int i = 0; i < ActLib::jestact; i++){
        e = ActLib::Act[i];
        if(e == NULL) continue;
        if(e->loaded != 1) continue;
        if(e->isUnSaved()){
            unsavedActIds.push_back(i);
        }
    }
}


void ConListWidget::findConsistsByEng(int id){
    query.clear();
    //Game::currentEngLib = englib;
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

void ConListWidget::getEngSets(Eng *e, QVector<int>& sets){
    Consist * c;
    for (int i = 0; i < ConLib::jestcon; i++){
        c = ConLib::con[i];
        if(c == NULL) continue;
        if(c->loaded !=1) continue;
        //qDebug() << c->name.toLower() <<" "<< e->engName.toLower();
        if(c->conName.split("#").first().toLower() == e->engName.toLower()){
            for (int j = 0; j < c->engItems.size(); j++){
                if(c->engItems[j].ename.toLower() == e->engName.toLower()){
                    qDebug() << "set " << c->name;
                    sets.push_back(i);
                    break;
                }
            }
        }
    }
}

void ConListWidget::itemsSelected(QListWidgetItem * item){
    
    //qDebug() << item->type() << " " << item->text();
    if(currentConType == 0)
        emit conListSelected(item->type());
    if(currentConType == 1)
        emit conListSelected(actShow.currentData().toInt(), item->type());
}
