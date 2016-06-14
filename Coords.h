#ifndef COORDS_H
#define	COORDS_H

#include <QString>
#include <unordered_map>

class OglObj;
class GLUU;
class TextObj;

class Coords {
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
    
    Coords();
    Coords(QString path);
    virtual ~Coords();
    virtual void render(GLUU* gluu, float * playerT, float* playerW, float playerRot);
protected:
    OglObj* simpleMarkerObj = NULL;
private:

};

#endif	/* COORDS_H */

