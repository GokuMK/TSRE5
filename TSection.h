/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TSECTION_H
#define	TSECTION_H

#include <QVector>

class Vector3f;
class QTextStream;
class FileBuffer;

class TSection {
public:
    int id;
    int type;
    float val1;
    float size;
    float radius;
    float angle;
    
    TSection();
    TSection(int index);
    TSection(int index, int t, float a, float r);
    TSection(const TSection& orig);
    virtual ~TSection();
    float getDlugosc();
    int getLineBufferSize(int pointSize, int step = 0);
    void drawSection(float* &ptr, float* matrix, float height);
    void drawSection(float* &ptr, float* matrix, float height, int idx, int vidx);
    void drawSection(float* &ptr, float* matrix, float height, int idx, int vidx, float offset, int step);
    void getPoints(QVector<float> &ptr, float* matrix);
    void setDrawPosition(float metry);
    float getAngle();
    int getHash();
    void getDrawPosition(Vector3f *out, float metry);
    void setCamPosition(float metry, float ob);
    void setCamRotation(float metry, float ob);
    float getDrawAngle(float metry);
    void saveToStream(QTextStream &out);
    void loadUtf16Data(FileBuffer *data);
private:

};

#endif	/* TSECTION_H */

