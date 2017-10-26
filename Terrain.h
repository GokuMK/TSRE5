/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TERRAIN_H
#define	TERRAIN_H
#include <QString>
#include "GLUU.h"
#include "TFile.h"
#include "Vector3f.h"
#include "OglObj.h"
#include "GameObj.h"

class Brush;

class Terrain : public GameObj {
    Q_OBJECT
public:
    static Brush* DefaultBrush;
    
    int loaded;
    float **terrainData;
    bool inUse = true;
    bool showBlob = false;
    float mojex;
    float mojez;
    Terrain(float x, float y);
    Terrain(const Terrain& orig);
    virtual ~Terrain();
    static void saveEmpty(int x, int y);
    static QString getTileName(int x, int y);
    QString getTileName();
    void save();
    void refresh();
    bool isModified();
    void setModified(bool value);
    void paintTexture(Brush* brush, int x, int z, float posx, float posz);
    void lockTexture(Brush* brush, int x, int z, float posx, float posz);
    void setTexture(Brush* brush, int x, int z, float posx, float posz, bool autoRot = true);
    void setTexture(Brush* brush, int u, bool autoRot = false);
    void toggleWaterDraw(int x, int z, float posx, float posz);
    void setWaterLevelGui();
    void toggleDraw(int x, int z, float posx, float posz);
    void setWaterDraw();
    void setDraw();
    void hideWaterDraw();
    void hideDraw();
    void toggleDraw();
    void toggleWaterDraw();
    void setDrawAdjacent();
    void rotatePatchTexture();
    void mirrorXPatchTexture();
    void mirrorYPatchTexture();
    float getPatchScaleTex();
    void scalePatchTexCoords(float val);
    QString getPatchTexTransformString();
    void setPatchTexTransform(QString val);
    void removeAllGaps();
    void toggleGaps(int x, int z, float posx, float posz);
    void setErrorBias(int x, int z, float val);
    float getErrorBias();
    void setErrorBias(float val);
    void setTileBlob();
    void makeTextureFromMap();
    void removeTextureFromMap();
    void fillHeightMap(float *data);
    int getTexture(int x, int z, float posx, float posz);
    int getSelectedPathId();
    int getSelectedShaderId();
    bool select(int value);
    bool select(int value, bool oneMore);
    bool unselect();
    void resetPatchTexCoords();
    void pushContextMenuActions(QMenu *menu);
    void render(float lodx, float lodz, float * playerT, float* playerW, float* target, float fov, int selectionColor);
    void renderWater(float lodx, float lodz, float * playerT, float* playerW, float* target, float fov, int layer, int selectionColor = 0);

public slots:
    void menuToggleWater();
    void menuToggleDraw();
    void menuPutTexture();
    
private:
    unsigned char **fData;
    bool jestF = false;
    bool modifiedF;
    bool isOgl;
    bool modified;
    QString texturepath;
    Vector3f **vertexData;//[257][257];
    Vector3f **normalData;//[257][257];
    bool hidden[256];
    bool uniqueTex[256];
    int texid[256];
    int texid2[256];
    bool texModified[256];
    bool texLocked[256];
    bool selectedPatchs[256];
    QOpenGLBuffer *VBO = NULL;
    QOpenGLVertexArrayObject *VAO = NULL;

    OglObj lines;
    OglObj mlines;
    OglObj slines;
    OglObj ulines;
    OglObj lockedlines;
    OglObj selectedlines;
    
    struct WaterTile {
        OglObj w[256];
    };
    QMap<int, WaterTile*> water;
    //OglObj water[256];
    
    
    OglObj terrainBlob;
    //QOpenGLBuffer wVBO[256];
    //QOpenGLVertexArrayObject wVAO[256];
    //bool jestW[256];
    int wTexid = -1;
    TFile* tfile;
    //int selectedPathId = -1;
    
    void saveRAW(QString name);
    bool readRAW(QString fSfile);
    bool readF(QString fSfile);
    void saveF(QString name);
    void newF();
    void vertexInit();
    void normalInit();
    void oglInit();
    void initBlob();
    void rotateTex(int idx);
    void mirrorXTex(int idx);
    void mirrorYTex(int idx);
    void scaleTex(int idx, float val);
    float getScaleTex(int idx);
    void convertTexToDefaultCoords(int idx);
    void paintTextureOnTile(Brush* brush, int y, int u, float x, float z);
    void reloadLines();
};

#endif	/* TERRAIN_H */

