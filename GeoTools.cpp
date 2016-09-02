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
    
    QPushButton *mapTileShowTool = new QPushButton("Show/H Map", this);
    QPushButton *mapTileLoadTool = new QPushButton("Load Map", this);
    QPushButton *heightTileLoadTool = new QPushButton("Load Height", this);
    
    
    QGridLayout *vlist3 = new QGridLayout;
    vlist3->setSpacing(2);
    vlist3->setContentsMargins(3,0,1,0);    
    row = 0;
    vlist3->addWidget(mapTileShowTool,row,0);
    vlist3->addWidget(mapTileLoadTool,row,1);
    vlist3->addWidget(heightTileLoadTool,row++,2);

    QLabel *label0;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    
    label0 = new QLabel("World Tile:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet("QLabel { color : #999999; }");
    vbox->addWidget(label0);
    vbox->addItem(vlist3);

    vbox->addStretch(1);
    this->setLayout(vbox);
    
    
    // signals
    QObject::connect(mapTileShowTool, SIGNAL(released()),
                      this, SLOT(mapTileShowToolEnabled()));
    
    QObject::connect(mapTileLoadTool, SIGNAL(released()),
                      this, SLOT(mapTileLoadToolEnabled()));
    
    QObject::connect(heightTileLoadTool, SIGNAL(released()),
                      this, SLOT(heightTileLoadToolEnabled()));
    
}

void GeoTools::mapTileShowToolEnabled(){
    emit enableTool("mapTileShowTool");
}

void GeoTools::mapTileLoadToolEnabled(){
    emit enableTool("mapTileLoadTool");
}

void GeoTools::heightTileLoadToolEnabled(){
    emit enableTool("heightTileLoadTool");
}

GeoTools::~GeoTools() {
}

