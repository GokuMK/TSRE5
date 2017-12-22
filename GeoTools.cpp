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
#include "Coords.h"
#include "HeightWindow.h"

GeoTools::GeoTools(QString name)
    : QWidget(){
    setFixedWidth(250);
    int row = 0;
    
    buttonTools["mapTileShowTool"] = new QPushButton("Show/Hide Map", this);
    buttonTools["mapTileLoadTool"] = new QPushButton("Load Map", this);
    buttonTools["heightTileLoadTool"] = new QPushButton("Load Height", this);
    buttonTools["makeTileTextureTool"] = new QPushButton("Make Tile Texture from Map", this);
    buttonTools["removeTileTextureTool"] = new QPushButton("Remove Map Tile Texture", this);
    QMapIterator<QString, QPushButton*> i(buttonTools);
    while (i.hasNext()) {
        i.next();
        i.value()->setCheckable(true);
    }

    QLabel *label0;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
        
    label0 = new QLabel("Map Layers:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addWidget(buttonTools["mapTileShowTool"]);
    vbox->addWidget(buttonTools["mapTileLoadTool"]);
    vbox->addWidget(buttonTools["makeTileTextureTool"]);
    vbox->addWidget(buttonTools["removeTileTextureTool"]);
    
    label0 = new QLabel("Terrain Heightmap:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addWidget(buttonTools["heightTileLoadTool"]);
    
    label0 = new QLabel("Auto tile generation:");
    label0->setContentsMargins(3,0,0,0);
    label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    QCheckBox *chAutoCreateTile = new QCheckBox("Create new tiles if not exist.");
    chAutoCreateTile->setChecked(Game::autoNewTiles);
    QCheckBox *chAutoGeoTerrain = new QCheckBox("Create terrain from Geodata. ");
    chAutoCreateTile->setChecked(Game::autoGeoTerrain);
    vbox->addWidget(chAutoCreateTile);
    vbox->addWidget(chAutoGeoTerrain);
    
    label0 = new QLabel("Tiles from marker file:");
    label0->setContentsMargins(3,0,0,0);
    //label0->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    vbox->addWidget(label0);
    vbox->addWidget(&markerFiles);
    markerFiles.setStyleSheet("combobox-popup: 0;");
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Radius:",&this->eRadius);
    eRadius.setRange(0,2);
    eRadius.setValue(0);
    vbox->addItem(vlist);
    QPushButton * checkGeodataFiles = new QPushButton("Check if geodata files available.", this);
    QObject::connect(checkGeodataFiles, SIGNAL(released()),
                      this, SLOT(checkGeodataFilesEnabled()));
    vbox->addWidget(checkGeodataFiles);
    
    QPushButton * generateTiles = new QPushButton("Generate tiles.", this);
    QObject::connect(generateTiles, SIGNAL(released()),
                      this, SLOT(generateTilesEnabled()));
    vbox->addWidget(generateTiles);

    label0 = new QLabel("Distant Terrain:");
    label0->setContentsMargins(3,0,0,0);
    vbox->addWidget(label0);
    QPushButton * checkGeodataLoFiles = new QPushButton("Check if geodata files available.", this);
    //QObject::connect(checkGeodataFiles, SIGNAL(released()),
    //                  this, SLOT(checkGeodataFilesEnabled()));
    vbox->addWidget(checkGeodataLoFiles);
    
    QPushButton * generateLoTiles = new QPushButton("Generate tiles using MKR.", this);
    QObject::connect(generateLoTiles, SIGNAL(released()),
                      this, SLOT(generateLoTilesEnabled()));
    vbox->addWidget(generateLoTiles);
    QPushButton * generateLoTilesFromTDB = new QPushButton("Generate tiles using TDB.", this);
    QObject::connect(generateLoTilesFromTDB, SIGNAL(released()),
                      this, SLOT(generateLoTilesFromTDBEnabled()));
    vbox->addWidget(generateLoTilesFromTDB);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    
    // signals
    QObject::connect(buttonTools["mapTileShowTool"], SIGNAL(toggled(bool)),
                      this, SLOT(mapTileShowToolEnabled(bool)));
    
    QObject::connect(buttonTools["mapTileLoadTool"], SIGNAL(toggled(bool)),
                      this, SLOT(mapTileLoadToolEnabled(bool)));
    
    QObject::connect(buttonTools["heightTileLoadTool"], SIGNAL(toggled(bool)),
                      this, SLOT(heightTileLoadToolEnabled(bool)));
    
    QObject::connect(buttonTools["makeTileTextureTool"], SIGNAL(toggled(bool)),
                      this, SLOT(makeTileTextureToolEnabled(bool)));
    
    QObject::connect(buttonTools["removeTileTextureTool"], SIGNAL(toggled(bool)),
                      this, SLOT(removeTileTextureToolEnabled(bool)));
    
    QObject::connect(chAutoCreateTile, SIGNAL(stateChanged(int)),
                      this, SLOT(chAutoCreateTileEnabled(int)));
    
    QObject::connect(chAutoGeoTerrain, SIGNAL(stateChanged(int)),
                      this, SLOT(chAutoGeoTerrainEnabled(int)));
    
}

void GeoTools::mkrList(QMap<QString, Coords*> list){
    mkrFiles = list;
    for (auto it = list.begin(); it != list.end(); ++it ){
        if(it.value() == NULL)
            continue;
        if(!it.value()->loaded)
            continue;
        if(it.key().startsWith("|"))
            continue;
        markerFiles.addItem(it.key());
    }
    //    mkrFilesSelected(markerFiles.itemText(0));
}

void GeoTools::checkGeodataFilesEnabled(){
    if(markerFiles.count() == 0)
        return;
    Coords* c = mkrFiles[markerFiles.currentText()];
    if(c == NULL) 
        return;
    
    QMap<int, QPair<int, int>*> tileList;
    int radius = eRadius.value();
    c->getTileList(tileList, radius);

    /*int x, z;
    QMapIterator<int, QPair<int, int>*> i2(tileList);
    while (i2.hasNext()) {
        i2.next();
        if(i2.value() == NULL)
            continue;
        x = i2.value()->first;
        z = i2.value()->second;
        qDebug() << x << z;
    }*/
    HeightWindow::CheckForMissingGeodataFiles(tileList);
}

void GeoTools::generateTilesEnabled(){
    if(markerFiles.count() == 0)
        return;
    Coords* c = mkrFiles[markerFiles.currentText()];
    if(c == NULL) 
        return;
    
    QMap<int, QPair<int, int>*> tileList;
    int radius = eRadius.value();
    c->getTileList(tileList, radius);
    
    emit createNewTiles(tileList);
}

void GeoTools::generateLoTilesEnabled(){
    if(markerFiles.count() == 0)
        return;
    Coords* c = mkrFiles[markerFiles.currentText()];
    if(c == NULL) 
        return;
    
    QMap<int, QPair<int, int>*> tileList;
    int radius = eRadius.value();
    c->getTileList(tileList, radius, 8);
    
    emit createNewLoTiles(tileList);
}

void GeoTools::generateLoTilesFromTDBEnabled(){

    TDB* tdb = Game::trackDB;
    if(tdb == NULL) 
        return;
    
    QMap<int, QPair<int, int>*> tileList;
    int radius = eRadius.value();
    tdb->getUsedTileList(tileList, radius, 8);
    
    emit createNewLoTiles(tileList);
}

void GeoTools::chAutoCreateTileEnabled(int state){
    if(state == Qt::Checked)
        Game::autoNewTiles = true;
    else
        Game::autoNewTiles = false;
}

void GeoTools::chAutoGeoTerrainEnabled(int state){
    if(state == Qt::Checked)
        Game::autoGeoTerrain = true;
    else
        Game::autoGeoTerrain = false;
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

void GeoTools::makeTileTextureToolEnabled(bool val){
    if(val){
        emit enableTool("makeTileTextureTool");
    } else {
        emit enableTool("");
    }
}

void GeoTools::removeTileTextureToolEnabled(bool val){
    if(val){
        emit enableTool("removeTileTextureTool");
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