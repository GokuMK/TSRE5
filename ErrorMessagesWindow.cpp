/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ErrorMessagesWindow.h"
#include "ErrorMessagesLib.h"
#include <QDebug>
#include "Game.h"
#include "ErrorMessage.h"
#include "ErrorMessageProperties.h"
#include "GeoCoordinates.h"

ErrorMessagesWindow::ErrorMessagesWindow(QWidget* parent) : QWidget(parent) {
    brushes[(int)ErrorMessage::Type_Error] = QBrush(QColor(Game::StyleRedText));
    brushes[(int)ErrorMessage::Type_Warning] = QBrush(QColor(200,200,0));
    brushes[(int)ErrorMessage::Type_Info] = QBrush(QColor(Game::StyleGreenText));
    brushes[1000] = QBrush(QColor(Game::StyleMainLabel));
    this->setWindowFlags(Qt::WindowType::Tool);
    //this->setFixedWidth(350);
    this->setMinimumWidth(730);
    this->setFixedHeight(400);
    this->setWindowTitle(tr("Errors & Messages"));
    
    properties = new ErrorMessageProperties(this);
    
    QVBoxLayout *errorListLayout = new QVBoxLayout;
    errorListLayout->setContentsMargins(0,0,0,0);
    errorListLayout->setSpacing(0);
    /*QPushButton *bNewActionEvent = new QPushButton("New Service");
    QObject::connect(bNewActionEvent, SIGNAL(released()),
                      this, SLOT(bNewServiceSelected()));
    QPushButton *bDeleteActionEvent = new QPushButton("Delete");
    QObject::connect(bDeleteActionEvent, SIGNAL(released()),
                      this, SLOT(bDeleteServiceSelected()));*/
    errorListLayout->addWidget(&errorList);
    errorListLayout->addWidget(properties);
    //errorListLayout->addWidget(bNewActionEvent);
    //errorListLayout->addWidget(bDeleteActionEvent);
    QStringList list;
    list.append("ID:");
    list.append("Time:");
    list.append("Type:");
    list.append("Source:");
    list.append("Message:");
    //list.append("Any:");
    //errorList.setFixedWidth(250);
    errorList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    errorList.setColumnCount(3);
    errorList.setHeaderLabels(list);
    errorList.setRootIsDecorated(false);
    errorList.header()->resizeSection(0,30);    
    errorList.header()->resizeSection(1,50);    
    errorList.header()->resizeSection(2,70);    
    errorList.header()->resizeSection(3,50);    
    errorList.header()->resizeSection(4,500);    
    //QHBoxLayout *v = new QHBoxLayout;
    //v->setSpacing(2);
    //v->setContentsMargins(1,1,1,1);
    //v->addItem(errorListLayout);
    //v->addWidget(serviceProperties);
    this->setLayout(errorListLayout);
    
    QObject::connect(&errorList, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                      this, SLOT(errorListSelected(QTreeWidgetItem*, int)));
    QObject::connect(properties, SIGNAL(jumpTo(PreciseTileCoordinate*)),
                      this, SLOT(jumpRequestReceived(PreciseTileCoordinate*)));
    QObject::connect(properties, SIGNAL(selectObject(GameObj*)),
                      this, SLOT(selectRequestReceived(GameObj*)));
    refreshErrorList();
}

void ErrorMessagesWindow::selectRequestReceived(GameObj* o){
    emit selectObject(o);
}

void ErrorMessagesWindow::jumpRequestReceived(PreciseTileCoordinate* c){
    emit jumpTo(c);
}

void ErrorMessagesWindow::errorListSelected(QTreeWidgetItem* item, int column){
    properties->showMessage(ErrorMessagesLib::ErrorMessages[item->type()]);
}

void ErrorMessagesWindow::refreshErrorList(){
    errorList.clear();
    QList<QTreeWidgetItem *> items;
    QStringList list;
    for(int i = ErrorMessagesLib::ErrorMessages.size() - 1; i >= 0 ; i-- ){
        if(ErrorMessagesLib::ErrorMessages[i] == NULL)
            continue;
        
        ErrorMessage *msg = ErrorMessagesLib::ErrorMessages[i];
        list.clear();
        
       //QTime time = QDateTime::fromMSecsSinceEpoch(msg->time).toString("HH:mm:ss");
        //qDebug() << msg->time << time.isValid()<< time.toString();
        list.append(QString::number(i));
        list.append(QDateTime::fromMSecsSinceEpoch(msg->time).toString("HH:mm:ss"));
        list.append(ErrorMessage::TypeNames[msg->type]);
        list.append(ErrorMessage::SourceNames[msg->source]);
        list.append(msg->description);
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, i );
        //item->setCheckState(0, Qt::Unchecked);
        //item->setCheckState(1, Qt::Unchecked);
        //item->setCheckState(2, Qt::Unchecked);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        item->setForeground(2, brushes[(int)msg->type]);
        item->setForeground(3, brushes[1000]);
        item->setTextAlignment(1, Qt::AlignCenter);
        item->setTextAlignment(2, Qt::AlignCenter);
        item->setTextAlignment(3, Qt::AlignCenter);
        items.append(item);
    }
    errorList.insertTopLevelItems(0, items);
}

void ErrorMessagesWindow::show(){
     refreshErrorList();
     QWidget::show();
}

ErrorMessagesWindow::~ErrorMessagesWindow() {
}

void ErrorMessagesWindow::hideEvent(QHideEvent *e){
    emit windowClosed();
}
