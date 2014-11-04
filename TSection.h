#ifndef TSECTION_H
#define	TSECTION_H
#include "Vector3f.h"

class TSection {
public:
    int id;
    int type;
    float val1;
    float size;
    float radius;
    float angle;
    
    TSection(int index);
    TSection(const TSection& orig);
    virtual ~TSection();
    float getDlugosc();
    void drawSection();
    void setDrawPosition(float metry);
    Vector3f *getDrawPosition(float metry);
    void setCamPosition(float metry, float ob);
    void setCamRotation(float metry, float ob);

private:

};

#endif	/* TSECTION_H */

