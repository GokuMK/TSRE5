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
#include "ActivityEventProperties.h"
#include "Activity.h"
#include "ActivityEvent.h"

ActivityEventWindow::ActivityEventWindow(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::WindowType::Tool);
    setWindowTitle(tr("Activity Events"));
    
    eventProperties = new ActivityEventProperties(this);
    
    QVBoxLayout *actionListLayout = new QVBoxLayout;
    actionListLayout->setContentsMargins(0,0,0,0);
    actionListLayout->setSpacing(0);
    QPushButton *bNewActionEvent = new QPushButton("New Action Event");
    QObject::connect(bNewActionEvent, SIGNAL(released()),
                      this, SLOT(bNewEventSelected()));
    QPushButton *bDeleteActionEvent = new QPushButton("Delete");
    QObject::connect(bDeleteActionEvent, SIGNAL(released()),
                      this, SLOT(bDeleteEventSelected()));
    actionListLayout->addWidget(&actionList);
    actionListLayout->addWidget(bNewActionEvent);
    actionListLayout->addWidget(bDeleteActionEvent);
    actionList.setFixedWidth(170);
    actionList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *locationListLayout = new QVBoxLayout;
    locationListLayout->setContentsMargins(0,0,0,0);
    locationListLayout->setSpacing(0);
    QPushButton *bNewLocationEvent = new QPushButton("New Location Event");
    QObject::connect(bNewLocationEvent, SIGNAL(released()),
                      this, SLOT(bNewEventSelected()));
    QPushButton *bDeleteLocationEvent = new QPushButton("Delete");
    QObject::connect(bDeleteLocationEvent, SIGNAL(released()),
                      this, SLOT(bDeleteEventSelected()));
    locationListLayout->addWidget(&locationList);
    locationListLayout->addWidget(bNewLocationEvent);
    locationListLayout->addWidget(bDeleteLocationEvent);
    locationList.setFixedWidth(170);
    locationList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QVBoxLayout *timeListLayout = new QVBoxLayout;
    timeListLayout->setContentsMargins(0,0,0,0);
    timeListLayout->setSpacing(0);
    QPushButton *bNewTimeEvent = new QPushButton("New Time Event");
    QObject::connect(bNewTimeEvent, SIGNAL(released()),
                      this, SLOT(bNewEventSelected()));
    QPushButton *bDeleteTimeEvent = new QPushButton("Delete");
    QObject::connect(bDeleteTimeEvent, SIGNAL(released()),
                      this, SLOT(bDeleteEventSelected()));
    timeListLayout->addWidget(&timeList);
    timeListLayout->addWidget(bNewTimeEvent);
    timeListLayout->addWidget(bDeleteTimeEvent);
    timeList.setFixedWidth(170);
    timeList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    actionWidget.setLayout(actionListLayout);
    locationWidget.setLayout(locationListLayout);
    timeWidget.setLayout(timeListLayout);
    
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setContentsMargins(1,1,1,1);
    tabWidget->addTab(&actionWidget, "Action");
    tabWidget->addTab(&locationWidget, "Location");
    tabWidget->addTab(&timeWidget, "Time");
    
    QHBoxLayout *v = new QHBoxLayout;
    v->setSpacing(2);
    v->setContentsMargins(1,1,1,1);
    v->addWidget(tabWidget);
    v->addWidget(eventProperties);
    this->setLayout(v);
    
    QObject::connect(&actionList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(eventListSelected(QListWidgetItem*)));
    
    QObject::connect(&locationList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(eventListSelected(QListWidgetItem*)));
    
    QObject::connect(&timeList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(eventListSelected(QListWidgetItem*)));
    
    QObject::connect(eventProperties, SIGNAL(eventNameChanged(int)),
                      this, SLOT(eventNameChanged(int)));
    
}

void ActivityEventWindow::eventNameChanged(int id){
    if(activity == NULL)
        return;
    int j = -1;
    /*for(int i = 0; i < activity->event.size(); i++ ){
        if(activity->event[i].id == id){
            j = i;
            break;
        }
    }
    if(j == -1){*/
    if(activity->currentEventSelected == NULL){
        showEvents(activity);
    }

    QListWidget *list = NULL;
    if(activity->currentEventSelected->category == ActivityEvent::CategoryAction)
        list = &actionList;
    if(activity->currentEventSelected->category == ActivityEvent::CategoryLocation)
        list = &locationList;
    if(activity->currentEventSelected->category == ActivityEvent::CategoryTime)
        list = &timeList;

    int idxR = list->currentRow();
    showEvents(activity);
    list->item(idxR)->setSelected(true);
    list->setCurrentRow(idxR);
    eventListSelected(list->item(idxR));
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
    
    eventProperties->setEventList(act->getEventIdNameList());
    eventProperties->setStationStopList(act->getServiceStationStopNameList());
}

void ActivityEventWindow::eventListSelected(QListWidgetItem * item){
    if(activity == NULL)
        return;
    eventProperties->showEvent(&activity->event[item->type()]);
    activity->currentEventSelected = &activity->event[item->type()];
    
    this->resize(this->width(), this->minimumHeight());
}

void ActivityEventWindow::bNewEventSelected(){
    if(activity == NULL)
        return;
    ActivityEvent::EventCategory category = ActivityEvent::CategoryUndefined;

    if(actionWidget.isVisible())
        category = ActivityEvent::CategoryAction;
    if(timeWidget.isVisible())
        category = ActivityEvent::CategoryTime;
    if(locationWidget.isVisible())
        category = ActivityEvent::CategoryLocation;
    
    activity->newEvent(category);
    
    showEvents(activity);
}

void ActivityEventWindow::bDeleteEventSelected(){
    if(activity == NULL)
        return;
    activity->deleteCurrentEvent();
    showEvents(activity);
}

ActivityEventWindow::~ActivityEventWindow() {
}
