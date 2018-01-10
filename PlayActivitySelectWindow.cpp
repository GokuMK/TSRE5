/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "PlayActivitySelectWindow.h"
#include "Route.h"
#include "Activity.h"
#include "ActLib.h"

PlayActivitySelectWindow::PlayActivitySelectWindow() : QDialog(){
    setFixedSize(500, 400);
    setWindowTitle("Choose Activity");
    
    QPushButton* ok = new QPushButton("OK");
    connect(ok, SIGNAL (released()), this, SLOT (close()));

    QVBoxLayout *vlist = new QVBoxLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(1,1,1,1);
    vlist->addWidget(&actList);
    actList.setStyleSheet("combobox-popup: 0;");
    actList.setMaxVisibleItems(35);
    actList.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QObject::connect(&actList, SIGNAL(activated(QString)), this, SLOT(activitySelected(QString)));
    vlist->addWidget(&eDescription);
    vlist->addWidget(ok);
    QObject::connect(ok, SIGNAL(released()), this, SLOT(okButtonEnabled()));
    this->setLayout(vlist);
}

void PlayActivitySelectWindow::setRoute(Route* r){
    route = r;
    if(r == NULL)
        return;
    
    actList.clear();
    foreach(int id, route->activityId){
        actList.addItem(ActLib::Act[id]->header->name, QVariant(id));
    }
    actList.setCurrentIndex(0);
    activitySelected("");
}

void PlayActivitySelectWindow::okButtonEnabled(){
    actId = actList.currentData().toInt();
}

void PlayActivitySelectWindow::activitySelected(QString val){
    int id = actList.currentData().toInt();
    Activity *a = ActLib::Act[id];
    if(a == NULL)
        return;
    QString txt = a->header->description;
    txt.replace("\\n","\n");
    eDescription.setPlainText(txt);
}

PlayActivitySelectWindow::~PlayActivitySelectWindow() {
}

