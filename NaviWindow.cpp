/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "NaviWindow.h"
#include "IghCoords.h"
#include <QtWidgets>
#include <QDebug>
#include "Coords.h"
#include "CoordsMkr.h"
#include "CoordsKml.h"

NaviWindow::NaviWindow(){
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    this->setFixedWidth(300);
    this->setFixedHeight(160);
    this->setWindowTitle(tr("World Position"));
    markerFiles.setStyleSheet("combobox-popup: 0;");
    markerList.setStyleSheet("combobox-popup: 0;");
    
    QPushButton *jumpButton = new QPushButton("Jump", this);
    QLabel *txLabel = new QLabel("X", this);
    QLabel *tyLabel = new QLabel("Y", this);
    QLabel *xLabel = new QLabel("x", this);
    QLabel *yLabel = new QLabel("y", this);
    QLabel *zLabel = new QLabel("z", this);
    QLabel *latLabel = new QLabel("lat", this);
    QLabel *lonLabel = new QLabel("lon", this);
    QLabel *empty = new QLabel(" ", this);
    
    QLabel *label1 = new QLabel("Position:");
    label1->setContentsMargins(3,0,0,0);
    label1->setStyleSheet("QLabel { color : #999999; }");

    QVBoxLayout *v = new QVBoxLayout;
    v->setSpacing(2);
    v->setContentsMargins(0,1,1,1);
    v->addWidget(&markerFiles);
    v->addWidget(&markerList);
    QGridLayout *vbox = new QGridLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(3,0,1,0);    
    int row = 0;
    vbox->addWidget(xLabel,0,0);
    vbox->addWidget(&xBox,0,1);
    vbox->addWidget(yLabel,0,2);
    vbox->addWidget(&yBox,0,3);
    vbox->addWidget(zLabel,0,4);
    vbox->addWidget(&zBox,0,5);

    v->addItem(vbox);
    
    vbox = new QGridLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(3,0,1,0);    
    //int row = 0;
    vbox->addWidget(txLabel,0,0);
    vbox->addWidget(&txBox,0,1);
    vbox->addWidget(tyLabel,0,2);
    vbox->addWidget(&tyBox,0,3);
    vbox->addWidget(latLabel,1,0);
    vbox->addWidget(&latBox,1,1);
    vbox->addWidget(lonLabel,1,2);
    vbox->addWidget(&lonBox,1,3);
    v->addItem(vbox);
    //vbox = new QGridLayout;
    //vbox->setSpacing(2);
    //vbox->setContentsMargins(3,0,1,0);    
    v->addWidget(&tileInfo);
    v->addWidget(jumpButton);
    //vbox->addStretch(1);
    this->setLayout(v);
    
    QObject::connect(&txBox, SIGNAL(textEdited(QString)),
                      this, SLOT(xyChanged(QString)));
    QObject::connect(&tyBox, SIGNAL(textEdited(QString)),
                      this, SLOT(xyChanged(QString)));
    QObject::connect(&xBox, SIGNAL(textEdited(QString)),
                      this, SLOT(xyChanged(QString)));
    QObject::connect(&yBox, SIGNAL(textEdited(QString)),
                      this, SLOT(xyChanged(QString)));
    QObject::connect(&zBox, SIGNAL(textEdited(QString)),
                      this, SLOT(xyChanged(QString)));
    QObject::connect(&latBox, SIGNAL(textEdited(QString)),
                      this, SLOT(latLonChanged(QString)));
    QObject::connect(&lonBox, SIGNAL(textEdited(QString)),
                      this, SLOT(latLonChanged(QString)));
    
    QObject::connect(jumpButton, SIGNAL(released()),
                      this, SLOT(jumpTileSelected()));
    
    QObject::connect(&markerFiles, SIGNAL(activated(QString)),
                      this, SLOT(mkrFilesSelected(QString)));
    QObject::connect(&markerList, SIGNAL(activated(QString)),
                      this, SLOT(mkrListSelected(QString)));
    

    tileInfo.setText(" ");
}

void NaviWindow::latLonChanged(QString val){
    this->jumpType = "latlon";
}
void NaviWindow::xyChanged(QString val){
    this->jumpType = "xy";
}

