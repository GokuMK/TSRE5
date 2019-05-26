/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ShapeHierarchyWindow.h"
#include "Game.h"
#include "ShapeHierarchyInfo.h"
#include "SFile.h"

ShapeHierarchyWindow::ShapeHierarchyWindow(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::WindowType::Tool);
    setWindowTitle(tr("Shape Hierarchy"));
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    QLabel *label = new QLabel("Hierarchy:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    //vbox->addWidget(label);

    QStringList list;
    //list.append("Show:");
    list.append("Name:");
    list.append("Polys:");
    list.append("Texture:");
    hierarchyList.setColumnCount(3);
    hierarchyList.setHeaderLabels(list);
    hierarchyList.setRootIsDecorated(false);
    //hierarchyList.header()->resizeSection(0,40);
    hierarchyList.header()->resizeSection(0,200);    
    hierarchyList.header()->resizeSection(1,50);    
    hierarchyList.header()->resizeSection(2,150);    
    vbox->addWidget(&hierarchyList);
    QObject::connect(&hierarchyList, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
                      this, SLOT(hierarchyListSelected(QTreeWidgetItem*, int)));

    this->setLayout(vbox);
    this->resize(500,350);
}

ShapeHierarchyWindow::~ShapeHierarchyWindow() {
}

void ShapeHierarchyWindow::hierarchyListSelected(QTreeWidgetItem* item, int id){
    qDebug() << id;
    if(currentShape == NULL)
        return;
    qDebug() << "T"<< item->type();
    //qDebug() << "D"<< item->data(0, 50).toUInt();
    if(item->type() < 0)
        return;
    if(item->checkState(0) == Qt::Checked){
        currentShape->enablePart(item->type());
    } else {
        currentShape->disablePart(item->type());
    }
}

void ShapeHierarchyWindow::clearLists(){
    hierarchyList.clear();
}

void ShapeHierarchyWindow::setHierarchyList(ShapeHierarchyInfo* info){
    hierarchyList.clear();
    QStringList list;
    QList<QTreeWidgetItem*> items;
    
    int count = -1;
    
    QVector<QTreeWidgetItem*> treeItems;
    QVector<bool> treeItemsAssigned;
    
    QHash<int, int> matricesPolys;
    foreach( ShapeHierarchyInfo::ShapePart i, info->parts){
        int idx = i.matrixId;
        while(idx >= 0){
            matricesPolys[idx] += i.polyCount;
            if(idx == info->hierarchy[idx])
                break;
            idx = info->hierarchy[idx];
        }
    }
    
    for(int i = 0; i < info->matrices.size(); i++){
        count++;
        list.clear();
        list.append(info->matrices[i]);
        list.append(QString::number(matricesPolys[i]));
        list.append(QString(""));
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, -1 );
        //item->setCheckState(0, Qt::Unchecked);
        //item->setCheckState(0, Qt::Checked);
        item->setTextColor(0, QColor(Game::StyleMainLabel));
        item->setTextColor(1, QColor(Game::StyleMainLabel));
        treeItems.append(item);
    }
    
    for(int i = 0; i < treeItems.count(); i++){
        int idx = info->hierarchy[i];
        if(idx < 0 || idx >= treeItems.size())
            continue;
        treeItems[idx]->insertChild(treeItems[idx]->childCount(), treeItems[i]);
        //treeItemsAssigned[i] = true;
        //treeItems[idx].p
    }
    foreach( ShapeHierarchyInfo::ShapePart i, info->parts){
        count++;
        list.clear();
        list.append("Shape Part");
        list.append(QString::number(i.polyCount));
        list.append(i.textureName);
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, i.uid );
        //item->setData(0, 50, QVariant::fromValue(i.uid));
        //qDebug() << "D"<< item->data(0, 50).toUInt();
        //qDebug() << item->type()<< "===================";
        if(!i.enabled)
            item->setCheckState(0, Qt::Unchecked);
        else
            item->setCheckState(0, Qt::Checked);
        treeItems[i.matrixId]->insertChild(treeItems[i.matrixId]->childCount(), item);
    }
    
    hierarchyList.blockSignals(true);
    for(int i = 0; i < treeItems.count(); i++){
        //treeItems[i]->setExpanded(true);
        if(treeItems[i]->parent() == NULL)
            hierarchyList.insertTopLevelItem(0, treeItems[i]);
    }
    hierarchyList.expandAll();
    hierarchyList.blockSignals(false);
}