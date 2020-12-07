/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ClientUsersWindow.h"
#include "RouteEditorServer.h"
#include <QDebug>
#include "Game.h"
#include "RouteEditorClient.h"
#include "ClientInfo.h"


ClientUsersWindow::ClientUsersWindow(QWidget* parent) : QWidget(parent) {
    brushes[0] = QBrush(QColor(Game::StyleRedText));
    brushes[1] = QBrush(QColor(200,200,0));
    brushes[2] = QBrush(QColor(Game::StyleGreenText));
    brushes[3] = QBrush(QColor(20,20,200));
    brushes[1000] = QBrush(QColor(Game::StyleMainLabel));
    this->setWindowFlags(Qt::WindowType::Tool);
    //this->setFixedWidth(350);
    this->setMinimumWidth(730);
    this->setFixedHeight(150);
    this->setWindowTitle(tr("Users:"));
    
    QVBoxLayout *errorListLayout = new QVBoxLayout;
    errorListLayout->setContentsMargins(0,0,0,0);
    errorListLayout->setSpacing(0);

    errorListLayout->addWidget(&usersList);

    QStringList list;
    list.append("User:");
    list.append("Position:");
    list.append("Last Action:");

    usersList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    usersList.setColumnCount(3);
    usersList.setHeaderLabels(list);
    usersList.setRootIsDecorated(false);
    usersList.header()->resizeSection(0,100);    
    usersList.header()->resizeSection(1,100);    
    usersList.header()->resizeSection(2,300);    

    this->setLayout(errorListLayout);

    //refreshErrorList();
}

ClientUsersWindow::~ClientUsersWindow() {
}


void ClientUsersWindow::refreshList(){
    usersList.clear();
    if(Game::serverClient == NULL)
        return;
    QList<QTreeWidgetItem *> items;
    QStringList list;
    
    foreach(ClientInfo *c, Game::serverClient->clientUsersList){
        if(c == NULL)
            continue;
        
        list.clear();
        list.append(c->username);
        list.append(QString::number(c->X)+" "+QString::number(c->Z));
        list.append(c->lastAction);
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, 0 );
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setForeground(0, brushes[2]);
        item->setForeground(1, brushes[1]);
        //item->setTextAlignment(1, Qt::AlignCenter);
        //item->setTextAlignment(2, Qt::AlignCenter);
        items.append(item);
    }
    usersList.insertTopLevelItems(0, items);
}

void ClientUsersWindow::show(){
    //refreshErrorList();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&ClientUsersWindow::refreshList));
    timer->start(1000);
    QWidget::show();
}

void ClientUsersWindow::hideEvent(QHideEvent *e){
    emit windowClosed();
}


