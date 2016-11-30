/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef MAPWINDOW_H
#define	MAPWINDOW_H

#include <QtWidgets>
#include "OSM.h"

class QNetworkReply;
class QImage;
class IghCoordinate;
class LatitudeLongitudeCoordinate;
class PreciseTileCoordinate;
class QPushButton;

class MapWindow : public QDialog {
    Q_OBJECT
public:
    OSM dane;
    int tileX;
    int tileZ;
    
    static std::unordered_map<int, QImage*> mapTileImages;
    MapWindow();
    virtual ~MapWindow();
    void load(QByteArray* data = NULL);
    bool ok = false;
    void get(LatitudeLongitudeCoordinate* min, LatitudeLongitudeCoordinate* max);
    int exec();
    
public slots:
    void isData(QNetworkReply* r);
    void loadOSM();
    void colorComboActivated(QString val);

private:
    QLabel* imageLabel;
    float minlat, minlon, maxlat, maxlon;
    bool invert = false;
    int loadCount;
    int totalLoadCount;
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* minLatlon = NULL;
    LatitudeLongitudeCoordinate* maxLatlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    QPushButton *loadButton = NULL;
    void reload();
};

#endif	/* MAPWINDOW_H */

