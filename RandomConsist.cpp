/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "RandomConsist.h"
#include <stdlib.h>
#include <QDateTime>

RandomConsist::RandomConsist() : QWidget(){
    //this->setFixedWidth(300);
    items.setFixedWidth(300);
    number.setText("10");
    number.setFixedWidth(150);
    //this->setFixedSize(300,300);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setWindowTitle(tr("Random Consist"));
    
    QLabel *label = new QLabel("Add items here, you want in random consist:");
    QPushButton* ok = new QPushButton("Add Random");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->addWidget(label, 0, 0, 1, 2, Qt::AlignCenter);
    vlist->addWidget(&items, 1, 0, 1, 2, Qt::AlignCenter);
    vlist->addWidget(new QLabel("Number of Units:"), 2, 0);
    vlist->addWidget(&number, 2, 1);
    vlist->addWidget(ok, 3, 0);
    vlist->addWidget(cancel, 3, 1);

    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );
}

void RandomConsist::cancel(){
    this->changed = 0;
    items.clear();
    this->close();
}
void RandomConsist::ok(){
    this->changed = 1;
    if(items.count() != 0){
        bool ok = false;
        int num = number.text().toInt(&ok);
        if(num < 1) ok = 1;
        if(num > 1000) num = 1000;
        if(ok){
            srand (QDateTime::currentMSecsSinceEpoch());
            for(int i = 0; i < num; i++){
                int id = rand() % items.count();
                emit addToConSelected(items.item(id)->type(), 2, 1);
            }
        }
    }
    items.clear();
    this->close();
}

RandomConsist::~RandomConsist() {
}

