/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityTrafficProperties.h"
#include <QTime>
#include "Game.h"
#include "Service.h"
#include "Traffic.h"
#include "ActivityTimetable.h"
#include "ActLib.h"

ActivityTrafficProperties::ActivityTrafficProperties(QWidget* parent) : QWidget(parent) {
    setMinimumWidth(350);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);

    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    int row = 0;
    QLabel *label = NULL;
    
    label = new QLabel("Name:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eFileName, row++, 1);
    //QObject::connect(&eFileName, SIGNAL(textEdited(QString)), this, SLOT(eFileNameSelected(QString)));
    label = new QLabel("Display Name:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eDisplayName, row++, 1);
    //QObject::connect(&eDisplayName, SIGNAL(textEdited(QString)), this, SLOT(eDisplayNameSelected(QString)));
        
    label = new QLabel("Services:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    QStringList list;
    list.append("Name:");
    list.append("Start Time:");
    lServcies.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lServcies.setColumnCount(2);
    lServcies.setHeaderLabels(list);
    lServcies.setRootIsDecorated(false);
    lServcies.header()->resizeSection(0,200);    
    lServcies.header()->resizeSection(1,100);    
    vlist->addWidget(&lServcies, row++, 0, 1, 2);
    QPushButton *bAddOutcome = new QPushButton("Add New");
    QObject::connect(bAddOutcome, SIGNAL(released()),
                      this, SLOT(bAddServiceSelected()));
    vlist->addWidget(bAddOutcome, row++, 0, 1, 2);
    QPushButton *bRemoveOutcome = new QPushButton("Remove Selected");
    QObject::connect(bRemoveOutcome, SIGNAL(released()),
                      this, SLOT(bRemoveServiceSelected()));
    vlist->addWidget(bRemoveOutcome, row++, 0, 1, 2);
    QPushButton *bCloneOutcome = new QPushButton("Clone Selected");
    QObject::connect(bCloneOutcome, SIGNAL(released()),
                      this, SLOT(bCloneOutcomeSelected()));
    vlist->addWidget(bCloneOutcome, row++, 0, 1, 2);
    
    label = new QLabel("Selected Service:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    vlist->addWidget(new QLabel("Service:"), row, 0);
    cServiceList.setStyleSheet("combobox-popup: 0;");
    cServiceList.setMaxVisibleItems(30);
    cServiceList.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vlist->addWidget(&cServiceList, row++, 1);
    QObject::connect(&cServiceList, SIGNAL(activated(QString)),
                      this, SLOT(serviceActoionListSelected(QString)));
    vlist->addWidget(new QLabel("Start Time:"), row, 0);
    eTime.setDisplayFormat("HH:mm:ss");
    vlist->addWidget(&eTime, row++, 1);
    QObject::connect(&eTime, SIGNAL(timeChanged(QTime)),
                      this, SLOT(eTimeSelected(QTime)));
    vbox->addItem(vlist);

    //vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&lServcies, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                      this, SLOT(lServciesSelected(QTreeWidgetItem*, int)));

}

ActivityTrafficProperties::~ActivityTrafficProperties() {
}

void ActivityTrafficProperties::showTraffic(Traffic* t){
    traffic = t;
    eFileName.setText(t->name);
    eDisplayName.setText(t->nameId);
    
    lServcies.clear();
    QList<QTreeWidgetItem *> items;
    QStringList list;
    for(int i = 0; i < traffic->service.size(); i++ ){
        if(traffic->service[i] == NULL)
            continue;
        //new QListWidgetItem ( route->service[i]->displayName, &serviceList, i );
        list.clear();
        list.append(traffic->service[i]->name);
        QTime time = QTime::fromMSecsSinceStartOfDay((traffic->service[i]->time*1000));
        list.append(time.toString("HH:mm:ss"));
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, i );
        items.append(item);
    }
    lServcies.insertTopLevelItems(0, items);
}

void ActivityTrafficProperties::lServciesSelected(QTreeWidgetItem* item, int column){
    if(traffic == NULL)
        return;
    ActivityTimetable* s = traffic->service[item->type()];
    if(s == NULL)
        return;
    
    cServiceList.clear();
    cServiceList.addItem("UNDEFINED", "");
    for(int i = 0; i < ActLib::jestservice; i++){
        if(ActLib::Services[i] == NULL)
            continue;
        cServiceList.addItem(ActLib::Services[i]->displayName, ActLib::Services[i]->nameId );
    }
    int index = cServiceList.findData(s->name.toLower(), Qt::UserRole, static_cast<Qt::MatchFlags>(Qt::CaseInsensitive));
    if(index < 0)
        index = 0;
    cServiceList.setCurrentIndex(index);
    
    //cOutcome.setCurrentIndex(cOutcome.findData((int)outcome->type));
    //cWeatherChange.setText(outcome->value.toString());
    eTime.setTime(QTime::fromMSecsSinceStartOfDay((s->time*1000)));
}

void ActivityTrafficProperties::bCloneOutcomeSelected(){
    if(traffic == NULL)
        return;
    
    ActivityTimetable* s = traffic->service[lServcies.currentItem()->type()];
    if(s == NULL)
        return;
    
    QDialog dialog;
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    QLineEdit number, mins;
    number.setText("0");
    mins.setText("15");
    vlist->addRow("Number of services:",&number);
    vlist->addRow("Duration:",&mins);
    dialog.setLayout(vlist);
    dialog.exec();
    
    bool ok = false;
    int count = number.text().toInt(&ok);
    if(!ok)
        return;
    int secs = mins.text().toInt(&ok)*60;
    if(!ok)
        return;
    
    for(int i = 0; i < count; i++){
        traffic->service.push_back(new ActivityTimetable());
        traffic->service.back()->setService(s->name);
        traffic->service.back()->setTime(s->time + secs*(i+1));
    }
    
    showTraffic(traffic);
}

void ActivityTrafficProperties::bAddServiceSelected(){
    if(traffic == NULL)
        return;
    traffic->service.push_back(new ActivityTimetable());
    //QModelIndex row = lServcies.currentIndex();
    showTraffic(traffic);
    //lServcies.setCurrentIndex(row);
    //serviceActoionListSelected();
}

void ActivityTrafficProperties::bRemoveServiceSelected(){
    
    traffic->service.remove(lServcies.currentItem()->type());
    
    showTraffic(traffic);
}

void ActivityTrafficProperties::serviceActoionListSelected(QString val){
    QString index = "";
    if(cServiceList.currentIndex() > 0)
        index = cServiceList.currentData().toString();
    
    ActivityTimetable* s = traffic->service[lServcies.currentItem()->type()];
    if(s == NULL)
        return;
    
    if(index == "")
        s->clear();
    else
        s->setService(index);
    
    lServcies.currentItem()->setText(0, s->name);
}

void ActivityTrafficProperties::eTimeSelected(QTime val){
    if(traffic == NULL)
        return;
    ActivityTimetable* s = traffic->service[lServcies.currentItem()->type()];
    if(s == NULL)
        return;
    s->setTime(val.msecsSinceStartOfDay()/1000);
    lServcies.currentItem()->setText(1, val.toString("HH:mm:ss"));
}