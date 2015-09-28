#ifndef TERRAIN_H
#define	TERRAIN_H
#include <QString>
#include "GLUU.h"
#include "TFile.h"
#include "Vector3f.h"
#include "OglObj.h"

class Brush;

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
    void save();
    void refresh();
    bool isModified();
    void setModified(bool value);
    void paintTexture(Brush* brush, int x, int z, float posx, float posz);
    void setTexture(Brush* brush, int x, int z, float posx, float posz);
    int getTexture(int x, int z, float posx, float posz);
    void render(float lodx, float lodz, float * playerT, float* playerW, float* target, float fov);

private:
    unsigned char **fData;
    bool jestF;
    bool isOgl;
    bool modified;
    float mojex;
    float mojez;
    QString texturepath;
    Vector3f **vertexData;//[257][257];
    Vector3f **normalData;//[257][257];
    bool hidden[256];
    int texid[256];
    bool texModified[256];
    QOpenGLBuffer VBO[256];
    QOpenGLVertexArrayObject VAO[256];

    OglObj lines;
    OglObj mlines;
    OglObj slines;
    OglObj water[256];
    //QOpenGLBuffer wVBO[256];
    //QOpenGLVertexArrayObject wVAO[256];
    //bool jestW[256];
    int wTexid = -1;
    TFile* tfile;
    
    void saveRAW(QString name);
    bool readRAW(QString fSfile);
    bool readF(QString fSfile);
    void vertexInit();
    void normalInit();
    void oglInit();
    void rotateTex(int idx);
    void convertTexToDefaultCoords(int idx);
    void paintTextureOnTile(Brush* brush, int y, int u, float x, float z);
    void reloadLines();
};

#endif	/* TERRAIN_H */

