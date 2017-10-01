/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityTimetableWindow.h"
#include "ActivityTimetableProperties.h"
#include "Activity.h"

ActivityTimetableWindow::ActivityTimetableWindow(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::WindowType::Tool);
    setWindowTitle(tr("Timetable"));
    
    timetableProperties = new ActivityTimetableProperties(this);
    
    QVBoxLayout *actionListLayout = new QVBoxLayout;
    actionListLayout->setContentsMargins(0,0,0,0);
    actionListLayout->setSpacing(0);
    actionListLayout->addWidget(&list);
    list.setFixedWidth(170);
    QHBoxLayout *v = new QHBoxLayout;
    v->setSpacing(2);
    v->setContentsMargins(1,1,1,1);
    v->addItem(actionListLayout);
    v->addWidget(timetableProperties);
    this->setLayout(v);
    
    QObject::connect(&list, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(listSelected(QListWidgetItem*)));
}


ActivityTimetableWindow::~ActivityTimetableWindow() {
}

void ActivityTimetableWindow::showTimetable(QVector<ActivityServiceDefinition*> s){
    if(this->isHidden())
        return;
    services = s;
    //serviceProperties->setPaths(route->path);
    //activity = r->s
    
    list.clear();
    //QList<QTreeWidgetItem *> items;
    //QStringList list;
    for(int i = 0; i < services.size(); i++ ){
        if(services[i] == NULL)
            continue;
        QListWidgetItem *item = new QListWidgetItem ( services[i]->name, &list, i );
        if(services[i]->player)
            item->setCheckState(Qt::Checked);
        else 
            item->setCheckState(Qt::Unchecked);
    }
}

void ActivityTimetableWindow::listSelected(QListWidgetItem *item){
    //if(route == NULL)
    //    return;
    timetableProperties->showTimetable(services[item->type()]);
}
