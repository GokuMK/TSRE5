/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "OverwriteDialog.h"

OverwriteDialog::OverwriteDialog() : QDialog(){
    this->setFixedWidth(200);

    label = new QLabel("\nConsist with this file name already exist. Overwrite?\n");
    label->setWordWrap(true);
    QPushButton* ok = new QPushButton("Yes");
    //QPushButton* trynew = new QPushButton("Try New FileName");
    QPushButton* cancel = new QPushButton("No");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));
    //connect(trynew, SIGNAL (released()), this, SLOT (newName()));
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->addWidget(label, 0, 0, 1, 2, Qt::AlignCenter);
    //vlist->addWidget(new QLabel("New FileName:"), 1, 0, Qt::AlignLeft);
    //vlist->addWidget(&name, 1, 1, 1, 1, Qt::AlignLeft);
    QHBoxLayout *vlist1 = new QHBoxLayout;
    vlist->addWidget(ok, 2, 0);
    //vlist->addWidget(trynew, 2, 1);
    vlist->addWidget(cancel, 2, 1);
    
//    mainLayout->setAlignment(browse, Qt::AlignBottom);
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
    
}

void OverwriteDialog::cancel(){
    this->changed = 0;
    this->close();
}
void OverwriteDialog::ok(){
    this->changed = 1;
    this->close();
}
/*
void OverwriteDialog::trynew(){
    this->changed = 2;
    this->close();
}*/

OverwriteDialog::~OverwriteDialog() {
}

