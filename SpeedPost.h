#ifndef SPEEDPOST_H
#define	SPEEDPOST_H

#include <QString>

class FileBuffer;

class SpeedPost {
public:
    QString name;
    QString speedResumeSignShape;
    QString speedSignShapeName;
    int speedSignShapeCount;
    float* speedSignShape;
    QString speedDigitTex;
    float speedTextSize[3];
    
    QString milepostShapeName;
    int milepostShapeCount;
    float* milepostShape;
    QString milepostDigitTex;
    float milepostTextSize[3];
    
    SpeedPost();
    virtual ~SpeedPost();
    void set(QString sh, FileBuffer* data);
private:

};

#endif	/* SPEEDPOST_H */

