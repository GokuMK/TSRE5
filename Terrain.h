#ifndef TERRAIN_H
#define	TERRAIN_H
#include <QString>
#include "GLUU.h"
#include "TFile.h"
#include "Vector3f.h"
#include "OglObj.h"

class Terrain {
public:
    int loaded;
    float **terrainData;
    bool inUse = true;
    Terrain(float x, float y);
    Terrain(const Terrain& orig);
    virtual ~Terrain();
    static void saveEmpty(int x, int y);
    static QString getTileName(int x, int y);
    bool readRAW(QString fSfile);
    bool readF(QString fSfile);
    void render(float lodx, float lodz, float * playerT, float* playerW, float* target, float fov);
    void vertexInit();
    void normalInit();
    void oglInit();
private:
    unsigned char **fData;
    bool jestF;
    bool isOgl;
    float mojex;
    float mojez;
    QString texturepath;
    Vector3f **vertexData;//[257][257];
    Vector3f **normalData;//[257][257];
    int texid[256];
    QOpenGLBuffer VBO[256];
    QOpenGLVertexArrayObject VAO[256];
    
    OglObj lines;
    OglObj water[256];
    //QOpenGLBuffer wVBO[256];
    //QOpenGLVertexArrayObject wVAO[256];
    //bool jestW[256];
    int wTexid = -1;
    TFile* tfile;
};

#endif	/* TERRAIN_H */

