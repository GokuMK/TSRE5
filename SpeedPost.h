#ifndef SPEEDPOST_H
#define	SPEEDPOST_H

#include <QString>

class FileBuffer;

class SpeedPost {
public:
    QString name;
    QString speedResumeSignShapeName;
    int speedResumeSignShapeCount = 0;
    float* speedResumeSignShape;

    QString speedWarningSignShapeName;
    int speedWarningSignShapeCount = 0;
    float* speedWarningSignShape;
    
    QString speedSignShapeName;
    int speedSignShapeCount = 0;
    float* speedSignShape;
    QString speedDigitTex;
    float speedTextSize[3];
    
    QString milepostShapeName;
    int milepostShapeCount = 0;
    float* milepostShape;
    QString milepostDigitTex;
    float milepostTextSize[3];
    
    SpeedPost();
    virtual ~SpeedPost();
    void set(QString sh, FileBuffer* data);
private:

};

#endif	/* SPEEDPOST_H */

