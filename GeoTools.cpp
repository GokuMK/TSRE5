/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GeoTools.h"
#include "TexLib.h"
#include "Brush.h"
#include "Texture.h"
#include "GuiFunct.h"
#include "TransferObj.h"
#include "Game.h"

GeoTools::GeoTools(QString name)
    : QWidget(){
    
    int row = 0;
    
    buttonTools["mapTileShowTool"] = new QPushButton("Show/H Map", this);
    buttonTools["mapTileLoadTool"] = new QPushButton("Load Map", this);
    buttonTools["heightTileLoadTool"] = new QPushButton("Load Height", this);
    QMapIterator<QString, QPushButton*> i(buttonTools);
    while (i.hasNext()) {
        i.next();
        i.value()->setCheckable(true);
    }
    
    QGridLayout *vlist3 = new QGridLayout;
    vlist3->setSpacing(2);
    vlist3->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist3->addWidget(buttonTools["mapTileShowTool"],row,0);
    vlist3->addWidget(buttonTools["mapTileLoadTool"],row,1);
    vlist3->addWidget(buttonTools["heightTileLoadTool"],row++,2);

    QLabel *label0;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    label0 = new QLabel("World Tile:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addItem(vlist3);

    vbox->addStretch(1);
    this->setLayout(vbox);
    
    
    // signals
    QObject::connect(buttonTools["mapTileShowTool"], SIGNAL(toggled(bool)),
                      this, SLOT(mapTileShowToolEnabled(bool)));
    
    QObject::connect(buttonTools["mapTileLoadTool"], SIGNAL(toggled(bool)),
                      this, SLOT(mapTileLoadToolEnabled(bool)));
    
    QObject::connect(buttonTools["heightTileLoadTool"], SIGNAL(toggled(bool)),
                      this, SLOT(heightTileLoadToolEnabled(bool)));
    
}

void GeoTools::mapTileShowToolEnabled(bool val){
    if(val){
        emit enableTool("mapTileShowTool");
    } else {
        emit enableTool("");
    }
}

void GeoTools::mapTileLoadToolEnabled(bool val){
    if(val){
        emit enableTool("mapTileLoadTool");
    } else {
        emit enableTool("");
    }
}

void GeoTools::heightTileLoadToolEnabled(bool val){
    if(val){
        emit enableTool("heightTileLoadTool");
    } else {
        emit enableTool("");
    }
}

GeoTools::~GeoTools() {
}

void GeoTools::msg(QString text, QString val){
    if(text == "toolEnabled"){
        QMapIterator<QString, QPushButton*> i(buttonTools);
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(true);
            i.value()->setChecked(false);
        }
        if(buttonTools[val] != NULL)
            buttonTools[val]->setChecked(true);
        i.toFront();
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(false);
        }
    }
}