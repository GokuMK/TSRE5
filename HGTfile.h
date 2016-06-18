#ifndef HGTFILE_H
#define	HGTFILE_H

#include <QString>

class QImage;

class HGTfile {
public:
    HGTfile();
    virtual ~HGTfile();
    bool load(int lat, int lon);
    void draw(QImage* &image);
    bool isLoaded();
    float getHeight(float lat, float lon);
private:
    short int** terrainData;
    int rowSize;
    bool loaded = false;
};

#endif	/* HGTFILE_H */

