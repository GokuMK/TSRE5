/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ChooseFileDialog.h"

ChooseFileDialog::ChooseFileDialog() : QDialog(){
    //this->setFixedWidth(300);
    items.setFixedWidth(600);
    //this->setFixedSize(300,300);

    //QLabel *label = new QLabel("Save changes in consists?");
    QPushButton* ok = new QPushButton("Edit");
    QPushButton* cancel = new QPushButton("Close");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->addWidget(&infoLabel, 0, 0, 1, 2, Qt::AlignCenter);
    //vlist->addWidget(new QLabel("New FileName:"), 1, 0, Qt::AlignLeft);
    //vlist->addWidget(&name, 1, 1, 1, 1, Qt::AlignLeft);
    //QHBoxLayout *vlist1 = new QHBoxLayout;
    vlist->addWidget(&items, 1, 0, 1, 2, Qt::AlignCenter);
    vlist->addWidget(ok, 2, 0);
    vlist->addWidget(cancel, 2, 1);
    
//    mainLayout->setAlignment(browse, Qt::AlignBottom);
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );
    //this->setFixedSize(this->width(),this->height());
}

void ChooseFileDialog::setMsg(QString msg){
    infoLabel.setText(msg);
}

void ChooseFileDialog::cancel(){
    this->changed = 0;
    this->close();
}
void ChooseFileDialog::ok(){
    this->changed = 1;
    this->close();
}

ChooseFileDialog::~ChooseFileDialog() {
}

