/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ShapeTexturesWindow.h"
#include "Game.h"
#include "ShapeTextureInfo.h"

ShapeTexturesWindow::ShapeTexturesWindow(QWidget* parent) : QWidget(parent) {
    setWindowFlags(Qt::WindowType::Tool);
    setWindowTitle(tr("Shape Textures"));
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    QLabel *label = new QLabel("Textures:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    //vbox->addWidget(label);

    QStringList list;
    list.append("Show:");
    list.append("Texture Name:");
    list.append("Loaded:");
    list.append("Resolution:");
    list.append("Format:");
    textureList.setColumnCount(5);
    textureList.setHeaderLabels(list);
    textureList.setRootIsDecorated(false);
    textureList.header()->resizeSection(0,40);
    textureList.header()->resizeSection(1,200);    
    textureList.header()->resizeSection(2,50);    
    textureList.header()->resizeSection(3,60);    
    textureList.header()->resizeSection(4,150);    
    vbox->addWidget(&textureList);
    QObject::connect(&textureList, SIGNAL(itemChanged(QTreeWidgetItem*, int)),
                      this, SLOT(textureListSelected(QTreeWidgetItem*, int)));

    this->setLayout(vbox);
    this->resize(500,200);
}

ShapeTexturesWindow::~ShapeTexturesWindow() {
}
void ShapeTexturesWindow::clearLists(){
    textureList.clear();
}

void ShapeTexturesWindow::setTextureList(QHash<int, ShapeTextureInfo*> textureInfoList){
    textureList.clear();
    QStringList list;
    QList<QTreeWidgetItem *> items;
    
    int count = -1;
    foreach(ShapeTextureInfo* i, textureInfoList){
        count++;
        list.clear();
        list.append(QString(""));
        list.append(i->textureName);
        list.append(i->loaded);
        list.append(i->resolution);
        list.append(i->format);
        QTreeWidgetItem *item = new QTreeWidgetItem((QTreeWidget*)0, list, count );
        item->setCheckState(0, Qt::Unchecked);
        item->setCheckState(0, Qt::Checked);
        items.append(item);
    }
    textureList.blockSignals(true);
    textureList.insertTopLevelItems(0, items);
    textureList.blockSignals(false);
}