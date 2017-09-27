/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityEventProperties.h"
#include "Game.h"
#include "ActivityEvent.h"
#include "IghCoords.h"
#include "EditFileNameDialog.h"
#include "GameObj.h"
#include "WorldObj.h"

ActivityEventProperties::ActivityEventProperties(QWidget* parent) : QWidget(parent) {
    //this->setMinimumHeight(400);
    setMinimumWidth(350);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        
    QMapIterator<ActivityEvent::EventType, QString> i1(ActivityEvent::EventTypeDescription);
    while (i1.hasNext()) {
        i1.next();
        if(i1.key() == ActivityEvent::EventTypeNone)
            continue;
        if(i1.key() == ActivityEvent::EventTypeLocation)
            continue;
        if(i1.key() == ActivityEvent::EventTypeTime)
            continue;
        cActionType.addItem(i1.value(), i1.key());
    }

    QMapIterator<ActivityEvent::Outcome::OutcomeType, QString> i2(ActivityEvent::Outcome::OutcomeTypeDescription);
    while (i2.hasNext()) {
        i2.next();
        cOutcome.addItem(i2.value(), i2.key());
    }
    
    buttonTools["pickNewEventLocationTool"] = new QPushButton("Pick new location");
    //buttonTools["pickNewEventWagonTool"] = new QPushButton("Pick new Car");
    QMapIterator<QString, QPushButton*> i(buttonTools);
    while (i.hasNext()) {
        i.next();
        i.value()->setCheckable(true);
    }
    
    cSoundType.addItem("Everywhere", QString("Everywhere"));
    cSoundType.addItem("Cab", QString("Cab"));
    cSoundType.addItem("Pass", QString("Pass"));
    cSoundType.addItem("Ground", QString("Ground"));
    
    
    cActionType.setStyleSheet("combobox-popup: 0;");
    cActionType.setMaxVisibleItems(30);
    cActionType.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    cOutcome.setStyleSheet("combobox-popup: 0;");
    cOutcome.setMaxVisibleItems(30);
    cOutcome.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    cOutcomeEvent.setStyleSheet("combobox-popup: 0;");
    cOutcomeEvent.setMaxVisibleItems(30);
    cOutcomeEvent.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    eOutcomeMessage.setFixedHeight(100);
    //eOutcomeMessage.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    outcomeList.setFixedHeight(100);
    outcomeList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    //vbox->setAlignment(Qt::AlignTop);
    
    // action event
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    int row = 0;
    QLabel *label = new QLabel("Action:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    label = new QLabel("Type:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    cActionType.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    vlist->addWidget(&cActionType, row++, 1);
    QObject::connect(&cActionType, SIGNAL(activated(QString)),
                      this, SLOT(cActionTypeSelected(QString)));
    //vlist->addWidget(new QLabel("Info:"), row, 0);
    //vlist->addWidget(&eActionInfo, row++, 1);
    actionWidget.setLayout(vlist);
    
    // action station
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    row = 0;
    label = new QLabel("Station:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    cStationStopAction.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    cStationStopAction.setStyleSheet("combobox-popup: 0;");
    cStationStopAction.setMaxVisibleItems(30);
    cStationStopAction.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vlist->addWidget(&cStationStopAction, row++, 1);
    actionWidgetStation.setLayout(vlist);
    
    // action siding
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    row = 0;
    label = new QLabel("Siding:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eActionSiding, row, 1);
    eActionSiding.setDisabled(true);
    bActionSiding.setText("Link Selected");
    QObject::connect(&bActionSiding, SIGNAL(released()),
                      this, SLOT(bActionSidingSelected()));
    vlist->addWidget(&bActionSiding, row++, 2);
    actionWidgetSiding.setLayout(vlist);
    
    // action speed
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    row = 0;
    label = new QLabel("Speed:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    eActionSpeed.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    vlist->addWidget(&eActionSpeed, row++, 1);
    eActionSpeed.setRange(0,1000);
    QObject::connect(&eActionSpeed, SIGNAL(editingFinished()),
                      this, SLOT(eActionSpeedSelected()));
    actionWidgetSpeed.setLayout(vlist);
    
    // action wagon list
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(1,0,1,0);
    QPushButton *bRemoveCar = new QPushButton("Remove Selected");
    QObject::connect(bRemoveCar, SIGNAL(released()),
                      this, SLOT(bRemoveCarSelected()));
    QPushButton *bJumpToCar = new QPushButton("Jump To Selected");
    QObject::connect(bJumpToCar, SIGNAL(released()),
                      this, SLOT(bJumpToCarSelected()));
    QPushButton *bDescCar = new QPushButton("Edit description");
    QObject::connect(bDescCar, SIGNAL(released()),
                      this, SLOT(bDescCarSelected()));
    bJumpToCar->setMinimumWidth(100);
    label = new QLabel("Wagon List:");
    label->setMaximumHeight(25);
    vlist->addWidget(label, 0, 0);
    vlist->addWidget(&wagonList, 0, 1, 5, 1);
    QPushButton *pickNewEventWagon = new QPushButton("Pick Selected");
    vlist->addWidget(pickNewEventWagon, 1, 0);
    QObject::connect(pickNewEventWagon, SIGNAL(released()),
                      this, SLOT(bPickNewEventWagonToolSelected()));
    vlist->addWidget(bJumpToCar, 2, 0);
    vlist->addWidget(bRemoveCar, 3, 0);
    vlist->addWidget(bDescCar, 4, 0);
    QStringList list;
    list.append("ID:");
    list.append("Description:");
    wagonList.setColumnCount(2);
    wagonList.setHeaderLabels(list);
    //wagonList.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    actionWidgetWagonList.setLayout(vlist);
    
    // location event
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    row = 0;
    label = new QLabel("Location:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);

    label = new QLabel("Position:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eLocationPosition, row++, 1, 1, 2);
    QPushButton *button = new QPushButton("Jump to location");
    QObject::connect(button, SIGNAL(released()),
                      this, SLOT(bJumpToEventLocationSelected()));
    vlist->addWidget(button, row, 1);
    QObject::connect(buttonTools["pickNewEventLocationTool"], SIGNAL(toggled(bool)),
                      this, SLOT(bPickEventLocationSelected(bool)));
    vlist->addWidget(buttonTools["pickNewEventLocationTool"], row++, 2);
    vlist->addWidget(new QLabel("Radius:"), row, 0);
    vlist->addWidget(&eLocationRadius, row++, 1, 1, 2);
    eLocationRadius.setRange(0,100);
    QObject::connect(&eLocationRadius, SIGNAL(editingFinished()),
                      this, SLOT(eLocationRadiusSelected()));
    label = new QLabel("Train must stop:");
    label->setMinimumHeight(25);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&cLocationStop, row++, 1, 1, 2);
    QObject::connect(&cLocationStop, SIGNAL(stateChanged(int)),
                      this, SLOT(cLocationStopSelected(int)));
    
    locationWidget.setLayout(vlist);
    
    // time event
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    row = 0;
    label = new QLabel("Time:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    label = new QLabel("Activation Time:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eTime, row++, 1);
    QObject::connect(&eTime, SIGNAL(timeChanged(QTime)),
                      this, SLOT(eTimeSelected(QTime)));
    eTime.setDisplayFormat("HH:mm:ss");
    eTime.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
    timeWidget.setLayout(vlist);
    vbox->addWidget(&actionWidget);
    vbox->addWidget(&actionWidgetSpeed);
    vbox->addWidget(&actionWidgetStation);
    vbox->addWidget(&actionWidgetSiding);
    vbox->addWidget(&actionWidgetWagonList);
    vbox->addWidget(&locationWidget);
    vbox->addWidget(&timeWidget);
    //
    
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    label = new QLabel("Event:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    label = new QLabel("Name:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eName, row++, 1);
    QObject::connect(&eName, SIGNAL(editingFinished()),
                      this, SLOT(eNameSelected()));
    vlist->addWidget(new QLabel("Activation Level:"), row, 0);
    vlist->addWidget(&eActivationLevel, row++, 1);
    eActivationLevel.setRange(-100,100);
    QObject::connect(&eActivationLevel, SIGNAL(editingFinished()),
                      this, SLOT(eActivationLevelSelected()));
    vlist->addWidget(new QLabel("Triggered Text:"), row, 0);
    vlist->addWidget(&eTriggeredText, row++, 1);
    QObject::connect(&eTriggeredText, SIGNAL(textEdited(QString)),
                      this, SLOT(eTriggeredTextSelected(QString)));
    vlist->addWidget(new QLabel("Untriggered Text:"), row, 0);
    vlist->addWidget(&eUntriggeredText, row++, 1);
    QObject::connect(&eUntriggeredText, SIGNAL(textEdited(QString)),
                      this, SLOT(eUntriggeredTextSelected(QString)));
    vlist->addWidget(new QLabel("Notes:"), row, 0);
    vlist->addWidget(&eNotes, row++, 1);
    QObject::connect(&eNotes, SIGNAL(textEdited(QString)),
                      this, SLOT(eNotesSelected(QString)));
    cAutoContinueLabel.setText("Disable pause:");
    cAutoContinueLabel.setMinimumHeight(22);
    vlist->addWidget(&cAutoContinueLabel, row, 0);
    QObject::connect(&cAutoContinueLabel, SIGNAL(stateChanged(int)),
                      this, SLOT(cAutoContinueLabelSelected(int)));
    vlist->addWidget(&eAutoContinue, row++, 1);
    QObject::connect(&eAutoContinue, SIGNAL(editingFinished()),
                      this, SLOT(eAutoContinueSelected()));
    //lReversable.setText("Reversable:");
    cReversable.setMinimumHeight(22);
    cReversable.setText("Reversable.");
    //vlist->addWidget(&lReversable, row, 0);
    vlist->addWidget(&cReversable, row++, 0);
    QObject::connect(&cReversable, SIGNAL(stateChanged(int)),
                      this, SLOT(cReversableSelected(int)));
    
    label = new QLabel("Outcomes:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    vlist->addWidget(&outcomeList, row++, 0, 1, 2);
    QPushButton *bAddOutcome = new QPushButton("Add New");
    QPushButton *bRemoveOutcome = new QPushButton("Remove Selected");
    QObject::connect(bAddOutcome, SIGNAL(released()),
                      this, SLOT(bAddOutcomeSelected()));
    QObject::connect(bRemoveOutcome, SIGNAL(released()),
                      this, SLOT(bRemoveOutcomeSelected()));
    
    vlist->addWidget(bAddOutcome, row++, 0, 1, 2);
    vlist->addWidget(bRemoveOutcome, row++, 0, 1, 2);
    
    label = new QLabel("Selected Outcome:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    vlist->addWidget(new QLabel("Action:"), row, 0);
    vlist->addWidget(&cOutcome, row++, 1);
    QObject::connect(&cOutcome, SIGNAL(activated(QString)),
                      this, SLOT(outcomeActoionListSelected(QString)));
    vbox->addItem(vlist);

    // Outcome Event
    
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    row = 0;
    label = new QLabel("Event:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    cOutcomeEvent.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    vlist->addWidget(&cOutcomeEvent, row++, 1);
    QObject::connect(&cOutcomeEvent, SIGNAL(activated(QString)),
                      this, SLOT(cOutcomeEventSelected(QString)));
    outcomeProperties[(int)ActivityEvent::Outcome::CategoryEvent] = new QWidget(this);
    outcomeProperties[(int)ActivityEvent::Outcome::CategoryEvent]->setLayout(vlist);
    vbox->addWidget(outcomeProperties[(int)ActivityEvent::Outcome::CategoryEvent]);
    
    // Outcome info
    
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    row = 0;
    vlist->addWidget(new QLabel("Message:"), row++, 0, 1, 2);
    vlist->addWidget(&eOutcomeMessage, row++, 0, 1, 2);
    QObject::connect(&eOutcomeMessage, SIGNAL(textChanged()),
                      this, SLOT(eOutcomeMessageSelected()));
    outcomeProperties[(int)ActivityEvent::Outcome::CategoryInfo] = new QWidget(this);
    outcomeProperties[(int)ActivityEvent::Outcome::CategoryInfo]->setLayout(vlist);
    vbox->addWidget(outcomeProperties[(int)ActivityEvent::Outcome::CategoryInfo]);
    
    // Outcome Sound File
    
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    row = 0;
    label = new QLabel("Sound File:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    eSoundFileName.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    vlist->addWidget(&eSoundFileName, row++, 1);
    vlist->addWidget(new QLabel("Sound Type:"), row, 0);
    vlist->addWidget(&cSoundType, row++, 1);
    outcomeProperties[(int)ActivityEvent::Outcome::CategorySoundFile] = new QWidget(this);
    outcomeProperties[(int)ActivityEvent::Outcome::CategorySoundFile]->setLayout(vlist);
    vbox->addWidget(outcomeProperties[(int)ActivityEvent::Outcome::CategorySoundFile]);
    
    vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    row = 0;
    label = new QLabel("Weather Change:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    cWeatherChange.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    vlist->addWidget(&cWeatherChange, row++, 1);
    outcomeProperties[(int)ActivityEvent::Outcome::CategoryWeatherChange] = new QWidget(this);
    outcomeProperties[(int)ActivityEvent::Outcome::CategoryWeatherChange]->setLayout(vlist);
    vbox->addWidget(outcomeProperties[(int)ActivityEvent::Outcome::CategoryWeatherChange]);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&outcomeList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(outcomeListSelected(QListWidgetItem*)));
    
    actionWidget.hide();
    actionWidgetSpeed.hide();
    actionWidgetStation.hide();
    actionWidgetSiding.hide();
    actionWidgetWagonList.hide();
    locationWidget.hide();
    timeWidget.hide();
    
    foreach(QWidget *w, outcomeProperties){
        if(w == NULL)
            continue;
        w->hide();
    }
}

ActivityEventProperties::~ActivityEventProperties() {
}

void ActivityEventProperties::showEvent(ActivityEvent *e){
    if(e == NULL){
        event = NULL;
        return;
    }
    
    if(event != NULL)
        event->unselect();
    
    event = e;
    event->select();
    
    actionWidget.hide();
    actionWidgetSpeed.hide();
    actionWidgetStation.hide();
    actionWidgetSiding.hide();
    actionWidgetWagonList.hide();
    locationWidget.hide();
    timeWidget.hide();
    cReversable.hide();
        
    if(event->category == ActivityEvent::CategoryAction){
        actionWidget.show();
        cActionType.setCurrentIndex(cActionType.findData((int)event->eventType));
        cReversable.setChecked(event->reversableEvent);
        cReversable.show();
        
        if(event->eventType == ActivityEvent::EventTypeReachSpeed){
            actionWidgetSpeed.show();
            eActionSpeed.setValue(event->speed);
        }
        if(event->eventType == ActivityEvent::EventTypePickupPassengers){
            actionWidgetStation.show();
            cStationStopAction.setCurrentIndex(event->stationStop);
        }
        if((event->eventType == ActivityEvent::EventTypeAssembleTrainAtLocation)
                || (event->eventType == ActivityEvent::EventTypeDropoffWagonsAtLocation) ){
            actionWidgetSiding.show();
            if(event->sidingItem > 0) {
                eActionSiding.setStyleSheet("color: green");
                eActionSiding.setText(QString("[")+QString::number(event->sidingItem)+"] "+event->getSidingDescription());
            } else {
                eActionSiding.setStyleSheet("color: red");
                eActionSiding.setText("Not Linked.");
            }
        }
        if((event->eventType == ActivityEvent::EventTypeAssembleTrain)
                || (event->eventType == ActivityEvent::EventTypePickupWagons)
                || (event->eventType == ActivityEvent::EventTypeDropoffWagonsAtLocation)
                || (event->eventType == ActivityEvent::EventTypeAssembleTrainAtLocation) ){
            actionWidgetWagonList.show();
            wagonList.clear();
            QStringList list;
            QList<QTreeWidgetItem *> items;
            for(int i = 0; i < event->getWagonListSize(); i++ ){
                list.clear();
                list.append(event->getWagonListIdDescription(i));
                list.append(event->getWagonListDescription(i));
                QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, i );
                //item->setCheckState(0, Qt::Unchecked);
                items.append(item);
            }  
            wagonList.insertTopLevelItems(0, items);
        }
    }
    if(event->category == ActivityEvent::CategoryLocation){
        locationWidget.show();
        if(event->location != NULL){
            eLocationRadius.setValue(event->location[4]);
            cLocationStop.blockSignals(true);
            if(event->triggerOnStop != 1){
                cLocationStop.setChecked(false);
            } else {
                cLocationStop.setChecked(true);
            }
            cLocationStop.blockSignals(false);
                
            eLocationPosition.setText(QString::number(event->location[0]) +" "+ QString::number(event->location[1]) +" "+ QString::number(event->location[2]) +" "+ QString::number(event->location[3]));
        }
    }
    if(event->category == ActivityEvent::CategoryTime){
        timeWidget.show();
        //QTime time(0, 0);
        eTime.blockSignals(true);
        eTime.setTime(QTime::fromMSecsSinceStartOfDay(event->time*1000));
        eTime.blockSignals(false);
    }
    
    eName.setText(event->name);
    eName.setCursorPosition(0);
    eActivationLevel.setValue(event->activationLevel);
    eTriggeredText.setText(event->textToDisplayOnCompletionIfTriggered);
    eTriggeredText.setCursorPosition(0);
    eUntriggeredText.setText(event->textToDisplayOnCompletionIfNotTriggered);
    eUntriggeredText.setCursorPosition(0);
    eNotes.setText(event->textToDisplayDescriptionOfTask);
    eNotes.setCursorPosition(0);
    cAutoContinueLabel.blockSignals(true);
    if(event->ortsContinue != -99999){
        cAutoContinueLabel.setChecked(true);
        eAutoContinue.show();
        eAutoContinue.setValue(event->ortsContinue);
    } else {
        cAutoContinueLabel.setChecked(false);
        eAutoContinue.hide();
    }
    cAutoContinueLabel.blockSignals(false);

    
    outcomeList.clear();
    for(int i = 0; i < event->outcomes.size(); i++){
       new QListWidgetItem ( ActivityEvent::Outcome::OutcomeTypeDescription[event->outcomes[i]->type], &outcomeList, i );
    }
    
    if(outcomeList.count() > 0){
        outcomeList.item(0)->setSelected(true);
        outcomeList.setCurrentRow(0);
        outcomeListSelected(outcomeList.item(0));
    }
}

void ActivityEventProperties::bActionSidingSelected(){
    if(event == NULL)
        return;
    bool ok = event->setSidingFromSelected();
    if(!ok){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Siding not linked!");
        msgBox.setText("Select siding before using this button.");
        msgBox.exec();
        return;
    }
    int id = outcomeList.currentRow();
    this->showEvent(event);   
    selctOutcomeOnList(id);
}

void ActivityEventProperties::bAddOutcomeSelected(){
    if(event == NULL)
        return;
    int id = event->newOutcome();
    this->showEvent(event);   
    selctOutcomeOnList(id);
}

void ActivityEventProperties::bRemoveOutcomeSelected(){
    if(event == NULL)
        return;
    int id = outcomeList.currentRow();
    if(id < 0)
        return;
    event->removeOutcome(id);
    this->showEvent(event);
    id--;
    if(id < 0)
        id = 0;
    selctOutcomeOnList(id);
}

void ActivityEventProperties::outcomeListSelected(QListWidgetItem* item){
    if(event == NULL)
        return;
    outcome = event->outcomes[item->type()];
    if(outcome == NULL)
        return;
    
    foreach(QWidget *w, outcomeProperties){
        if(w == NULL)
            continue;
        w->hide();
    }
        
    cOutcome.setCurrentIndex(cOutcome.findData((int)outcome->type));
    
    if(outcomeProperties[(int)outcome->category] == NULL)
        return;
    outcomeProperties[(int)outcome->category]->show();
    
    if(outcome->category == ActivityEvent::Outcome::CategoryInfo){
        QString txt = outcome->value.toString();
        txt.replace("\\n","\n");
        eOutcomeMessage.blockSignals(true);
        eOutcomeMessage.setPlainText(txt);
        eOutcomeMessage.blockSignals(false);
    }
    
    if(outcome->category == ActivityEvent::Outcome::CategoryEvent){
        int index = this->cOutcomeEvent.findData((int)outcome->value.toInt());
        cOutcomeEvent.setCurrentIndex(index);
    }
    
    if(outcome->category == ActivityEvent::Outcome::CategorySoundFile){
        eSoundFileName.setText(outcome->value.toStringList()[0]);
        cSoundType.setCurrentIndex(cSoundType.findData(outcome->value.toStringList()[1]));
    }
    
    if(outcome->category == ActivityEvent::Outcome::CategoryWeatherChange){
        cWeatherChange.setText(outcome->value.toString());
        cWeatherChange.setText("Not supported yet!");
    }
}

void ActivityEventProperties::cActionTypeSelected(QString item){
    if(event == NULL)
        return;

    event->setActionToNewType((ActivityEvent::EventType)cActionType.currentData().toInt());
    int id = outcomeList.currentRow();
    showEvent(event);
    selctOutcomeOnList(id);
}

void ActivityEventProperties::outcomeActoionListSelected(QString item){
    if(outcome == NULL)
        return;

    outcome->setToNewType((ActivityEvent::Outcome::OutcomeType)cOutcome.currentData().toInt());
    
    int id = outcomeList.currentRow();
    showEvent(event);
    selctOutcomeOnList(id);
}

void ActivityEventProperties::cOutcomeEventSelected(QString val){
    if(outcome == NULL)
        return;
    outcome->setEventLinkId(cOutcomeEvent.currentData().toInt());
}

void ActivityEventProperties::setEventList(QMap<int, QString> eventNames){
    cOutcomeEvent.clear();
    QMapIterator<int, QString> i1(eventNames);
    while (i1.hasNext()) {
        i1.next();
        //qDebug() << "item"<< i1.key() << i1.value();
        cOutcomeEvent.addItem(i1.value(), i1.key());
    }
}

void ActivityEventProperties::setStationStopList(QMap<int, QString> eventNames){
    cStationStopAction.clear();
    QMapIterator<int, QString> i1(eventNames);
    while (i1.hasNext()) {
        i1.next();
        //qDebug() << "item"<< i1.key() << i1.value();
        cStationStopAction.addItem(i1.value(), i1.key());
    }
}

void ActivityEventProperties::selctOutcomeOnList(int id){
    if(id < 0)
        return;
    if(outcomeList.count() > id){
        outcomeList.item(id)->setSelected(true);
        outcomeList.setCurrentRow(id);
        outcomeListSelected(outcomeList.item(id));
    }
}

void ActivityEventProperties::eOutcomeMessageSelected(){
    if(outcome == NULL)
        return;
    outcome->setMessage(eOutcomeMessage.toPlainText());
}

void ActivityEventProperties::eActivationLevelSelected(){
    if(event == NULL)
        return;
    event->setActivationLevel(eActivationLevel.value());
}

void ActivityEventProperties::eNameSelected(){
    if(event == NULL)
        return;
    if(eName.text() == event->name)
        return;
    event->setName(eName.text());
    emit eventNameChanged(event->id);
}

void ActivityEventProperties::eTriggeredTextSelected(QString val){
    if(event == NULL)
        return;
    event->setTriggeredText(val);
}

void ActivityEventProperties::eUntriggeredTextSelected(QString val){
    if(event == NULL)
        return;
    event->setUntriggeredText(val);
}

void ActivityEventProperties::eActionSpeedSelected(){
    if(event == NULL)
        return;
    
    event->setSpeed(eActionSpeed.value());
}

void ActivityEventProperties::eNotesSelected(QString val){
    if(event == NULL)
        return;
    event->setNotes(val);
}

void ActivityEventProperties::eLocationRadiusSelected(){
    if(event == NULL)
        return;
    event->setLocationRadius(eLocationRadius.value());
}

void ActivityEventProperties::cLocationStopSelected(int val){
    if(event == NULL)
        return;
    if(val == Qt::Checked)
        event->setLocationStop(true);
    else
        event->setLocationStop(false);
}

void ActivityEventProperties::cReversableSelected(int val){
    if(event == NULL)
        return;
    if(val == Qt::Checked)
        event->setReversable(true);
    else
        event->setReversable(false);
}

void ActivityEventProperties::eAutoContinueSelected(){
    if(event == NULL)
        return;
    event->setAutoContinue(eAutoContinue.value());
}

void ActivityEventProperties::cAutoContinueLabelSelected(int val){
    if(event == NULL)
        return;
    if(val == Qt::Checked){
        event->setAutoContinue(0);
        eAutoContinue.show();
        eAutoContinue.setValue(0);
    } else { 
        event->setAutoContinue(-99999);
        eAutoContinue.hide();
    }
}

void ActivityEventProperties::eTimeSelected(QTime val){
    if(event == NULL)
        return;
    event->setTime(val.msecsSinceStartOfDay()/1000);
}

void ActivityEventProperties::bJumpToEventLocationSelected(){
    if(event == NULL)
        return;
    if(coordinate == NULL)
        coordinate = new PreciseTileCoordinate();
    
    coordinate->TileX = event->location[0];
    coordinate->TileZ = event->location[1];
    coordinate->setWxyz(event->location[2], 0, event->location[3]);
    
    emit jumpTo(coordinate);
}

void ActivityEventProperties::bPickEventLocationSelected(bool val){
    if(event == NULL)
        return;
    if(val)
        emit enableTool("pickNewEventLocationTool");
    else
        emit enableTool("");
}

void ActivityEventProperties::bPickNewEventWagonToolSelected(){
    if(event == NULL)
        return;
    event->addSelectedWagonToList();
    
    int id = outcomeList.currentRow();
    showEvent(event);
    selctOutcomeOnList(id);
}

void ActivityEventProperties::msg(QString text, QString val){
    if(text == "toolEnabled"){
        QMapIterator<QString, QPushButton*> i(buttonTools);
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(true);
            i.value()->setChecked(false);
        }
        if(buttonTools[val] != NULL)
            buttonTools[val]->setChecked(true);
        i.toFront();
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(false);
        }
    }
}

void ActivityEventProperties::bJumpToCarSelected(){
    if(event == NULL)
        return;
    float position[5];
    bool ok = event->getWagonListItemPosition(wagonList.currentItem()->type(), position);
    if(!ok)
        return;
    
    if(coordinate == NULL)
        coordinate = new PreciseTileCoordinate();
    
    coordinate->TileX = position[0];
    coordinate->TileZ = position[1];
    coordinate->setWxyz(position[2], 0, position[4]);
    
    emit jumpTo(coordinate);
}

void ActivityEventProperties::bRemoveCarSelected(){
    if(event == NULL)
        return;
    event->removeWagonListItem(wagonList.currentItem()->type());
    
    int id = outcomeList.currentRow();
    showEvent(event);
    selctOutcomeOnList(id);
}

void ActivityEventProperties::bDescCarSelected(){
    if(event == NULL)
        return;
    EditFileNameDialog dialog;
    dialog.setWindowTitle("Wagon Item Description.");
    qDebug() << wagonList.currentItem()->type();
    dialog.name.setText(event->getWagonListItemDescription(wagonList.currentItem()->type()));
    dialog.exec();
    if(dialog.isOk)
        event->setWagonListItemDescription(wagonList.currentItem()->type(), dialog.name.text());
    
    int id = outcomeList.currentRow();
    showEvent(event);
    selctOutcomeOnList(id);
}