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
#include "Consist.h"
#include "Game.h"
#include "EngLib.h"
#include "Eng.h"
#include <stdlib.h>
#include <QDateTime>

RandomConsist::RandomConsist(QWidget* parent) : QWidget(parent){
    setWindowFlags(Qt::WindowType::Tool);
    items.setFixedWidth(350);
    list.setFixedWidth(240);
    list.setStyleSheet("combobox-popup: 0;");
    name.setFixedWidth(160);
    number.setText("10");
    number.setFixedWidth(240);
    //this->setFixedSize(300,300);
    setWindowTitle(tr("Random Consist"));
    
    QLabel *label = new QLabel("Add items to the List, you want in random consist:");
    QPushButton* ok = new QPushButton("Add Random");
    QPushButton* cancel = new QPushButton("Cancel");
    QPushButton* save = new QPushButton("Save");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));
    connect(save, SIGNAL (released()), this, SLOT (save()));
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->addWidget(label, 0, 0, 1, 3, Qt::AlignCenter);
    vlist->addWidget(&items, 1, 0, 1, 3, Qt::AlignCenter);
    vlist->addWidget(new QLabel("Or load List from disk:"), 2, 0, Qt::AlignLeft);
    vlist->addWidget(&list, 2, 1, 1, 2, Qt::AlignCenter);
    QObject::connect(&list, SIGNAL(activated(QString)),
                      this, SLOT(listSelected(QString)));
    vlist->addWidget(new QLabel("Save current List as:"), 3, 0, Qt::AlignLeft);
    vlist->addWidget(&name, 3, 1, Qt::AlignCenter);
    vlist->addWidget(save, 3, 2, Qt::AlignCenter);
    vlist->addWidget(new QLabel("Number of Units:"), 4, 0, Qt::AlignLeft);
    vlist->addWidget(&number, 4, 1, 1, 2);
    vlist->addWidget(ok, 5, 0, 1, 2);
    vlist->addWidget(cancel, 5, 2);

    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );
    
    loadSets();
}

void RandomConsist::loadSets(){
    list.clear();
    
    QString path;
    path = Game::root + "/trains/consists/randomsets/";
    QDir dir(path);
    dir.setFilter(QDir::Files);
    dir.setNameFilters(QStringList()<<"*.con");
    qDebug() << path;
    if(!dir.exists())
        qDebug() << "not exist";
    qDebug() << dir.count() <<" con files";
    foreach(QString engfile, dir.entryList()){
        Consist *c = new Consist(path, engfile);
        list.addItem(c->name, sets.size());
        sets.push_back(c);
    }
}

void RandomConsist::listSelected(QString val){
    items.clear();
    Consist *c = sets[list.currentData().toInt()];
    if(c == NULL)
        return;
    for(int i = 0; i < c->engItems.size(); i++)
        new QListWidgetItem ( Game::currentEngLib->eng[c->engItems[i].eng]->displayName, &items, c->engItems[i].eng);
}

void RandomConsist::cancel(){
    this->changed = 0;
    items.clear();
    this->close();
}

void RandomConsist::save(){
    if(name.text().size() < 1)
        return;
    if(items.count() < 1)
        return;
    Consist *c = new Consist();
    c->path += "randomsets/";
    c->name = name.text()+".con";
    
    QDir dir(c->path);
    if(!dir.exists())
        dir.mkpath(".");
    
    for(int i = 0; i < items.count(); i++){
        c->appendEngItem(items.item(i)->type());
    }
    
    c->save();
    list.addItem(c->name, sets.size());
    sets.push_back(c);
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