void NaviWindow::jumpTileSelected(){
    if(aCoords == NULL)
        aCoords = new PreciseTileCoordinate();
    
    if(this->jumpType == "xy"){
        aCoords->setWxyz(xBox.text().toInt(), yBox.text().toInt(), zBox.text().toInt());
        aCoords->TileX = txBox.text().toInt();
        aCoords->TileZ = tyBox.text().toInt();
        emit jumpTo(aCoords);
    }
    if(this->jumpType == "latlon"){
        igh = MstsCoordinates::ConvertToIgh(latBox.text().toDouble(), lonBox.text().toDouble(), igh);
        aCoords = MstsCoordinates::ConvertToTile(igh, aCoords);
        aCoords->setWxyz();
        aCoords->wZ = -aCoords->wZ;
        emit jumpTo(aCoords);
    }
    if(this->jumpType == "marker"){
        if(mkrPlaces[markerList.currentText().toStdString()] == NULL) return;
        igh = MstsCoordinates::ConvertToIgh(mkrPlaces[markerList.currentText().toStdString()]->Latitude, mkrPlaces[markerList.currentText().toStdString()]->Longitude, igh);
        aCoords = MstsCoordinates::ConvertToTile(igh, aCoords);
        aCoords->setWxyz();
        aCoords->wZ = -aCoords->wZ;
        emit jumpTo(aCoords);
    }

}

void NaviWindow::naviInfo(int all, int hidden){
    if(all != objCount || hidden != objHidden ){
        objCount = all;
        objHidden = hidden;
        this->tileInfo.setText("Objects: "+QString::number(all, 10)+" ( including "+QString::number(hidden, 10)+" hidden )");
    }
}

void NaviWindow::posInfo(PreciseTileCoordinate* coords){
    if(lastX != coords->X || lastY != coords->Y || lastZ != coords->Z || lastTX != coords->TileX || lastTZ != coords->TileZ){
        lastX = coords->wX;
        lastY = coords->wY;
        lastZ = coords->wZ;
        lastTX = coords->TileX;
        lastTZ = coords->TileZ;
        this->txBox.setText(QString::number(lastTX, 10));
        this->tyBox.setText(QString::number(lastTZ, 10));
        this->xBox.setText(QString::number(lastX, 10));
        this->yBox.setText(QString::number(lastY, 10));
        this->zBox.setText(QString::number(lastZ, 10));
        igh = MstsCoordinates::ConvertToIgh(coords);
        latlon = MstsCoordinates::ConvertToLatLon(igh);
        this->latBox.setText(QString::number(latlon->Latitude));
        this->lonBox.setText(QString::number(latlon->Longitude));
    }
}

void NaviWindow::mkrList(std::unordered_map<std::string, Coords*> list){
    mkrFiles = list;
    for (auto it = list.begin(); it != list.end(); ++it ){
        markerFiles.addItem(QString::fromStdString(it->first));
    }
}

void NaviWindow::mkrFilesSelected(QString item){
    Coords* c = mkrFiles[item.toStdString()];
    if(c == NULL) return;
    this->sendMsg("mkrFile", item);
    this->mkrPlaces.clear();
    
    QStringList hash;

    for(int i = 0; i < c->markerList.size(); i++){
        if(this->mkrPlaces[c->markerList[i].name.toStdString()] == NULL)
            this->mkrPlaces[c->markerList[i].name.toStdString()] = new LatitudeLongitudeCoordinate();
        this->mkrPlaces[c->markerList[i].name.toStdString()]->Latitude = c->markerList[i].lat;
        this->mkrPlaces[c->markerList[i].name.toStdString()]->Longitude = c->markerList[i].lon;
        hash.append(c->markerList[i].name);
    }
    hash.sort(Qt::CaseInsensitive);
    hash.removeDuplicates();
    markerList.clear();
    markerList.addItems(hash);
    markerList.setMaxVisibleItems(25);
}

void NaviWindow::mkrListSelected(QString item){
        this->jumpType = "marker";
}
NaviWindow::~NaviWindow() {
}

void NaviWindow::hideEvent(QHideEvent *e){
    emit windowClosed();
}
