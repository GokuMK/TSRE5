#ifndef HGTFILE_H
#define	HGTFILE_H

#include <QString>

class QImage;

class HGTfile {
public:
    HGTfile();
    virtual ~HGTfile();
    void load(int lat, int lon);
    void draw(QImage* &image);
    float getHeight(float lat, float lon);
private:
    short int** terrainData;
    int rowSize;
    static QString filePath;
};

#endif	/* HGTFILE_H */

