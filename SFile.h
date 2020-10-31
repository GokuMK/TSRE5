/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SFILE_H
#define	SFILE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QString>
#include <QVector>

class FileBuffer;
class ShapeTextureInfo;
class ShapeHierarchyInfo;
class ContentHierarchyInfo;

class SFile {
public:

    struct SObjHeader {
        QVector<int> geometryNodeMap;
    };
    
    struct fshader {
        QString name;
        float alpha;
    };
    
    struct czes {
        int iloscv;
        int prim_state_idx;
        //QOpenGLBuffer VBO;
        //QOpenGLVertexArrayObject VAO;
        int offset;
        int* idx;
        bool enabled = true;
    };

    struct sub {
        int iloscc;
        SObjHeader header;
        czes* czesci;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };

    struct dist {
        int levelSelection;
        int ilosch;
        int* hierarchia;
        int iloscs;
        sub* subobiekty;
    };

    struct matrt {
        QString name;
        float param[16];
        float fixed[16];
        bool isFixed = false;
        long long int hash = 0;
    };

    struct primst {
        int vtx_state, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8;
    };

    struct text {
        int image, arg1, arg2, arg3;
    };

    struct vtxs {
        int arg1, matrix, arg2, arg3, arg4;
    };

    struct imgs {
        QString name;
        int tex;
        int texAddr = -1;
    };

    struct fpoint {
        float x, y, z;
    };

    struct punlist {
        fpoint* points;
        fpoint* uv_points;
        fpoint* normals;
        int ipoints;
        int iuv_points;
        int inormals;
    };
    
    // zmienne
    //struct VBO {
    //    int w[];
    //};
    
    struct EsdBoundingBox {
        float shape[6];
        float rotation[3];
        float translation[3];
        
        EsdBoundingBox(){
            rotation[0] = 0;
            rotation[1] = 0;
            rotation[2] = 0;
            translation[0] = 0;
            translation[1] = 0;
            translation[2] = 0;
        }
    };
    
    struct AnimFrameId {
        unsigned int id1 = 0;
        unsigned int id2 = 0;
        float offset = 0;
    };
    
    struct AnimNode {
        struct TcbKey {
            int frame;
            float quat[4];
            float param[5];
        };
        struct SlerpRot {
            int frame;
            float quat[4];
        };
        struct LinearKey {
            int frame;
            float pos[3];
        };
        QVector<TcbKey> tcbKey;
        QVector<AnimFrameId> tcbId;
        QVector<SlerpRot> slerpRot;
        QVector<AnimFrameId> slerpbId;
        QVector<LinearKey> linearKey;
        QVector<AnimFrameId> linearId;
    };
    
    struct Animation {
        int frames;
        float fps;
        QVector<AnimNode> node;
        void loadC(FileBuffer* data, int length);
        void loadX(FileBuffer* data);
    };
    
    QVector<Animation> animations;
    bool animated = false;
    
    QString pathid;
    QString sciezka;
    QString nazwa;
    QString texPath;
    QString sdName;
    int isinit = 0;
    int loaded = 0;
    bool loadedSd = false;
    int texloaded = 0;
    int ref = 0;
    int esdDetailLevel = -1;
    int esdAlternativeTexture = -1;
    QVector<EsdBoundingBox> esdBoundingBox;
    
    //VBO vbo[];
    punlist tpoints;
    // zmienne
    int iloscd;
    int iloscm = 0;
    matrt* macierz;
    int ilosci;
    imgs* image;
    int ilosct;
    text* texture;
    int iloscv;
    vtxs* vtxstate;
    int iloscps;
    primst* primstate;
    dist* distancelevel;
    int currentDistanceLevel = 0;
    
    int ishaders;
    fshader* shader;
    float size;
    float bound[6];
    
    SFile();
    SFile(QString pathid, QString name, QString texp );
    SFile(const SFile& orig);
    virtual ~SFile();
    void load();
    void enablePart(unsigned int uid, unsigned int stateId = 0);
    void disablePart(unsigned int uid, unsigned int stateId = 0);
    void updateSim(float deltaTime, unsigned int stateId = 0);
    void render();
    void render(int selectionColor, unsigned int stateId);
    void getSize();
    bool getBoxPoints(QVector<float> &points);
    void getFloorBorderLinePoints(float *&punkty);
    bool isSnapable();
    void addSnapablePoints(QVector<float> &out);
    void reload();
    unsigned int newState();
    void setAnimated(unsigned int stateId, bool animated);
    void setEnabledSubObjs(unsigned int stateId, unsigned int enabledSubObjs);
    void setCurrentDistanceLevel(unsigned int stateId, int level);
    void enableSubObjByName(unsigned int stateId, QString name, bool val);
    void enableSubObjByNameQueue(unsigned int stateId, QString name, bool val);
    void fillShapeTextureInfo(QHash<int, ShapeTextureInfo*> &list, unsigned int stateId = 0);
    void fillShapeHierarchyInfo(ShapeHierarchyInfo* info, unsigned int stateId = 0);
    void fillContentHierarchyInfo(QVector<ContentHierarchyInfo*> &list, int parent);
private:
    struct State {
        bool animated = false; 
        int enabledSubObjs = 0xFFFFFFFF;
        float frameCount = 0;
        unsigned long long int lastTime = 0;
        QMap<QString, bool> enableSubObjQueue;
        int distanceLevel = 0;
    };
    QVector<State> state;
    
    void loadSd();
    float* getPmatrix(int currentDlevel, float* pmatrix, int matrix);
    float* getPmatrixAnimated(int currentDlevel, float* pmatrix, int matrix, float frame);
    void buildFrameIds();
    bool snapable = false;
};

#endif	/* SFILE_H */

