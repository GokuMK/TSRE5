#ifndef NAVIWINDOW_H
#define	NAVIWINDOW_H

#include <QtWidgets>
#include <unordered_map>

class PreciseTileCoordinate;
class IghCoordinate;
class LatitudeLongitudeCoordinate;
class CoordsMkr;

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
    void mkrList(std::unordered_map<std::string, CoordsMkr*> list);
    void mkrFilesSelected(QString item);
    void mkrListSelected(QString item);

signals:
    void jumpTo(PreciseTileCoordinate*);
    void sendMsg(QString name);
    void sendMsg(QString name, bool val);
    void sendMsg(QString name, int val);
    void sendMsg(QString name, float val);
    void sendMsg(QString name, QString val);
    
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
    std::unordered_map<std::string, CoordsMkr*> mkrFiles;
    std::unordered_map<std::string, LatitudeLongitudeCoordinate*> mkrPlaces;
    QString jumpType = "";
};

#endif	/* NAVIWINDOW_H */

