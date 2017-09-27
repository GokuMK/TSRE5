/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityTrafficWindow.h"
#include "ActivityTrafficProperties.h"

ActivityTrafficWindow::ActivityTrafficWindow(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::WindowType::Tool);
    setWindowTitle(tr("Traffic"));
    
    trafficProperties = new ActivityTrafficProperties(this);
    
    QVBoxLayout *actionListLayout = new QVBoxLayout;
    actionListLayout->setContentsMargins(0,0,0,0);
    actionListLayout->setSpacing(0);
    QPushButton *bNew = new QPushButton("New Traffic");
    QObject::connect(bNew, SIGNAL(released()),
                      this, SLOT(bNewSelected()));
    QPushButton *bDelete = new QPushButton("Delete");
    QObject::connect(bDelete, SIGNAL(released()),
                      this, SLOT(bDeleteSelected()));
    actionListLayout->addWidget(&list);
    actionListLayout->addWidget(bNew);
    actionListLayout->addWidget(bDelete);
    QHBoxLayout *v = new QHBoxLayout;
    v->setSpacing(2);
    v->setContentsMargins(1,1,1,1);
    v->addItem(actionListLayout);
    v->addWidget(trafficProperties);
    this->setLayout(v);
    
    QObject::connect(&list, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(listSelected(QListWidgetItem*)));
}

ActivityTrafficWindow::~ActivityTrafficWindow() {
}

