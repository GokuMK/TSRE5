#ifndef COORDSMKR_H
#define	COORDSMKR_H

#include <QString>
#include <unordered_map>

class OglObj;
class GLUU;
class TextObj;

class CoordsMkr {
public:
    struct Marker{
        QString name;
        float lat;
        float lon;
        int type;
        int tileX;
        int tileZ;
        int x;
        int y;
        int z;
    };
    std::vector<Marker> markerList;
    std::unordered_map<std::string, TextObj*> nameGl;
    
    bool loaded = false;
    
    CoordsMkr(QString path);
    virtual ~CoordsMkr();
    void render(GLUU* gluu, float * playerT, float* playerW, float playerRot);
private:
    OglObj* simpleMarkerObj = NULL;
};

#endif	/* COORDSMKR_H */

