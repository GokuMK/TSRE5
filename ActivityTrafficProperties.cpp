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
    QObject::connect(&eFileName, SIGNAL(editingFinished(QString)),
                      this, SLOT(eFileNameSelected(QString)));
    label = new QLabel("Display Name:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eDisplayName, row++, 1);
    QObject::connect(&eDisplayName, SIGNAL(editingFinished(QString)),
                      this, SLOT(eDisplayNameSelected(QString)));
        
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
    QPushButton *bRemoveOutcome = new QPushButton("Remove Selected");
    QObject::connect(bAddOutcome, SIGNAL(released()),
                      this, SLOT(bAddOutcomeSelected()));
    QObject::connect(bRemoveOutcome, SIGNAL(released()),
                      this, SLOT(bRemoveOutcomeSelected()));
    
    vlist->addWidget(bAddOutcome, row++, 0, 1, 2);
    vlist->addWidget(bRemoveOutcome, row++, 0, 1, 2);
    
    label = new QLabel("Selected Service:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    vlist->addWidget(new QLabel("Service:"), row, 0);
    vlist->addWidget(&cServiceList, row++, 1);
    QObject::connect(&cServiceList, SIGNAL(activated(QString)),
                      this, SLOT(outcomeActoionListSelected(QString)));
    vlist->addWidget(new QLabel("Start Time:"), row, 0);
    eTime.setDisplayFormat("HH:mm:ss");
    vlist->addWidget(&eTime, row++, 1);
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
        
    //cOutcome.setCurrentIndex(cOutcome.findData((int)outcome->type));
    //cWeatherChange.setText(outcome->value.toString());
    eTime.setTime(QTime::fromMSecsSinceStartOfDay((s->time*1000)));
}