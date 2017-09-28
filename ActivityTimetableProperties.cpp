/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityTimetableProperties.h"

ActivityTimetableProperties::ActivityTimetableProperties(QWidget* parent) : QWidget(parent) {
    setMinimumWidth(300);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);

    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    
    int row = 0;
    QLabel *label = NULL;
    
    QStringList list;
    list.append("Station:");
    list.append("Arrive:");
    list.append("Depart:");
    list.append("Performance:");
    lTimetable.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    lTimetable.setColumnCount(4);
    lTimetable.setHeaderLabels(list);
    lTimetable.setRootIsDecorated(false);
    //lTimetable.header()->resizeSection(0,200);    
    //lTimetable.header()->resizeSection(1,100);    
    vlist->addWidget(&lTimetable, row++, 0, 1, 2);
    QPushButton *bAddOutcome = new QPushButton("Calculate");
    QObject::connect(bAddOutcome, SIGNAL(released()),
                      this, SLOT(bAddOutcomeSelected()));
    
    vlist->addWidget(bAddOutcome, row++, 0, 1, 2);
        vbox->addItem(vlist);

    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&lTimetable, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                      this, SLOT(lServciesSelected(QTreeWidgetItem*, int)));

}

ActivityTimetableProperties::~ActivityTimetableProperties() {
}

