/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#include "ActionChooseDialog.h"

ActionChooseDialog::ActionChooseDialog() : ActionChooseDialog(10) {
    
}

ActionChooseDialog::ActionChooseDialog(int buttonCount) : QDialog(){
    buttonSize = buttonCount;
    //this->setFixedWidth(300);

    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    infoLabel.setFixedWidth(400);
    infoLabel.setMargin(2);
    vlist->addWidget(&infoLabel, 0, 0, 1, 1, Qt::AlignCenter);
    //vlist->addWidget(new QLabel("New FileName:"), 1, 0, Qt::AlignLeft);
    //vlist->addWidget(&name, 1, 1, 1, 1, Qt::AlignLeft);
    //QHBoxLayout *vlist1 = new QHBoxLayout;
    for(int i = 0; i < buttonSize; i++){
        bok[i] = new QPushButton(this);
        bok[i]->setFixedWidth(400);
        bok[i]->hide();
        mapper.setMapping(bok[i], i);
        connect(bok[i], SIGNAL(clicked()), &mapper, SLOT(map()));
        
        vlist->addWidget(bok[i], i+1, 0, 1, 1, Qt::AlignCenter);
    }
    
    connect(&mapper, SIGNAL(mapped(int)), this, SLOT(action(int)));
    
    //this->setT
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void ActionChooseDialog::setInfoText(QString text){
    infoLabel.setText(text);
}

void ActionChooseDialog::pushAction(QString action, QString text){
    if(count >= buttonSize) return;
    actions[count] = action;
    bok[count]->setText(text);
    bok[count]->show();
    count++;
}

void ActionChooseDialog::action(int i){
    actionChoosen = actions[i];
    this->close();
}

ActionChooseDialog::~ActionChooseDialog() {
    
}

