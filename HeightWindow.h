/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef HEIGHTWINDOW_H
#define	HEIGHTWINDOW_H

#include <QtWidgets>
#include <unordered_map>
#include <QMap>
#include <QPair>

class QNetworkReply;
class QImage;
class IghCoordinate;
class LatitudeLongitudeCoordinate;
class PreciseTileCoordinate;
class HGTfile;

class HeightWindow : public QDialog {
    Q_OBJECT
public:
    HeightWindow();
    virtual ~HeightWindow();
    int tileX;
    int tileZ;
    int terrainResolution;
    int terrainSize;
    bool ok = false;
    float** terrainData = NULL;
    
    static void CheckForMissingGeodataFiles(QMap<int, QPair<int, int>*> &tileList);
    static std::unordered_map<int, HGTfile*> hqtFiles;
    int exec();
    
public slots:
    void load(bool gui = true);
    
private:
    QLabel* imageLabel;
    float minlat, minlon, maxlat, maxlon;
    float minVal = 999;
    float maxVal = -999;
    
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* mLatlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    
    void drawTile(QImage* &image, bool gui);
};

#endif	/* HEIGHTWINDOW_H */

