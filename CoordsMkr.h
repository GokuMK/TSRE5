#ifndef COORDSMKR_H
#define	COORDSMKR_H

#include <QString>

class OglObj;
class GLUU;

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
    bool loaded = false;
    
    CoordsMkr(QString path);
    virtual ~CoordsMkr();
    void render(GLUU* gluu, float * playerT, float* playerW);
private:
    OglObj* simpleMarkerObj = NULL;
};

#endif	/* COORDSMKR_H */

