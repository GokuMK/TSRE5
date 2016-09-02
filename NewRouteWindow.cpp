/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "NewRouteWindow.h"

NewRouteWindow::NewRouteWindow() : QDialog(){
    this->setFixedSize(200, 100);

    QPushButton* ok = new QPushButton("OK");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));

    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Name ID:",&this->name);
    vlist->addRow("Lat:",&this->lat);
    vlist->addRow("Lon:",&this->lon);
    vlist->addRow(ok,cancel);
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
}

void NewRouteWindow::cancel(){
    this->changed = false;
    this->close();
}
void NewRouteWindow::ok(){
    this->changed = true;
    this->close();
}

NewRouteWindow::~NewRouteWindow() {
}

