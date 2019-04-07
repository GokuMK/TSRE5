/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ShapeViewerNavigatorWidget.h"
#include "ContentHierarchyInfo.h"
#include "Game.h"

ShapeViewerNavigatorWidget::ShapeViewerNavigatorWidget(QWidget* parent) : QWidget(parent){
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QLabel *label = new QLabel("Directory Files:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&searchFiles);
    vbox->addWidget(&dirFiles);
    
    label = new QLabel("Current File Items:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&fileItems);
    this->setMinimumWidth(220);
    this->setLayout(vbox);
    
    QStringList list;
    list.append("");
    fileItems.setHeaderLabels(list);
    dirFiles.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fileItems.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

ShapeViewerNavigatorWidget::~ShapeViewerNavigatorWidget() {
}

void ShapeViewerNavigatorWidget::listHierarchy(QVector<ContentHierarchyInfo*> info){
    fileItems.clear();
    QStringList list;
    QList<QTreeWidgetItem*> items;
    
    int count = -1;
    
    QVector<QTreeWidgetItem*> treeItems;
    QVector<bool> treeItemsAssigned;
    
    for(int i = 0; i < info.size(); i++){
        count++;
        list.clear();
        list.append(info[i]->name);
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, count );
        if(info[i]->distanceLevelId < 0)
            item->setTextColor(0, QColor(Game::StyleMainLabel));
        treeItems.append(item);
    }
    for(int i = 0; i < treeItems.count(); i++){
        int idx = info[i]->parent;
        if(idx < 0 || idx >= treeItems.size())
            continue;
        treeItems[idx]->insertChild(treeItems[idx]->childCount(), treeItems[i]);
        //treeItemsAssigned[i] = true;
        //treeItems[idx].p
    }
    
    fileItems.blockSignals(true);
    for(int i = 0; i < treeItems.count(); i++){
        //treeItems[i]->setExpanded(true);
        if(treeItems[i]->parent() == NULL)
            fileItems.insertTopLevelItem(0, treeItems[i]);
    }
    fileItems.expandAll();
    fileItems.blockSignals(false);
}

void ShapeViewerNavigatorWidget::listDirectoryFiles(QString filepath){
    filepath.replace("\\", "/");
    filepath.replace("//", "/");
    QString dirp = filepath.section("/",0,-2);
    qDebug() << dirp;
    QDir dir(dirp);
    qDebug() << filepath;
    dir.setFilter(QDir::Files);
    
    dirFiles.clear();
    foreach(QString pfile, dir.entryList()){
        if(pfile == "." || pfile == "..")   
            continue;
        if(!pfile.endsWith(".s", Qt::CaseInsensitive) 
                && !pfile.endsWith(".con", Qt::CaseInsensitive) 
                && !pfile.endsWith(".eng", Qt::CaseInsensitive)
                && !pfile.endsWith(".s", Qt::CaseInsensitive))
            continue;
        QListWidgetItem *item = new QListWidgetItem(pfile.section("/", -1,-1), &dirFiles, 0);
        item->setData(0, pfile);
    }
}