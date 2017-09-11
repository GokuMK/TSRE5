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

ActivityEventProperties::ActivityEventProperties(QWidget* parent) : QWidget(parent) {
    this->setMaximumHeight(600);
    this->setMinimumWidth(350);
    this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        
    QMapIterator<ActivityEvent::EventType, QString> i1(ActivityEvent::EventTypeDescription);
    while (i1.hasNext()) {
        i1.next();
        cActionType.addItem(i1.value(), i1.key());
    }

    QMapIterator<ActivityEvent::Outcome::OutcomeType, QString> i2(ActivityEvent::Outcome::OutcomeTypeDescription);
    while (i2.hasNext()) {
        i2.next();
        cOutcome.addItem(i2.value(), i2.key());
    }
    
    eOutcomeMessage.setMinimumHeight(100);
    //eOutcomeMessage.setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    outcomeList.setMinimumHeight(70);
    outcomeList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
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
    vlist->addWidget(&cActionType, row++, 1);
    vlist->addWidget(new QLabel("Info:"), row, 0);
    vlist->addWidget(&eActionInfo, row++, 1);
    
    actionWidget.setLayout(vlist);
    
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
    vlist->addWidget(&eLocationPosition, row++, 1);
    vlist->addWidget(new QPushButton("Pick new location"), row++, 1);
    vlist->addWidget(new QLabel("Radius:"), row, 0);
    vlist->addWidget(&eLocationRadius, row++, 1);
    label = new QLabel("Train must stop:");
    label->setMinimumHeight(25);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&cLocationStop, row++, 1);
    
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
    
    label = new QLabel("Activation time:");
    label->setMinimumWidth(100);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&eTime, row++, 1);
    eTime.setDisplayFormat("HH:mm:ss");
    eTime.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    timeWidget.setLayout(vlist);
    vbox->addWidget(&actionWidget);
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
    vlist->addWidget(new QLabel("ActivationLevel:"), row, 0);
    vlist->addWidget(&eActivationLevel, row++, 1);
    vlist->addWidget(new QLabel("TriggeredText:"), row, 0);
    vlist->addWidget(&eTriggeredText, row++, 1);
    vlist->addWidget(new QLabel("UntriggeredText:"), row, 0);
    vlist->addWidget(&eUntriggeredText, row++, 1);
    vlist->addWidget(new QLabel("Notes:"), row, 0);
    vlist->addWidget(&eNotes, row++, 1);
    label = new QLabel("Reversable:");
    label->setMinimumHeight(25);
    vlist->addWidget(label, row, 0);
    vlist->addWidget(&cReversable, row++, 1);
    
    label = new QLabel("Outcomes:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    vlist->addWidget(&outcomeList, row++, 0, 1, 2);
    QPushButton *bAddOutcome = new QPushButton("Add New");
    QPushButton *bRemoveOutcome = new QPushButton("Remove Selected");
    
    vlist->addWidget(bAddOutcome, row++, 0, 1, 2);
    vlist->addWidget(bRemoveOutcome, row++, 0, 1, 2);
    
    label = new QLabel("Selected Outcome:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    vlist->addWidget(new QLabel("Action:"), row, 0);
    vlist->addWidget(&cOutcome, row++, 1);
    vlist->addWidget(new QLabel("Event:"), row, 0);
    vlist->addWidget(&cOutcomeEvent, row++, 1);
    vlist->addWidget(new QLabel("Message:"), row++, 0, 1, 2);
    vlist->addWidget(&eOutcomeMessage, row++, 0, 1, 2);
    
    vbox->addItem(vlist);
    
    //vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&outcomeList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(outcomeListSelected(QListWidgetItem*)));
    
    actionWidget.hide();
    locationWidget.hide();
    timeWidget.hide();
}

ActivityEventProperties::~ActivityEventProperties() {
}

void ActivityEventProperties::showEvent(ActivityEvent *e){
    if(e == NULL)
        return;
    
    event = e;
    
    actionWidget.hide();
    locationWidget.hide();
    timeWidget.hide();
    if(event->category == ActivityEvent::CategoryAction){
        actionWidget.show();
        this->cActionType.setCurrentIndex((int)event->eventType);
    }
    if(event->category == ActivityEvent::CategoryLocation){
        locationWidget.show();
    }
    if(event->category == ActivityEvent::CategoryTime){
        timeWidget.show();
        QTime time(0, 0);
        this->eTime.setTime(time.addSecs(event->time));
    }
    
    this->eName.setText(event->name);
    this->eActivationLevel.setText(QString::number(event->activationLevel));
    this->eTriggeredText.setText(event->textToDisplayOnCompletionIfTriggered);
    this->eUntriggeredText.setText(event->textToDisplayOnCompletionIfNotTriggered);
    this->eNotes.setText(event->textToDisplayDescriptionOfTask);
    
    this->outcomeList.clear();
    for(int i = 0; i < event->outcomes.size(); i++){
       new QListWidgetItem ( ActivityEvent::Outcome::OutcomeTypeDescription[event->outcomes[i]->type], &outcomeList, i );
    }
    
    if(outcomeList.count() > 0){
        outcomeList.item(0)->setSelected(true);
        outcomeListSelected(outcomeList.item(0));
    }
}

void ActivityEventProperties::outcomeListSelected(QListWidgetItem* item){
    ActivityEvent::Outcome *outcome = event->outcomes[item->type()];
    if(outcome == NULL)
        return;
    
    this->cOutcome.setCurrentIndex((int)outcome->type);
    
    if(outcome->category == ActivityEvent::Outcome::CategoryInfo){
        QString txt = outcome->value.toString();
        txt.replace("\\n","\n");
        this->eOutcomeMessage.setPlainText(txt);
    } else {
        this->eOutcomeMessage.clear();
    }
    
    if(outcome->category == ActivityEvent::Outcome::CategoryEvent){
        this->cOutcomeEvent.setDisabled(false);
        int index = this->cOutcomeEvent.findData((int)outcome->value.toInt());
        this->cOutcomeEvent.setCurrentIndex(index);
    } else {
        this->cOutcomeEvent.setCurrentIndex(-1);
        this->cOutcomeEvent.setDisabled(true);
    }
    
}

void ActivityEventProperties::setEventList(QMap<int, QString> eventNames){
    this->cOutcomeEvent.clear();
    QMapIterator<int, QString> i1(eventNames);
    while (i1.hasNext()) {
        i1.next();
        //qDebug() << "item"<< i1.key() << i1.value();
        cOutcomeEvent.addItem(i1.value(), i1.key());
    }
}