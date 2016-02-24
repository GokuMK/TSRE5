#ifndef HEIGHTWINDOW_H
#define	HEIGHTWINDOW_H

#include <QtWidgets>
#include <unordered_map>

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
    bool ok = false;
    float** terrainData = NULL;
    
    static std::unordered_map<int, HGTfile*> hqtFiles;
    int exec();
    
public slots:
    void load();
    
private:
    QLabel* imageLabel;
    float minlat, minlon, maxlat, maxlon;
    float minVal = 999;
    float maxVal = -999;
    
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* mLatlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    
    void drawTile(QImage* &image);
};

#endif	/* HEIGHTWINDOW_H */

