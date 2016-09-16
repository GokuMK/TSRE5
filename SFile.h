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

class SFile {
public:

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
    };

    struct sub {
        int iloscc;
        czes* czesci;
        QOpenGLBuffer VBO;
        QOpenGLVertexArrayObject VAO;
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
    float bound[6];
    
    SFile();
    SFile(QString pathid, QString name, QString texp );
    SFile(const SFile& orig);
    virtual ~SFile();
    void load();
    void render();
    void getSize();
private:
    void loadSd();
    float* getPmatrix(float* pmatrix, int matrix);
};

#endif	/* SFILE_H */

