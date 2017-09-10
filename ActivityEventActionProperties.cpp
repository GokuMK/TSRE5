/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityEventActionProperties.h"
#include "Game.h"
#include "ActivityEvent.h"

ActivityEventActionProperties::ActivityEventActionProperties(QWidget* parent) : QWidget(parent) {
    this->setMaximumHeight(500);
    this->setMinimumWidth(350);
    eOutcomeMessage.setMinimumHeight(100);
    outcomeList.setMinimumHeight(70);
    outcomeList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    int row = 0;
    QLabel *label = new QLabel("Action:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    
    vlist->addWidget(new QLabel("Type:"), row, 0);
    vlist->addWidget(&cActionType, row++, 1);
    vlist->addWidget(new QLabel("Info:"), row, 0);
    vlist->addWidget(&eActionInfo, row++, 1);
    
    label = new QLabel("Event:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist->addWidget(label, row++, 0, 1, 2);
    

    vlist->addWidget(new QLabel("Name:"), row, 0);
    vlist->addWidget(&eName, row++, 1);
    vlist->addWidget(new QLabel("ActivationLevel:"), row, 0);
    vlist->addWidget(&eActivationLevel, row++, 1);
    vlist->addWidget(new QLabel("TriggeredText:"), row, 0);
    vlist->addWidget(&eTriggeredText, row++, 1);
    vlist->addWidget(new QLabel("UntriggeredText:"), row, 0);
    vlist->addWidget(&eUntriggeredText, row++, 1);
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
    vlist->addWidget(&eOutcomeEvent, row++, 1);
    vlist->addWidget(new QLabel("Message:"), row++, 0, 1, 2);
    vlist->addWidget(&eOutcomeMessage, row++, 0, 1, 2);
    
    
    vbox->addItem(vlist);
   
    vbox->addStretch(1);
    this->setLayout(vbox);
}

ActivityEventActionProperties::~ActivityEventActionProperties() {
}

void ActivityEventActionProperties::showEvent(ActivityEvent *e){
    if(e == NULL)
        return;
    
    event = e;
    
    this->eName.setText(event->name);
    this->eTriggeredText.setText(event->textToDisplayOnCompletionIfTriggered);
    this->eUntriggeredText.setText(event->textToDisplayOnCompletionIfNotTriggered);
    this->eNotes.setText(event->textToDisplayDescriptionOfTask);
}
