/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityTimetableProperties.h"
#include "Service.h"
#include "Activity.h"
#include "ActivityTimetable.h"
#include "Game.h"
#include "TDB.h"
#include "TRitem.h"

ActivityTimetableProperties::ActivityTimetableProperties(QWidget* parent) : QWidget(parent) {
    setMinimumWidth(450);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);

    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    int row = 0;
    QLabel *label = NULL;
    
    QStringList list;
    list.append("Station:");
    list.append("Arrive:");
    list.append("Depart:");
    list.append("Performance:");
    //lTimetable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //lTimetable.setColumnCount(4);
    //lTimetable.setHeaderLabels(list);
    //lTimetable.setRootIsDecorated(false);
    //lTimetable.header()->resizeSection(0,200);    
    //lTimetable.header()->resizeSection(1,100);    
    //tableWidget->setRowCount(10);
    lTimetable.setColumnCount(4);
    lTimetable.setHorizontalHeaderLabels(list);
    lTimetable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lTimetable.verticalHeader()->hide();
    lTimetable.verticalHeader()->setDefaultSectionSize(25);
    lTimetable.horizontalHeader()->resizeSection(0,220);
    lTimetable.horizontalHeader()->resizeSection(1,60);
    lTimetable.horizontalHeader()->resizeSection(2,60);
    lTimetable.horizontalHeader()->resizeSection(3,100);
    lTimetable.horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    vlist->addWidget(&lTimetable, row++, 0, 1, 2);
    QPushButton *bAddOutcome = new QPushButton("Calculate");
    QObject::connect(bAddOutcome, SIGNAL(released()),
                      this, SLOT(bAddOutcomeSelected()));
    
    vlist->addWidget(bAddOutcome, row++, 0, 1, 2);
        vbox->addItem(vlist);

    //vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&lTimetable, SIGNAL(cellChanged(int, int)),
                      this, SLOT(lTimetableSelected(int, int)));
}

ActivityTimetableProperties::~ActivityTimetableProperties() {
}

void ActivityTimetableProperties::showTimetable(ActivityServiceDefinition* s){
    service = s;
    ActivityTimetable *t = service->trafficDefinition;
    if(t == NULL)
        return;
    
    QTableWidgetItem *newItem;
    QTime time;
    TDB *tdb = Game::trackDB;
    if(tdb == NULL)
        return;
    
    lTimetable.blockSignals(true);
    lTimetable.clearContents();
    lTimetable.setRowCount(0);
    QString name = "";
    for(int i = 0; i < t->platformStartID.size(); i++){
        TRitem *trit = tdb->trackItems[t->platformStartID[i]];
        if(trit != NULL){
            name = trit->stationName;
        } else {
            name = "Platform ID: "+QString::number(t->platformStartID[i]);
        }
        newItem = new QTableWidgetItem(name);
        lTimetable.insertRow(lTimetable.rowCount());
        lTimetable.setItem(i, 0, newItem);
        time = QTime::fromMSecsSinceStartOfDay(t->arrivalTime[i]*1000);
        newItem = new QTableWidgetItem(time.toString("HH:mm:ss"));
        lTimetable.setItem(i, 1, newItem);
        time = QTime::fromMSecsSinceStartOfDay(t->departTime[i]*1000);
        newItem = new QTableWidgetItem(time.toString("HH:mm:ss"));
        lTimetable.setItem(i, 2, newItem);
        newItem = new QTableWidgetItem(QString::number(s->efficiency[i]));
        lTimetable.setItem(i, 3, newItem);
    }
    lTimetable.blockSignals(false);
}

void ActivityTimetableProperties::lTimetableSelected(int row, int column){
     qDebug() << "column" << column;
    
    if(service == NULL)
        return;
    ActivityTimetable *t = service->trafficDefinition;
    
    if(column < 1)
        return;
    QTime time;
    if(column == 1){
        time = QTime::fromString(lTimetable.item(row, column)->text(), "HH:mm:ss");
        t->setArrival(row, time.msecsSinceStartOfDay()/1000);
    }
    if(column == 2){
        time = QTime::fromString(lTimetable.item(row, column)->text(), "HH:mm:ss");
        t->setDepart(row, time.msecsSinceStartOfDay()/1000);
    }
    if(column == 3){
        service->setTimetableEfficiency(row, lTimetable.item(row, column)->text().toFloat());
    }
}