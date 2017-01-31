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
#include <unordered_map>
#include <vector>

class QNetworkReply;
class QImage;
class IghCoordinate;
class LatitudeLongitudeCoordinate;
class PreciseTileCoordinate;
class QPushButton;
class MapData;

class MapWindow : public QDialog {
    Q_OBJECT
public:
    int tileX;
    int tileZ;
    
    static std::unordered_map<int, QImage*> mapTileImages;
    static bool LoadMapFromDisk(int x, int z);
    MapWindow();
    virtual ~MapWindow();
    bool ok = false;

    int exec();
    
public slots:
    void load();
    void saveToDisk();
    void colorComboActivated(QString val);
    void reload();
    void isStatusInfo(QString val);

private:
    QVector<MapData*> mapServices;
    MapData *dane = NULL;
    QLabel* imageLabel;
    float minlat, minlon, maxlat, maxlon;
    bool invert = false;
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* minLatlon = NULL;
    LatitudeLongitudeCoordinate* maxLatlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    QPushButton *loadButton = NULL;
    QComboBox mapServicesCombo;
};

#endif	/* MAPWINDOW_H */

