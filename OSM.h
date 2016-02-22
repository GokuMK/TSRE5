#ifndef OSM_H
#define	OSM_H

#include <unordered_map>
#include <vector>

class IghCoordinate;
class LatitudeLongitudeCoordinate;
class PreciseTileCoordinate;
class QImage;
class QColor;
class QPen;
class QPainter;
class QBrush;

class Node {
    
public:
    int64_t id;
    float lat;
    float lon;
    unsigned short type;
    unsigned char val1;
    unsigned char val2;
     
    Node(){
         //id = 0;
         lon = 0;
         lat = 0;
         type = 0;
         val1 = 0; 
         val2 = 0;
    }   
    
    Node(int64_t id1, float lat1, float lon1){
         id = id1;
         lon = lon1;
         lat = lat1;
         type = 0;
         val1 = 0; 
         val2 = 0;
    }
};

class Way {
public:
    int64_t id;
    unsigned short type;
    unsigned char val1;
    unsigned char val2;
    std::vector<int64_t> ref;
    std::vector<float> lat;
    std::vector<float> lon;
    
    Way(int64_t id1){
         id = id1;
         type = 0;
         val1 = 0;
         val2 = 0;
    }
    
    Way(){
         id = 0;
         type = 0;
         val1 = 0;
         val2 = 0;
    }    
};

class OSM {
public:
    float maxlat, minlat, maxlon, minlon;
    float height, width;
    int tileX, tileZ;
    std::unordered_map<int64_t,Node*> nodes;
    std::vector<Way*> ways[10];
    OSM();
    virtual ~OSM();
    void draw(QImage* myImage);
    int rX(float tlon);
    int rY(float tlat);
    void r(int &x, int &y, float lat, float lon);

private:
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* latlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    QColor* color;
    QColor* roadBorder;
    QPen* p;
    QPainter* gg;
    QBrush* brush;
    void setColor(int r, int g, int b);
    void setColor(QColor* color);
    void setPenSettings(QPen* pen);
};

#endif	/* OSM_H */

