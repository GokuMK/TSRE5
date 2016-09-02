/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ConUnitsWidget.h"
#include "Consist.h"
#include "Eng.h"
#include "EngLib.h"
#include "Game.h"

ConUnitsWidget::ConUnitsWidget() {
    QPushButton *bUp = new QPushButton("Up");
    QPushButton *bDown = new QPushButton("Down");
    QPushButton *bDel = new QPushButton("Delete");
    QPushButton *bFlip = new QPushButton("Flip");
    QHBoxLayout *bbox = new QHBoxLayout;
    bbox->addWidget(bUp);
    bbox->addWidget(bDown);
    bbox->addWidget(bDel);
    bbox->addWidget(bFlip);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Units:",&count);
    vbox->addItem(bbox);
    vbox->addItem(vlist);
    vbox->addWidget(&items);
    //vbox->addStretch(1);
    this->setLayout(vbox);
    this->setMinimumWidth(250);
    QObject::connect(&items, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(itemsSelected(QListWidgetItem*)));
    
    QObject::connect(bDel, SIGNAL(released()),
                      this, SLOT(bDelReleased()));
    QObject::connect(bUp, SIGNAL(released()),
                      this, SLOT(bUpReleased()));
    QObject::connect(bDown, SIGNAL(released()),
                      this, SLOT(bDownReleased()));
    QObject::connect(bFlip, SIGNAL(released()),
                      this, SLOT(bFlipReleased()));
    items.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ConUnitsWidget::~ConUnitsWidget() {
}

void ConUnitsWidget::setCon(Consist* c){
    con = c;
    if(con == NULL) return;
    items.clear();
    count.setText(QString::number(con->engItems.size()));
    Eng * e;
    QString prefix;
    for (int i = 0; i < con->engItems.size(); i++){
        e = englib->eng[con->engItems[i].eng];
        if(e == NULL) continue;
        prefix = "";
        if(e->loaded !=1) {
            prefix += "[BROKEN]";
            if(con->engItems[i].flip) prefix+="[F]";
            new QListWidgetItem ( prefix+" "+con->engItems[i].ename, &items, i);
        } else {
            if(con->engItems[i].flip) prefix+="[F]";
            new QListWidgetItem ( prefix+" "+e->engName, &items, i);
        }
    }
}

void ConUnitsWidget::itemsSelected(QListWidgetItem * item){
    emit selected(item->type());
}

void ConUnitsWidget::bDelReleased(){
    Game::currentEngLib = englib;
    con->deteleSelected();
    emit refreshItem();
}

void ConUnitsWidget::bUpReleased(){
    Game::currentEngLib = englib;
    con->moveLeftSelected();
    emit refreshItem();
}

void ConUnitsWidget::bDownReleased(){
    Game::currentEngLib = englib;
    con->moveRightSelected();
    emit refreshItem();
}

void ConUnitsWidget::bFlipReleased(){
    Game::currentEngLib = englib;
    con->flipSelected();
    emit refreshItem();
}