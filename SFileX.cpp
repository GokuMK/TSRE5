/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SFileX.h"
#include "SFile.h"
#include <QDebug>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QtCore>
#include "ParserX.h"
#include "GLUU.h"

void SFileX::odczytajshaders(FileBuffer* bufor, SFile* pliks) {
    pliks->ishaders = ParserX::GetNumber(bufor);

    pliks->shader = new SFile::fshader[pliks->ishaders];
    for (int i = 0; i < pliks->ishaders; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        bufor->off += 2;
        pliks->shader[i].name = ParserX::GetString(bufor).toLower();
        if(pliks->shader[i].name == "texdiff") 
            pliks->shader[i].alpha = 1;
        else 
            pliks->shader[i].alpha = 0;
        //qDebug() << pliks->shaderName[i];
        //console.log(pliks->image[i].name);
    }
}

//-----------------------------------
//Oczytanie i zapisanie sekcji points
//-----------------------------------

void SFileX::odczytajpunkty(FileBuffer* bufor, SFile* pliks) {
    pliks->tpoints.ipoints = ParserX::GetNumber(bufor);
    //qDebug() << ilosc;
    pliks->tpoints.points = new SFile::fpoint[pliks->tpoints.ipoints + 1];
    for (int i = 0; i < pliks->tpoints.ipoints; i++) {
        pliks->tpoints.points[i].x = ParserX::GetNumber(bufor);
        pliks->tpoints.points[i].y = ParserX::GetNumber(bufor);
        pliks->tpoints.points[i].z = ParserX::GetNumber(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji uv_points
//-----------------------------------

void SFileX::odczytajuvpunkty(FileBuffer* bufor, SFile* pliks) {
    int ilosc = ParserX::GetNumber(bufor);
    pliks->tpoints.uv_points = new SFile::fpoint[ ilosc + 1];
    for (int i = 0; i < ilosc; i++) {
        pliks->tpoints.uv_points[i].x = ParserX::GetNumber(bufor);
        pliks->tpoints.uv_points[i].y = ParserX::GetNumber(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji normals
//-----------------------------------

void SFileX::odczytajnormalne(FileBuffer* bufor, SFile* pliks) {
    int ilosc = ParserX::GetNumber(bufor);
    pliks->tpoints.normals = new SFile::fpoint[ilosc + 1];
    for (int i = 0; i < ilosc; i++) {
        pliks->tpoints.normals[i].x = ParserX::GetNumber(bufor);
        pliks->tpoints.normals[i].y = ParserX::GetNumber(bufor);
        pliks->tpoints.normals[i].z = ParserX::GetNumber(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji matrices
//-----------------------------------

void SFileX::odczytajmatrices(FileBuffer* bufor, SFile* pliks) {
    pliks->iloscm = ParserX::GetNumber(bufor);
    pliks->macierz = new SFile::matrt[pliks->iloscm + 1];

    //System.out.println("iloscm: "+pliks->iloscm);
    for (int i = 0; i < pliks->iloscm; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        //pliks->macierz[i] = new float[16];
        for (int j = 0; j < 16; j++) {
            if (j == 3 || j == 7 || j == 11) {
                pliks->macierz[i].param[j] = 0;
                continue;
            }
            if (j == 15) {
                pliks->macierz[i].param[j] = 1;
                continue;
            }
            pliks->macierz[i].param[j] = ParserX::GetNumber(bufor);
        }
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji images
//-----------------------------------

void SFileX::odczytajimages(FileBuffer* bufor, SFile* pliks) {
    pliks->ilosci = ParserX::GetNumber(bufor);
    pliks->image = new SFile::imgs[pliks->ilosci + 1];
    for (int i = 0; i < pliks->ilosci; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        bufor->off += 2;
        int j = 0;
        pliks->image[i].name = ParserX::GetString(bufor);
        pliks->image[i].tex = -1;
        //console.log(pliks->image[i].name);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji textures
//-----------------------------------

void SFileX::odczytajtextures(FileBuffer* bufor, SFile* pliks) {
    pliks->ilosct = ParserX::GetNumber(bufor);
    pliks->texture = new SFile::text[pliks->ilosct];
    for (int i = 0; i < pliks->ilosct; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        //pliks->texture[i] = new pliks->Texture();
        pliks->texture[i].image = ParserX::GetNumber(bufor);
        pliks->texture[i].arg1 = ParserX::GetNumber(bufor);
        pliks->texture[i].arg2 = ParserX::GetNumber(bufor);
        pliks->texture[i].arg3 = ParserX::GetNumber(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji vtx_states
//-----------------------------------

void SFileX::odczytajvtx_states(FileBuffer* bufor, SFile* pliks) {
    pliks->iloscv = ParserX::GetNumber(bufor);
    pliks->vtxstate = new SFile::vtxs[pliks->iloscv];
    for (int i = 0; i < pliks->iloscv; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        //pliks->vtxstate[i] = new pliks->Vtxs();
        pliks->vtxstate[i].arg1 = ParserX::GetHex(bufor);
        pliks->vtxstate[i].matrix = ParserX::GetNumber(bufor);
        pliks->vtxstate[i].arg2 = ParserX::GetNumber(bufor);
        pliks->vtxstate[i].arg3 = ParserX::GetNumber(bufor);

        while (bufor->get() != 41) {
            bufor->off++;
        }
        bufor->off++;
        pliks->vtxstate[i].arg4 = 0;
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji prim_states
//-----------------------------------

void SFileX::odczytajprim_states(FileBuffer* bufor, SFile* pliks) {
    pliks->iloscps = ParserX::GetNumber(bufor);
    pliks->primstate = new SFile::primst[pliks->iloscps];
    for (int i = 0; i < pliks->iloscps; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        //pliks->primstate[i] = new pliks->Primst();
        pliks->primstate[i].arg1 = ParserX::GetHex(bufor);
        pliks->primstate[i].arg2 = ParserX::GetNumber(bufor);
        pliks->primstate[i].arg3 = ParserX::GetNumber(bufor);
        pliks->primstate[i].arg4 = -1;
        for (int j = 0; j < pliks->primstate[i].arg3; j++) {
            if (j == 0) pliks->primstate[i].arg4 = ParserX::GetNumber(bufor);
            else ParserX::GetNumber(bufor);
        }
        pliks->primstate[i].arg5 = ParserX::GetNumber(bufor);
        pliks->primstate[i].vtx_state = ParserX::GetNumber(bufor);
        pliks->primstate[i].arg6 = ParserX::GetNumber(bufor);
        pliks->primstate[i].arg7 = ParserX::GetNumber(bufor);
        pliks->primstate[i].arg8 = ParserX::GetNumber(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji loddetail
//-----------------------------------

void SFileX::odczytajlodd(FileBuffer* bufor, SFile* pliks) {
    QString sh = "";
    int i, j, ii, jj;
    int iloscs, iloscv;
    int aktidx, iloscp, w, czilosc = 0, iii;
    int n, p, txt, nul;
    int v_ilosc;

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    GLUU* gluu = GLUU::get();
    fvertex* vert = new fvertex[120000];

    i = 0; // odczytujemy jeden lodcontrol=0;
    sh = "lod_control";
    ParserX::szukajsekcji1(sh, bufor);
    //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
    //szukamy sekcji distance levels
    sh = "distance_levels";
    ParserX::szukajsekcji1(sh, bufor);
    //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
    // wczytujemy ilosc distancelevels
    pliks->iloscd = ParserX::GetNumber(bufor);
    pliks->distancelevel = new SFile::dist[pliks->iloscd];

    // wczytujemy distancelevls
    for (j = 0; j < pliks->iloscd; j++) {
        //pliks->distancelevel[j] = new SFile->Dist();

        sh = "distance_level";
        ParserX::szukajsekcji1(sh, bufor);
        //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
        //wczytanie hierarchii
        sh = "distance_level_header";
        ParserX::szukajsekcji1(sh, bufor);
        nul = ParserX::GetNumber(bufor); //dlevel_selection//
        pliks->distancelevel[j].ilosch = ParserX::GetNumber(bufor);
        pliks->distancelevel[j].hierarchia = new int[pliks->distancelevel[j].ilosch + 1];
        for (ii = 0; ii < pliks->distancelevel[j].ilosch; ii++) {
            pliks->distancelevel[j].hierarchia[ii] = ParserX::GetNumber(bufor);
        }

        //szukamy subobjektow
        sh = "sub_objects";
        ParserX::szukajsekcji1(sh, bufor);
        //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
        //ilosc subobjektow
        pliks->distancelevel[j].iloscs = ParserX::GetNumber(bufor);
        // przydzielenie im pamieci
        pliks->distancelevel[j].subobiekty = new SFile::sub[pliks->distancelevel[j].iloscs + 1];

        //wczytujemy subobjekty
        iloscs = pliks->distancelevel[j].iloscs;
        for (ii = 0; ii < iloscs; ii++) {

            //szukanie itego subobjekta
            sh = "sub_object";
            ParserX::szukajsekcji1(sh, bufor);
            //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
            //szukaj sekcji wierzcholki
            sh = "vertices";
            ParserX::szukajsekcji1(sh, bufor);
            //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
            //ilosc wierzcholkow
            v_ilosc = ParserX::GetNumber(bufor);
            //wczytanie wierzholkow
            //System.out.println(v_ilosc); 

            iloscv = v_ilosc;
            //console.log("iloscv "+v_ilosc);
            for (jj = 0; jj < iloscv; jj++) {
                //vert[jj].arg1 = 
                vert[jj].arg1 = (short) ParserX::GetHex(bufor);
                vert[jj].point = (unsigned int) ParserX::GetNumber(bufor);
                vert[jj].normal = (unsigned int) ParserX::GetNumber(bufor);
                vert[jj].arg2 = (short) ParserX::GetHex(bufor);
                vert[jj].arg3 = (short) ParserX::GetHex(bufor);

                //if (jj < 20) Console.WriteLine("tu jest :" + bufor[ibufor]);
                //if (jj < 20) Console.WriteLine("tu jest :" + bufor[ibufor]);
                //vert[jj].arg2 = 
                //if (jj < 20) Console.WriteLine("tu jest :" + bufor[ibufor]);
                //vert[jj].arg3 = 
                vert[jj].material = (short) ParserX::GetNumber(bufor);

                for (int jjjj = 0; jjjj < vert[jj].material; jjjj++) {
                    if (jjjj == 0) vert[jj].uvpoint = (unsigned int) ParserX::GetNumber(bufor);
                }

                //if (jj < 100) 
                //Console.WriteLine("vert " + vert[jj].arg1 + " " + vert[jj].point + " " + vert[jj].normal + " " + vert[jj].arg2 + " " + vert[jj].arg3 + " " + vert[jj].material + " " + vert[jj].uvpoint);
            }
            //Console.Write("\n");
            //for(int yyy=0; yyy<500; yyy+=2) Console.Write((char)bufor[ibufor+yyy]);
            //Console.Write("\n");
            //szukaj sekcji z czesciami

            sh = "vertex_sets";
            ParserX::szukajsekcji1(sh, bufor);
            //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
            sh = "primitives";
            ParserX::szukajsekcji1(sh, bufor);
            //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
            pliks->distancelevel[j].subobiekty[ii].iloscc = ParserX::GetNumber(bufor);
            //console.log("wczytam p " + iloscc);
            // przydzielenie im pamieci
            pliks->distancelevel[j].subobiekty[ii].czesci = new SFile::czes[pliks->distancelevel[j].subobiekty[ii].iloscc + 1];

            //wczytaj czesci
            czilosc = 0;
            aktidx = 0;

            for (jj = 0; jj < pliks->distancelevel[j].subobiekty[ii].iloscc; jj++) {
                //pliks->distancelevel[j].subobiekty[ii][czilosc] = new pliks->Czesc();
                //qDebug() << "cc " << pliks->distancelevel[j].subobiekty[ii].iloscc;
                //wybor sekcji lista czy indeks
                w = ParserX::sekcjap(bufor);
                //console.log("ww " + w);
                if (w == -1) {
                    //jesli lista
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].prim_state_idx = aktidx;
                    //wczytanie indeksow wierzcholkow
                    sh = "vertex_idxs";
                    ParserX::szukajsekcji1(sh, bufor);
                    //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv = ParserX::GetNumber(bufor);

                    //pliks->distancelevel[j].subobiekty[ii][czilosc].pwierzcholki = new Float32Array(pliks->distancelevel[j].subobiekty[ii][czilosc].iloscv*8);
                    //float *wierzcholki = new float[pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv * 8];
                    //var nwierzcholki = new Float32Array(pliks->distancelevel[j].subobiekty[ii][czilosc].iloscv*3);
                    //var twierzcholki = new Float32Array(pliks->distancelevel[j].subobiekty[ii][czilosc].iloscv*2);
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].idx = new int[pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv];
                    //lista wierzcholkow
                    //pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].wierzcholki = new Sfile.wie[pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv];
                    for (int iii = pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv - 1; iii >= 0; iii--) {
                        //pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].wierzcholki[iii] = new Sfile.wie();
                        //qDebug() << "cc " << pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv;
                        //w = ParserX::GetNumber(bufor);
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].idx[iii] = ParserX::GetNumber(bufor);
                        /*n = vert[w].normal;
                        txt = vert[w].uvpoint;
                        p = vert[w].point;

                        wierzcholki[iii * 8 + 0] = pliks->tpoints.points[p].x;
                        wierzcholki[iii * 8 + 1] = pliks->tpoints.points[p].y;
                        wierzcholki[iii * 8 + 2] = pliks->tpoints.points[p].z;
                        wierzcholki[iii * 8 + 3] = pliks->tpoints.normals[n].x;
                        wierzcholki[iii * 8 + 4] = pliks->tpoints.normals[n].y;
                        wierzcholki[iii * 8 + 5] = pliks->tpoints.normals[n].z;
                        wierzcholki[iii * 8 + 6] = pliks->tpoints.uv_points[txt].x;
                        wierzcholki[iii * 8 + 7] = pliks->tpoints.uv_points[txt].y;*/
                    }
                    /*QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VAO.create();
                    QOpenGLVertexArrayObject::Binder vaoBinder(&pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VAO);

                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.create();
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.bind();
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.allocate(wierzcholki, pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv * 8 * sizeof (GLfloat));
                    f->glEnableVertexAttribArray(0);
                    f->glEnableVertexAttribArray(1);
                    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), 0);
                    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (GLfloat), reinterpret_cast<void *> (6 * sizeof (GLfloat)));
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.release();
                    
                    delete[] wierzcholki;*/
                    //nwierzcholki = null;
                    //twierzcholki = null;
                    //pliks->distancelevel[j].subobiekty[ii][czilosc].pwierzcholki = pwierzcholki;
                    //pominiecie normals i flags 
                    sh = "normal_idxs";
                    ParserX::szukajsekcji1(sh, bufor);
                    sh = "flags";
                    ParserX::szukajsekcji1(sh, bufor);
                    czilosc++;
                } else {
                    aktidx = w;
                }
            }
            pliks->distancelevel[j].subobiekty[ii].iloscc = czilosc;
            //Console.WriteLine("wczytano cz " + czilosc);
            /////////////////////////
           int iloscv = 0;
                int offset = 0;
                for (int jj = 0; jj < pliks->distancelevel[j].subobiekty[ii].iloscc; jj++) {
                    iloscv += pliks->distancelevel[j].subobiekty[ii].czesci[jj].iloscv;
                }

                pliks->distancelevel[j].subobiekty[ii].VAO.create();
                QOpenGLVertexArrayObject::Binder vaoBinder(&pliks->distancelevel[j].subobiekty[ii].VAO);
                        
                pliks->distancelevel[j].subobiekty[ii].VBO.create();
                pliks->distancelevel[j].subobiekty[ii].VBO.bind();
                pliks->distancelevel[j].subobiekty[ii].VBO.allocate(iloscv * 9 * sizeof(GLfloat));
                f->glEnableVertexAttribArray(0);
                f->glEnableVertexAttribArray(1);
                f->glEnableVertexAttribArray(2);
                f->glEnableVertexAttribArray(3);
                f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), 0);
                f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
                f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
                f->glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), reinterpret_cast<void *>(8 * sizeof(GLfloat)));

                for (int jj = 0; jj < pliks->distancelevel[j].subobiekty[ii].iloscc; jj++) {
                    float *wierzcholki = new float[pliks->distancelevel[j].subobiekty[ii].czesci[jj].iloscv*9];

                    for (int iii = pliks->distancelevel[j].subobiekty[ii].czesci[jj].iloscv - 1; iii >= 0; iii--) {
                            //pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].wierzcholki[iii] = new SFile::wie();

                        w = pliks->distancelevel[j].subobiekty[ii].czesci[jj].idx[iii];
                        int prim_state = pliks->distancelevel[j].subobiekty[ii].czesci[jj].prim_state_idx;
                        float alpha = 0;
                        float alphaTest = 0;
                        if(pliks->primstate[prim_state].arg2 < pliks->ishaders)
                            alpha = pliks->shader[pliks->primstate[prim_state].arg2].alpha;
                        else 
                            alpha = 0;
                        if(pliks->primstate[prim_state].arg6 == 1)
                            alphaTest = -0.51f;
                        else 
                            alphaTest = -gluu->alphaTest;
                        if(alpha == 1)
                            alphaTest = 1.0;
                            //System.out.println("----v "+w);
                        n = vert[w].normal;
                        txt = vert[w].uvpoint;
                        p = vert[w].point;

                        wierzcholki[iii*9+0] = pliks->tpoints.points[p].x;
                        wierzcholki[iii*9+1] = pliks->tpoints.points[p].y;
                        wierzcholki[iii*9+2] = pliks->tpoints.points[p].z;
                        wierzcholki[iii*9+3] = pliks->tpoints.normals[n].x;
                        wierzcholki[iii*9+4] = pliks->tpoints.normals[n].y;
                        wierzcholki[iii*9+5] = pliks->tpoints.normals[n].z;
                        wierzcholki[iii*9+6] = pliks->tpoints.uv_points[txt].x;
                        wierzcholki[iii*9+7] = pliks->tpoints.uv_points[txt].y;
                        wierzcholki[iii*9+8] = alphaTest;
                            //directxSmierdzi-=2;
                            //if(directxSmierdzi<-2) directxSmierdzi = 2;
                    }
                    pliks->distancelevel[j].subobiekty[ii].VBO.write(offset * 9 * sizeof(GLfloat), wierzcholki, pliks->distancelevel[j].subobiekty[ii].czesci[jj].iloscv * 9 * sizeof(GLfloat));
                    delete[] wierzcholki;
                    pliks->distancelevel[j].subobiekty[ii].czesci[jj].offset = offset;
                    offset += pliks->distancelevel[j].subobiekty[ii].czesci[jj].iloscv;
                    delete[] pliks->distancelevel[j].subobiekty[ii].czesci[jj].idx;
                }
                pliks->distancelevel[j].subobiekty[ii].VBO.release();
        }
    }

    delete[] vert;
    delete[] pliks->tpoints.normals;
    delete[] pliks->tpoints.points;
    delete[] pliks->tpoints.uv_points;
    return;
};
