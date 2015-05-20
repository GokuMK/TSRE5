#ifndef TRACKSHAPE_H
#define	TRACKSHAPE_H

#include <QString>

class TrackShape {
public:
    struct SectionIdx{
        int n = 0;
        float pos[3];
        float rotDeg = 0;
        unsigned short sect[12];
    };
    
    QString filename;
    int id;
    int numpaths = 0;
    int mainroute = -1;
    int clearancedist = -1;
    bool crossovershape = false;
    bool tunnelshape = false;
    bool roadshape = false;
    bool manualjunctionshape = false;
    bool dyntrack = false;
    SectionIdx *path;
    int xoverpts = 0;
    float *xoverpt;
    
 //      } else if (sh == ("xoverpts")) {    
        
    TrackShape();
    TrackShape(int t);
    virtual ~TrackShape();
    //void getShapePath();
private:

};

#endif	/* TRACKSHAPE_H */

