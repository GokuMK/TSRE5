/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "MapDataOSM.h"
#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <QString>
#include <QImage>
#include <QPainter>
#include "IghCoords.h"
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include "CoordsMkr.h"
#include "IghCoords.h"
#include "OSMFeatures.h"
#include <QTime>

MapDataOSM::MapDataOSM() {
}

MapDataOSM::~MapDataOSM() {
}

void MapDataOSM::setColor(QColor* color){
    setColor(color->red(), color->green(), color->blue());
}

void MapDataOSM::setColor(int r, int g, int b){
    color->setRgb(r,g,b);
    p->setColor(*color);
    gg->setPen(*p);
    brush->setColor(*color);
}

void MapDataOSM::setPenSettings(QPen* pen){
    p->setCapStyle(pen->capStyle());
    p->setJoinStyle(pen->joinStyle());
    p->setWidthF(pen->widthF());
    gg->setPen(*p);
}

bool MapDataOSM::draw(QImage* myImage) {
    if(nodes.size() == 0) return false;
    
    igh = new IghCoordinate();
    latlon = new LatitudeLongitudeCoordinate();
    aCoords = new PreciseTileCoordinate();
    
    gg = new QPainter();
    gg->begin(myImage);
    gg->setRenderHint(QPainter::RenderHint::Antialiasing, false);
    color = new QColor();
    roadBorder = new QColor();
    p = new QPen;
    brush = new QBrush;
    brush->setStyle(Qt::SolidPattern);
    setColor(0,0,255);
    
    height = myImage->height(); //gg->getClipBounds().height;
    width = myImage->width(); //gg->getClipBounds().width;
    
    float zoom = 1.0;
    QBrush tbrush;
    QPen basicStroke(tbrush, 0, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
    QPen olinia14(tbrush, 14.0 / zoom, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);// = new BasicStroke(14 / zoom, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND);
    QPen olinia12(tbrush, 12.0 / zoom, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);// = new BasicStroke(12 / zoom, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND);
    QPen olinia10(tbrush, 10.0 / zoom, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);// = new BasicStroke(10 / zoom, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND);
    QPen olinia8(tbrush, 8.0 / zoom, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);// = new BasicStroke(8 / zoom, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND);
    QPen olinia6(tbrush, 6.0 / zoom, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);// = new BasicStroke(6 / zoom, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND);
    QPen olinia4(tbrush, 4.0 / zoom, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);// = new BasicStroke(4 / zoom, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND);
    QPen olinia2(tbrush, 2.0 / zoom, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);// = new BasicStroke(2 / zoom, BasicStroke.CAP_BUTT, BasicStroke.JOIN_ROUND);

    QPen slinia12(tbrush, 12.0 / zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);// = new BasicStroke(12 / zoom, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND);
    QPen slinia10(tbrush, 10.0 / zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);// = new BasicStroke(10 / zoom, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND);
    QPen slinia8(tbrush, 8.0 / zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);// = new BasicStroke(8 / zoom, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND);
    QPen slinia6(tbrush, 6.0 / zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);// = new BasicStroke(6 / zoom, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND);
    QPen slinia4(tbrush, 4.0 / zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);// = new BasicStroke(4 / zoom, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND);
    QPen slinia2(tbrush, 2.0 / zoom, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);// = new BasicStroke(2 / zoom, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND);
    /////////*/
    //p.setColor(QColor::fromRgb(241, 238, 232));
    gg->fillRect(0, 0, (int)width, (int)height, QColor::fromRgb(241, 238, 232));
    int fail = 0, tf = 0;

    Way* w;
    QVector<QPoint> ww;//, wy;
    QPolygon poly;
    QPainterPath path;
    int drawX, drawY;
    
    for(int layer = 0; layer < 10; layer++)
    for (int i = 0; i < this->ways[layer].size(); i++) {
        w = this->ways[layer][i];
        ww.clear();
        poly.clear();
        path = QPainterPath();

        if (w->val2 == 7) roadBorder->setRgb(0, 0, 0);
        else roadBorder->setRgb(180, 180, 180);

        for (int i = 0; i < w->ref.size(); i++) {
            if (nodes[w->ref[i]] == NULL) {
                fail++;
                break;
            }
            r(drawX, drawY, nodes[w->ref[i]]->lat, nodes[w->ref[i]]->lon);
            ww.push_back(QPoint(drawX, drawY));
            poly.push_back(QPoint(drawX, drawY));
            //ww.push_back(QPoint(rX(nodes[w->ref[i]]->lon), rY(nodes[w->ref[i]]->lat)));
        }
        path.addPolygon(poly);
        
        ///gg->drawPolyline(ww);
        //typ rysowania:
        if (w->type > 129 && w->type < 163) {
            (setColor(190, 173, 173));
            gg->fillPath(path, *brush);
            (setColor(169, 148, 165));
            gg->drawPolyline(ww);
        } else if (w->type > 459 && w->type < 548) {
            (setColor(200, 170, 170));
            gg->fillPath(path, *brush);
            (setColor(169, 148, 165));
            gg->drawPolyline(ww);
        } else if (w->type == 381) {
            (setColor(141, 196, 108));
            gg->fillPath(path, *brush);
        } else if (w->type == 287) {
            (setColor(133, 193, 133));
            gg->fillPath(path, *brush);
        } else if (w->type == 442) {
            (setColor(212, 170, 170));
            gg->fillPath(path, *brush);
        } else if (w->type == 289 || w->type == 305 || w->type == 300) {
            (setColor(207, 236, 168));
            gg->fillPath(path, *brush);
        } else if (w->type == 321 || w->type == 322 || w->type == 317) {
            (setColor(206, 246, 202));
            gg->fillPath(path, *brush);
        } else if (w->type == 318) {
            (setColor(137, 210, 174));
            gg->fillPath(path, *brush);
            (setColor(180, 180, 180));
            gg->drawPath(path);
        } else if (w->type == 324) {
            (setColor(116, 219, 185));
            gg->fillPath(path, *brush);
            (setColor(180, 180, 180));
            gg->drawPath(path);
        } else if (w->type == 374) {
            (setColor(181, 226, 181));
            gg->fillPath(path, *brush);
        } else if (w->type == 380) {
            (setColor(95, 180, 160));
            gg->fillPath(path, *brush);
        } else if (w->type == 379 || w->type == 633 || w->type == 301 || w->type == 279) {
            (setColor(181, 208, 208));
            gg->fillPath(path, *brush);
        } else if (w->type == 61) {
            (setColor(246, 238, 182));
            gg->fillPath(path, *brush);
        } else if (w->type == 284) {
            (setColor(234, 216, 184));
            gg->fillPath(path, *brush);
        } else if (w->type == 298) {
            (setColor(195, 195, 195));
            gg->fillPath(path, *brush);
        } else if (w->type == 288) {
            (setColor(224, 224, 206));
            gg->fillPath(path, *brush);
        } else if (w->type == 282) {
            (setColor(238, 200, 200));
            gg->fillPath(path, *brush);
        } else if (w->type == 281) {
            (setColor(151, 191, 164));
            gg->fillPath(path, *brush);
        } else if (w->type == 299) {
            (setColor(222, 208, 213));
            gg->fillPath(path, *brush);
        } else if (w->type == 292) {
            (setColor(222, 208, 213));
            gg->fillPath(path, *brush);
        } else if (w->type == 303) {
            (setColor(234, 214, 214));
            gg->fillPath(path, *brush);
        } else if (w->type == 291) {
            (setColor(231, 241, 222));
            gg->fillPath(path, *brush);
        } else if (w->type == 297 || w->type == 278) {
            (setColor(204, 220, 112));
            gg->fillPath(path, *brush);
        } else if (w->type == 239) {
            (setColor(157, 256, 108));
            gg->fillPath(path, *brush);
        } else if (w->type == 624) {
            (setColor(164, 242, 161));
            gg->fillPath(path, *brush);
        } else if (w->type == 367) {
            (setColor(213, 216, 159));
            gg->fillPath(path, *brush);
        } else if (w->type == 360) {
            (setColor(254, 240, 186));
            gg->fillPath(path, *brush);
        } else if (w->type == 293 || w->type == 283 || w->type == 290 || w->type == 280) {
            (setColor(176, 176, 142));
            gg->fillPath(path, *brush);
        } else if (w->type == 319) {
            (setColor(204, 254, 254));
            gg->fillPath(path, *brush);
            (setColor(180, 180, 180));
            gg->drawPath(path);
        } else if (w->type == 307 || w->type == 311 || w->type == 312) {
            (setColor(199, 241, 163));
            gg->fillPath(path, *brush);
            (setColor(148, 214, 151));
            gg->drawPath(path);
        } else if (w->type > 624 && w->type < 637) {
            (setColor(181, 208, 208));
            setPenSettings(&slinia10);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 246 || w->type == 227) {
            setColor(roadBorder);
            setPenSettings(&olinia12);
            gg->drawPolyline(ww);
            (setColor(254, 254, 254));
            setPenSettings(&slinia10);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 264) {
            setColor(roadBorder);
            setPenSettings(&olinia8);
            gg->drawPolyline(ww);
            (setColor(254, 254, 254));
            setPenSettings(&slinia6);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 251) {
            setColor(roadBorder);
            setPenSettings(&olinia6);
            gg->drawPolyline(ww);
            (setColor(254, 254, 254));
            setPenSettings(&slinia4);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 257) {
            setColor(roadBorder);
            setPenSettings(&olinia12);
            gg->drawPolyline(ww);
            (setColor(252, 250, 116));
            setPenSettings(&slinia10);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 258) {
            setColor(roadBorder);
            setPenSettings(&olinia10);
            gg->drawPolyline(ww);
            (setColor(252, 255, 136));
            setPenSettings(&slinia8);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 240 || w->type == 226) {
            setColor(roadBorder);
            setPenSettings(&olinia4);
            gg->drawPolyline(ww);
            (setColor(230, 230, 230));
            setPenSettings(&slinia2);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 241 || w->type == 234) {
            setColor(roadBorder);
            setPenSettings(&olinia4);
            gg->drawPolyline(ww);
            (setColor(230, 230, 230));
            setPenSettings(&slinia2);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 302) {
            (setColor(220, 220, 220));
            gg->fillPath(path, *brush);
        } else if (w->type == 249) {
            setColor(roadBorder);
            setPenSettings(&olinia12);
            gg->drawPolyline(ww);
            (setColor(253, 191, 111));
            setPenSettings(&slinia10);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 250) {
            setColor(roadBorder);
            setPenSettings(&olinia10);
            gg->drawPolyline(ww);
            (setColor(253, 211, 121));
            setPenSettings(&slinia8);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 231) {
            setColor(roadBorder);
            setPenSettings(&olinia4);
            gg->drawPolyline(ww);
            (setColor(254, 200, 200));
            setPenSettings(&slinia2);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 223) {
            setColor(roadBorder);
            setPenSettings(&olinia4);
            gg->drawPolyline(ww);
            (setColor(200, 254, 200));
            setPenSettings(&slinia2);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 254) {
            setColor(roadBorder);
            setPenSettings(&olinia4);
            gg->drawPolyline(ww);
            (setColor(254, 100, 100));
            setPenSettings(&slinia2);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 229) {
            setColor(roadBorder);
            setPenSettings(&olinia4);
            gg->drawPolyline(ww);
            (setColor(200, 200, 254));
            setPenSettings(&slinia2);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 242) {
            setColor(roadBorder);
            setPenSettings(&olinia12);
            gg->drawPolyline(ww);
            (setColor(228, 109, 113));
            setPenSettings(&slinia10);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 243) {
            setColor(roadBorder);
            setPenSettings(&olinia10);
            gg->drawPolyline(ww);
            (setColor(228, 129, 133));
            setPenSettings(&slinia8);
            gg->drawPolyline(ww);
            //setPenSettings(&basicStroke);
        } else if (w->type == 236 || w->type == 261) {
            setColor(roadBorder);
            setPenSettings(&olinia14);
            gg->drawPolyline(ww);
            (setColor(255, 69, 0));
            setPenSettings(&slinia12);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 237 || w->type == 238 || w->type == 262) {
            setColor(roadBorder);
            setPenSettings(&olinia10);
            gg->drawPolyline(ww);
            (setColor(255, 89, 20));
            setPenSettings(&slinia8);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 259) {
            setColor(roadBorder);
            setPenSettings(&olinia4);
            gg->drawPolyline(ww);
            (setColor(220, 220, 220));
            setPenSettings(&slinia2);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 445) {
            if (w->val2 == 7) {
                (setColor(0, 0, 0));
                setPenSettings(&olinia6);
                gg->drawPolyline(ww);
                (setColor(255, 255, 255));
                setPenSettings(&olinia4);
                gg->drawPolyline(ww);
                (setColor(110, 110, 110));
                setPenSettings(&slinia2);
                gg->drawPolyline(ww);
                setPenSettings(&basicStroke);
            } else {
                (setColor(90, 90, 90));
                setPenSettings(&slinia2);
                gg->drawPolyline(ww);
                setPenSettings(&basicStroke);
            }
        } else if (w->type == 440) {
            if (w->val2 == 7) {
                (setColor(0, 0, 0));
                setPenSettings(&olinia8);
                gg->drawPolyline(ww);
                (setColor(255, 255, 255));
                setPenSettings(&olinia6);
                gg->drawPolyline(ww);
                (setColor(90, 90, 90));
                setPenSettings(&slinia4);
                gg->drawPolyline(ww);
                setPenSettings(&basicStroke);
            } else {
                (setColor(70, 70, 70));
                setPenSettings(&slinia4);
                gg->drawPolyline(ww);
                setPenSettings(&basicStroke);
            }
        } else if (w->type == 14) {
            (setColor(204, 153, 254));
            gg->fillPath(path, *brush);
            (setColor(154, 117, 182));
            gg->drawPolyline(ww);
        } else if (w->type == 75) {
            (setColor(240, 240, 216));
            gg->fillPath(path, *brush);
            (setColor(210, 180, 160));
            gg->drawPolyline(ww);
        } else if (w->type == 65) {
            (setColor(220, 130, 110));
            gg->fillPath(path, *brush);
            (setColor(150, 150, 150));
            gg->drawPolyline(ww);
        } else if (w->type == 12 || w->type == 13) {
            (setColor(187, 187, 204));
            setPenSettings(&slinia10);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 71) {
            (setColor(190, 173, 173));
            gg->fillPath(path, *brush);
            (setColor(169, 148, 165));
            gg->drawPolyline(ww);
        } else if (w->type == 268) {
            (setColor(173, 173, 173));
            gg->fillPath(path, *brush);
            (setColor(169, 148, 165));
            gg->drawPolyline(ww);
        } else if (w->type == 285) {
            (setColor(233, 216, 189));
            gg->fillPath(path, *brush);
        } else if (w->type == 286) {
            (setColor(220, 190, 146));
            gg->fillPath(path, *brush);
        } else if (w->type == 294) {
            (setColor(207, 236, 168));
            gg->fillPath(path, *brush);
        } else if (w->type == 296) {
            (setColor(207, 255, 168));
            gg->fillPath(path, *brush);
        } else if (w->type == 366) {
            (setColor(198, 228, 180));
            gg->fillPath(path, *brush);
        } else if (w->type == 407) {
            (setColor(241, 238, 232));
            gg->fillPath(path, *brush);
        } else if (w->type == 338) {
            (setColor(241, 238, 232));
            setPenSettings(&slinia10);
            gg->drawPolyline(ww);
            setPenSettings(&basicStroke);
        } else if (w->type == 363) {
            (setColor(150, 150, 150));
            gg->drawPolyline(ww);
        } else {
            (setColor(50, 50, 50));
            gg->drawPolyline(ww);
        }
    }

    gg->end();
    qDebug() << "fail " << fail;
    return true;
}

void MapDataOSM::r(int& x, int& y, float lat, float lon){
    igh = MstsCoordinates::ConvertToIgh(lat, lon, igh);
    aCoords = MstsCoordinates::ConvertToTile(igh, aCoords);
    x = (aCoords->X+(aCoords->TileX-this->tileX))*height;
    y = (aCoords->Z-(aCoords->TileZ-this->tileZ))*height;
}

int MapDataOSM::rX(float tlon) {
    //float minlon = lon - lonScale*zoom;
    //float maxlon = lon + lonScale*zoom;
    return (int) ((float) (tlon - minlon)*((float) height / ((maxlon - minlon))));
}

int MapDataOSM::rY(float tlat) {
    //float minlat = lat - latScale*zoom;
    //float maxlat = lat + latScale*zoom;
    return (int) ((float) (tlat - maxlat)*((float) height / ((minlat - maxlat))));
}

void MapDataOSM::load(){

    LatitudeLongitudeCoordinate p00;
    p00.Latitude = (maxlat + minlat)/2.0;
    p00.Longitude = (maxlon + minlon)/2.0;
    LatitudeLongitudeCoordinate p01;
    p01.Latitude = (maxlat + minlat)/2.0;
    p01.Longitude = maxlon;
    LatitudeLongitudeCoordinate p10;
    p10.Latitude = maxlat;
    p10.Longitude = (maxlon + minlon)/2.0;
    LatitudeLongitudeCoordinate pm10;
    pm10.Latitude = minlat;
    pm10.Longitude = (maxlon + minlon)/2.0;
    LatitudeLongitudeCoordinate p0m1;
    p0m1.Latitude = (maxlat + minlat)/2.0;
    p0m1.Longitude = minlon;
    LatitudeLongitudeCoordinate minLatlon;
    LatitudeLongitudeCoordinate maxLatlon;
    minLatlon.Latitude = minlat;
    minLatlon.Longitude = minlon;
    maxLatlon.Latitude = maxlat;
    maxLatlon.Longitude = maxlon;
    
    qDebug() << "lat " << minlat << " " << maxlat;
    qDebug() << "lon " << minlon << " " << maxlon;
    
    // split osm request to four 1024x1024m requests. 
    loadCount = 0;
    totalLoadCount = 4;
    
    get(&minLatlon, &p00);
    get(&p00, &maxLatlon);
    get(&pm10, &p01);
    get(&p0m1, &p10);
}

void MapDataOSM::get(LatitudeLongitudeCoordinate* min, LatitudeLongitudeCoordinate* max){
    QNetworkAccessManager* mgr = new QNetworkAccessManager();
    connect(mgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(isData(QNetworkReply*)));
    // the HTTP request
    qDebug() << "wait";
    QNetworkRequest req( QUrl( QString("http://www.openstreetmap.org/api/0.6/map?bbox="
    +QString::number(min->Longitude)
    +","
    +QString::number(min->Latitude)
    +","
    +QString::number(max->Longitude)
    +","
    +QString::number(max->Latitude)
    ) ) );
    mgr->get(req);
}

void MapDataOSM::isData(QNetworkReply* r){
    QByteArray data = r->readAll();
    qDebug() << "data " << data.length();    

    if(data.length()==0){
        //"No data from the network..." label
        emit statusInfo(QString("No data from the network..."));
        // 5 seconds, warning time.
        QTime cTime = QTime::currentTime().addSecs(5);  
        while (QTime::currentTime() < cTime){
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        //End delay
        r->close();
        //"Load" label
        emit statusInfo(QString("Load"));
    } else {
        loadData(&data);
        loadCount++;
        if(loadCount == totalLoadCount){
            emit statusInfo(QString("Load"));
            emit loaded(); 
        }
    }
}

void MapDataOSM::loadData(QByteArray* data){
    if(data == NULL){
        QFile file("F:/OSM/tczew.osm");
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            qDebug() << "no file" << file.errorString();
            exit(0);
        }
        qDebug() <<  "file";
        QByteArray data2 = file.readAll();
        data = &data2;
    } 
    
    int inode = 0;
    int iway = 0;
    bool node = true;
    bool way = false;
    bool bounds = false;
    int iii = 0, uuu = 0;
    
    Node* tnode;
    Way* tway;
    
    QXmlStreamReader reader((*data));
    reader.readNext();
    QString name;
    QXmlStreamAttributes attr;
    while (!reader.isEndDocument()) {
        if (reader.isStartElement()) {
            name = reader.name().toString();
            attr = reader.attributes();
            
            if (name.toUpper() == ("NODE")) {
                node = true;
                if (inode++ % 100000 == 0) qDebug() << "n " << inode;
                tnode = new Node(
                        attr.value("id").toULongLong(),
                        attr.value("lat").toFloat(),
                        attr.value("lon").toFloat()
                        );
            } else if (name.toUpper() == ("WAY")) {
                way = true;
                if (iway++ % 100000 == 0) qDebug() << "w " << iway;
                tway = new Way(attr.value("id").toULongLong());
                tway->ref.clear();
            } else if (name.toUpper() == ("ND") && way) {
                tway->ref.push_back ((attr.value("ref").toLongLong()));
            } else if (name.toUpper() == ("TAG")&&(way || node)) {
                //adres
                if (attr.value("k").startsWith("ADDR", Qt::CaseInsensitive)) {
                }
                //nazwa
                else if (attr.value("k").startsWith("NAME", Qt::CaseInsensitive)) {
                }
                //drogi
                else if (attr.value("k").startsWith("ONEWAY", Qt::CaseInsensitive)) {}
                else if (attr.value("k").startsWith("MAXSPEED", Qt::CaseInsensitive)) {}
                else if (attr.value("k").startsWith("SURFACE", Qt::CaseInsensitive))  {}
                else if (attr.value("k").startsWith("BRIDGE", Qt::CaseInsensitive)) {
                    if (way) tway->val2 = 7;
                }
                else if (attr.value("k").startsWith("TUNNEL", Qt::CaseInsensitive)) {
                    if (way) tway->val2 = 6;
                }
                //miejsca
                else if (attr.value("k").startsWith("AMENITY", Qt::CaseInsensitive)) {
                    //System.out.println(attr.getValue("v").toUpperCase());
                    //if(node) {
                    //    uuu++;
                    //    dane.miejsca.add(new Miejsce(tnode.lat, tnode.lon, attr.getValue("v").toUpperCase().replace("_", "")));
                    //}
                }
                //bariery
                else if (attr.value("k").startsWith("BARRIER", Qt::CaseInsensitive)) {}
                //las
                else if (attr.value("k").startsWith("WOOD", Qt::CaseInsensitive)) {}
                //sport
                else if (attr.value("k").startsWith("SPORT", Qt::CaseInsensitive)) {}
                else {
                    //inne budynki 
                    if (attr.value("k").startsWith("BUILDING", Qt::CaseInsensitive)) {
                        if (way) tway->type = (short) OSMFeatures::LIST["BUILDING_YES"];
                        iii++;
                    }
                    //jakies gowna co nie chce
                    //if(attr.getValue("k").toUpperCase().startsWith("SOURCE", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("CREATED", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("EWMAPA", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("BUILDING:", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("REF", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("WIKI", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("ACCESS", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("NOTE", 0)) return;
                    //if(attr.getValue("k").toUpperCase().startsWith("TRACKTYPE", 0)) return;
                    //inne -> enum
                    QString fname = "";
                    fname += attr.value("k").toString() + "_" + attr.value("v").toString();
                    fname = fname.toUpper();
                    if (OSMFeatures::LIST[fname.toStdString()] != 0) {
                        if (way) tway->type = (short) OSMFeatures::LIST[fname.toStdString()];
                        if (node) tnode->type = (short) OSMFeatures::LIST[fname.toStdString()];
                        iii++; //System.out.println(fname);
                    } else {
                        //if(!fname.startsWith("source", Qt::CaseInsensitive)
                        //    && !fname.startsWith("created", Qt::CaseInsensitive)
                        //    )
                        //    qDebug() << "fail: " << fname;
                    }
                }
            } else if (name.toUpper() == ("BOUNDS")) {
                bounds = true;
                //minlat = attr.value("minlat").toFloat();
                //minlon = attr.value("minlon").toFloat();
                //maxlat = attr.value("maxlat").toFloat();
                //maxlon = attr.value("maxlon").toFloat();
                //qDebug() << "minmax";
                //qDebug() << minlat << " " << maxlat;
                ///qDebug() << minlon << " " << maxlon;
            }
        } else if (reader.isEndElement()) {
            name = reader.name().toString();
            if (name.toUpper() == ("NODE")) {
                nodes[tnode->id] = tnode;
                node = false;
            }
            if (name.toUpper() == ("WAY")) {
                //qDebug() << Features::LAYER[tway->type];
                int tlayer = OSMFeatures::LAYER[tway->type];
                if(tlayer > 9) tlayer = 9;
                if(tway->val2==7) 
                    ways[9].push_back(tway);
                else
                    ways[9-tlayer].push_back(tway);
                way = false;
            }
            if (name.toUpper() == ("BOUNDS")) {
                bounds = false;
            }
        } else if (reader.isCharacters()) {

        }
        
        reader.readNext();
    }
    qDebug() << "node/way: " << inode << "/" << iway;
}