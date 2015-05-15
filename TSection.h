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
    int getLineBufferSize();
    void drawSection(float* &ptr, float* matrix);
    void setDrawPosition(float metry);
    float getAngle();
    Vector3f *getDrawPosition(float metry);
    void setCamPosition(float metry, float ob);
    void setCamRotation(float metry, float ob);

private:

};

#endif	/* TSECTION_H */

