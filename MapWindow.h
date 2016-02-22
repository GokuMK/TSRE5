#ifndef MAPWINDOW_H
#define	MAPWINDOW_H

#include <QtWidgets>
#include "OSM.h"

class QNetworkReply;
class QImage;
class IghCoordinate;
class LatitudeLongitudeCoordinate;
class PreciseTileCoordinate;
            
class MapWindow : public QWidget {
    Q_OBJECT
public:
    OSM dane;
    int tileX;
    int tileZ;
    
    static std::unordered_map<int, QImage*> mapTileImages;
    MapWindow();
    virtual ~MapWindow();
    void load(QByteArray* data = NULL);
    void get();

public slots:
    void isData(QNetworkReply* r);
    void loadOSM();

private:
    QLabel* imageLabel;
    float minlat, minlon, maxlat, maxlon;
    
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* minLatlon = NULL;
    LatitudeLongitudeCoordinate* maxLatlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
};

#endif	/* MAPWINDOW_H */

