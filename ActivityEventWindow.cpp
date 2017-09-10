/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityEventWindow.h"
#include "ActivityEventActionProperties.h"
#include "ActivityEventLocationProperties.h"
#include "ActivityEventTimeProperties.h"
#include "Activity.h"
#include "ActivityEvent.h"

ActivityEventWindow::ActivityEventWindow(QWidget* parent) : QWidget(parent) {
    this->setWindowFlags(Qt::WindowType::Tool);
    this->setWindowTitle(tr("Activity Events"));
    
    eventActionProperties = new ActivityEventActionProperties(this);
    eventLocationProperties = new ActivityEventLocationProperties(this);
    eventTimeProperties = new ActivityEventTimeProperties(this);
    
    QHBoxLayout *actionLayout = new QHBoxLayout;
    actionLayout->setSpacing(2);
    actionLayout->setContentsMargins(1,1,1,1);
    QVBoxLayout *actionListLayout = new QVBoxLayout;
    actionListLayout->setContentsMargins(0,0,0,0);
    actionListLayout->setSpacing(0);
    QPushButton *bNewActionEvent = new QPushButton("New Action Event");
    actionListLayout->addWidget(&actionList);
    actionListLayout->addWidget(bNewActionEvent);
    actionLayout->addItem(actionListLayout);
    actionList.setFixedWidth(200);
    actionList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    actionLayout->addWidget(eventActionProperties);
    QHBoxLayout *locationLayout = new QHBoxLayout;
    locationLayout->setSpacing(2);
    locationLayout->setContentsMargins(1,1,1,1);
    QVBoxLayout *locationListLayout = new QVBoxLayout;
    locationListLayout->setContentsMargins(0,0,0,0);
    locationListLayout->setSpacing(0);
    QPushButton *bNewLocationEvent = new QPushButton("New Location Event");
    locationListLayout->addWidget(&locationList);
    locationListLayout->addWidget(bNewLocationEvent);
    locationLayout->addItem(locationListLayout);
    locationList.setFixedWidth(200);
    locationList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    locationLayout->addWidget(eventLocationProperties);
    QHBoxLayout *timeLayout = new QHBoxLayout;
    timeLayout->setSpacing(2);
    timeLayout->setContentsMargins(1,1,1,1);
    QVBoxLayout *timeListLayout = new QVBoxLayout;
    timeListLayout->setContentsMargins(0,0,0,0);
    timeListLayout->setSpacing(0);
    QPushButton *bNewTimeEvent = new QPushButton("New Time Event");
    timeListLayout->addWidget(&timeList);
    timeListLayout->addWidget(bNewTimeEvent);
    timeLayout->addItem(timeListLayout);
    timeList.setFixedWidth(200);
    timeList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    timeLayout->addWidget(eventTimeProperties);
    
    actionWidget.setLayout(actionLayout);
    locationWidget.setLayout(locationLayout);
    timeWidget.setLayout(timeLayout);
    
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setContentsMargins(1,1,1,1);
    tabWidget->addTab(&actionWidget, "Action Events");
    tabWidget->addTab(&locationWidget, "Location Events");
    tabWidget->addTab(&timeWidget, "Time Events");
    
    QVBoxLayout *v = new QVBoxLayout;
    v->setSpacing(2);
    v->setContentsMargins(1,1,1,1);
    v->addWidget(tabWidget);
    this->setLayout(v);
    
     QObject::connect(&actionList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(actionListSelected(QListWidgetItem*)));
}

void ActivityEventWindow::showEvents(Activity* act){
    if(this->isHidden())
        return;
    activity = act;
    
    actionList.clear();
    locationList.clear();
    timeList.clear();
    
    for(int i = 0; i < activity->event.size(); i++ ){
        if(activity->event[i].category == ActivityEvent::CategoryAction){
            new QListWidgetItem ( activity->event[i].name, &actionList, i );
        }
        if(activity->event[i].category == ActivityEvent::CategoryLocation){
            new QListWidgetItem ( activity->event[i].name, &locationList, i );
        }
        if(activity->event[i].category == ActivityEvent::CategoryTime){
            new QListWidgetItem ( activity->event[i].name, &timeList, i );
        }
    }
    
    
}

void ActivityEventWindow::actionListSelected(QListWidgetItem * item){
    eventActionProperties->showEvent(&activity->event[item->type()]);
}

ActivityEventWindow::~ActivityEventWindow() {
}

