/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TERRAINTREEWINDOW_H
#define	TERRAINTREEWINDOW_H

#include <QtWidgets>

class QImage;
class IghCoordinate;
class LatitudeLongitudeCoordinate;
class PreciseTileCoordinate;

class TerrainTreeWindow : public QDialog {
    Q_OBJECT
public:
    TerrainTreeWindow();
    virtual ~TerrainTreeWindow();
private:
    QLabel* imageLabel;
    QImage* myImage;
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* minLatlon = NULL;
    LatitudeLongitudeCoordinate* maxLatlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
};

#endif	/* TERRAINTREEWINDOW_H */

