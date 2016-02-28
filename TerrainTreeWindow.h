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

