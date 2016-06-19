#ifndef COORDS_H
#define	COORDS_H

#include <QString>
#include <unordered_map>

class OglObj;
class GLUU;
class TextObj;

class Coords {
public:
    struct Marker {
        QString name;
        float lat;
        float lon;
        int type;
        OglObj* oglObj = NULL;
        std::vector<int> tileX;
        std::vector<int> tileZ;
        std::vector<int> x;
        std::vector<int> y;
        std::vector<int> z;
    };
    //struct Line {
//
    //};
    
    std::vector<Marker> markerList;
    std::unordered_map<std::string, TextObj*> nameGl;
    //std::vector<Line> lineList;
    
    bool loaded = false;
    
    Coords();
    Coords(QString path);
    virtual ~Coords();
    virtual void render(GLUU* gluu, float * playerT, float* playerW, float playerRot);
protected:
    OglObj* simpleMarkerObjP = NULL;
    OglObj* simpleMarkerObjL = NULL;
private:

};

#endif	/* COORDS_H */

