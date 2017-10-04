/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityServiceWindow.h"
#include "ActivityServiceProperties.h"
#include "Game.h"
#include "Activity.h"
#include "Route.h"
#include "Service.h"
#include "ActLib.h"
#include "EditFileNameDialog.h"

ActivityServiceWindow::ActivityServiceWindow(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::WindowType::Tool);
    setWindowTitle(tr("Services"));
    
    serviceProperties = new ActivityServiceProperties(this);
    
    QVBoxLayout *actionListLayout = new QVBoxLayout;
    actionListLayout->setContentsMargins(0,0,0,0);
    actionListLayout->setSpacing(0);
    QPushButton *bNewActionEvent = new QPushButton("New Service");
    QObject::connect(bNewActionEvent, SIGNAL(released()),
                      this, SLOT(bNewServiceSelected()));
    QPushButton *bDeleteActionEvent = new QPushButton("Delete");
    QObject::connect(bDeleteActionEvent, SIGNAL(released()),
                      this, SLOT(bDeleteServiceSelected()));
    actionListLayout->addWidget(&serviceList);
    actionListLayout->addWidget(bNewActionEvent);
    actionListLayout->addWidget(bDeleteActionEvent);
    QStringList list;
    list.append("Name:");
    list.append("This:");
    list.append("Any:");
    serviceList.setFixedWidth(250);
    serviceList.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    serviceList.setColumnCount(3);
    serviceList.setHeaderLabels(list);
    serviceList.setRootIsDecorated(false);
    serviceList.header()->resizeSection(0,170);    
    serviceList.header()->resizeSection(1,30);    
    serviceList.header()->resizeSection(2,30);    
    QHBoxLayout *v = new QHBoxLayout;
    v->setSpacing(2);
    v->setContentsMargins(1,1,1,1);
    v->addItem(actionListLayout);
    v->addWidget(serviceProperties);
    this->setLayout(v);
    
    QObject::connect(&serviceList, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                      this, SLOT(serviceListSelected(QTreeWidgetItem*, int)));
}

ActivityServiceWindow::~ActivityServiceWindow() {
}

void ActivityServiceWindow::showServices(Route* r){
    if(this->isHidden())
        return;
    route = r;
    serviceProperties->setPaths(route->path);
    //activity = r->s
    
    serviceList.clear();
    QList<QTreeWidgetItem *> items;
    QStringList list;
    for(int i = 0; i < ActLib::jestservice; i++ ){
        if(ActLib::Services[i] == NULL)
            continue;
        //new QListWidgetItem ( route->service[i]->displayName, &serviceList, i );
        list.clear();
        list.append(ActLib::Services[i]->displayName);
        list.append("");
        list.append("");
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, i );
        if(i == 0){
            item->setCheckState(0, Qt::Checked);
            item->setCheckState(1, Qt::Checked);
        }else{
            item->setCheckState(0, Qt::Unchecked);
            item->setCheckState(1, Qt::Unchecked);
        }
        item->setCheckState(2, Qt::Checked);
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        items.append(item);
    }
    serviceList.insertTopLevelItems(0, items);
}

void ActivityServiceWindow::serviceListSelected(QTreeWidgetItem * item, int column){
    if(route == NULL)
        return;
    serviceProperties->showService(ActLib::Services[item->type()]);
}

void ActivityServiceWindow::serviceNameChanged(int id){
    
}

void ActivityServiceWindow::bNewServiceSelected(){
    EditFileNameDialog eWindow;
    eWindow.exec();
    if(eWindow.isOk && eWindow.name.text().length() > 0){
        ActLib::AddService(Game::root + "/routes/" + Game::route + "/services/", eWindow.name.text()+".srv", true);
    }
    showServices(route);
    reloadServicesList();
}

void ActivityServiceWindow::bDeleteServiceSelected(){
    
}