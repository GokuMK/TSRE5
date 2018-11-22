/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef URIIMAGEDRAWTHREAD_H
#define	URIIMAGEDRAWTHREAD_H

#include <QObject>
#include <QThread>
#include <QVector>
#include "MapDataUrlImage.h"

class QImage;
class LatitudeLongitudeCoordinate;

class UriImageDrawThread : public QThread {
     Q_OBJECT

public:
    LatitudeLongitudeCoordinate *llpoint00;
    LatitudeLongitudeCoordinate *llpoint01;
    LatitudeLongitudeCoordinate *llpoint10;
    LatitudeLongitudeCoordinate *llpoint11;
    QImage *myImage;
    QVector<MapDataUrlImage::MapImage> *images;
    int wmin = 0;
    int wmax = 0;
    
    UriImageDrawThread() = default;
    virtual ~UriImageDrawThread() = default;
    void run();
private:

};

#endif	/* URIIMAGEDRAWTHREAD_H */

