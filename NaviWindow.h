#ifndef NAVIWINDOW_H
#define	NAVIWINDOW_H

#include <QtWidgets>

class PreciseTileCoordinate;
class IghCoordinate;
class LatitudeLongitudeCoordinate;

class NaviWindow : public QWidget {
    Q_OBJECT
public:
    NaviWindow();
    virtual ~NaviWindow();
    
public slots:
    void jumpTileSelected();  
    void naviInfo(int all, int hidden);
    void posInfo(PreciseTileCoordinate* coords);
    void latLonChanged(QString val);
    void xyChanged(QString val);

signals:
    void jumpTo(PreciseTileCoordinate*);

private:
    QComboBox markerFiles;
    QComboBox markerList;
    QLineEdit txBox;
    QLineEdit tyBox;
    QLineEdit latBox;
    QLineEdit lonBox;
    QLineEdit xBox;
    QLineEdit yBox;
    QLineEdit zBox;
    QLabel tileInfo;
    QString lastEdit = "";
    
    int lastTX = 0; 
    int lastTZ = 0;
    int lastX = 0; 
    int lastY = 0;
    int lastZ = 0;
    int objCount = 0;
    int objHidden = 0;
    
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* latlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    
    QString jumpType = "";
};

#endif	/* NAVIWINDOW_H */

