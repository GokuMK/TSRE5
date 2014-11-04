#ifndef SFILE_H
#define	SFILE_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QString>

class SFile {
public:
    SFile();
    SFile(QString pathid, QString name, QString texp );
    SFile(const SFile& orig);
    virtual ~SFile();
    
    QString pathid;
    QString sciezka;
    QString nazwa;
    QString texPath;
    int isinit = 0;
    int loaded = 0;
    int texloaded = 0;
    int ref = 0;

    struct fshader {
        QString name;
        float alpha;
    };
    
    struct czes {
        int iloscv;
        int prim_state_idx;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
    };

    struct sub {
        int iloscc;
        czes* czesci;
    };

    struct dist {
        int ilosch;
        int* hierarchia;
        int iloscs;
        sub* subobiekty;
    };

    struct matrt {
        float param[16];
        float fixed[16];
        bool isFixed = false;
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
    };
    
    // zmienne
    struct VBO {
        int w[];
    };
    VBO vbo[];
    punlist tpoints;
    // zmienne
    int iloscd;
    int iloscm;
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
    
    int ishaders;
    fshader* shader;
    float size;
    
    void Load(QString path);
    void render();
    float* getPmatrix(float* pmatrix, int matrix);
    void getSize();
private:

};

#endif	/* SFILE_H */

