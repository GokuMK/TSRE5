#include "SFileX.h"
#include "SFile.h"
#include <QDebug>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QtCore>
#include "ParserX.h"

void SFileX::odczytajshaders(FileBuffer* bufor, SFile* pliks) {
    pliks->ishaders = ParserX::parsujr(bufor);

    pliks->shader = new SFile::fshader[pliks->ishaders];
    for (int i = 0; i < pliks->ishaders; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        bufor->off += 2;
        pliks->shader[i].name = ParserX::odczytajtc(bufor).toLower();
        //qDebug() << pliks->shaderName[i];
        //console.log(pliks->image[i].name);
    }
}

//-----------------------------------
//Oczytanie i zapisanie sekcji points
//-----------------------------------

void SFileX::odczytajpunkty(FileBuffer* bufor, SFile* pliks) {
    pliks->tpoints.ipoints = ParserX::parsujr(bufor);
    //qDebug() << ilosc;
    pliks->tpoints.points = new SFile::fpoint[pliks->tpoints.ipoints + 1];
    for (int i = 0; i < pliks->tpoints.ipoints; i++) {
        pliks->tpoints.points[i].x = ParserX::parsujr(bufor);
        pliks->tpoints.points[i].y = ParserX::parsujr(bufor);
        pliks->tpoints.points[i].z = ParserX::parsujr(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji uv_points
//-----------------------------------

void SFileX::odczytajuvpunkty(FileBuffer* bufor, SFile* pliks) {
    int ilosc = ParserX::parsujr(bufor);
    pliks->tpoints.uv_points = new SFile::fpoint[ ilosc + 1];
    for (int i = 0; i < ilosc; i++) {
        pliks->tpoints.uv_points[i].x = ParserX::parsujr(bufor);
        pliks->tpoints.uv_points[i].y = ParserX::parsujr(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji normals
//-----------------------------------

void SFileX::odczytajnormalne(FileBuffer* bufor, SFile* pliks) {
    int ilosc = ParserX::parsujr(bufor);
    pliks->tpoints.normals = new SFile::fpoint[ilosc + 1];
    for (int i = 0; i < ilosc; i++) {
        pliks->tpoints.normals[i].x = ParserX::parsujr(bufor);
        pliks->tpoints.normals[i].y = ParserX::parsujr(bufor);
        pliks->tpoints.normals[i].z = ParserX::parsujr(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji matrices
//-----------------------------------

void SFileX::odczytajmatrices(FileBuffer* bufor, SFile* pliks) {
    pliks->iloscm = ParserX::parsujr(bufor);
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
            pliks->macierz[i].param[j] = ParserX::parsujr(bufor);
        }
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji images
//-----------------------------------

void SFileX::odczytajimages(FileBuffer* bufor, SFile* pliks) {
    pliks->ilosci = ParserX::parsujr(bufor);
    pliks->image = new SFile::imgs[pliks->ilosci + 1];
    for (int i = 0; i < pliks->ilosci; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        bufor->off += 2;
        int j = 0;
        pliks->image[i].name = ParserX::odczytajtc(bufor);
        pliks->image[i].tex = -1;
        //console.log(pliks->image[i].name);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji textures
//-----------------------------------

void SFileX::odczytajtextures(FileBuffer* bufor, SFile* pliks) {
    pliks->ilosct = ParserX::parsujr(bufor);
    pliks->texture = new SFile::text[pliks->ilosct];
    for (int i = 0; i < pliks->ilosct; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        //pliks->texture[i] = new pliks->Texture();
        pliks->texture[i].image = ParserX::parsujr(bufor);
        pliks->texture[i].arg1 = ParserX::parsujr(bufor);
        pliks->texture[i].arg2 = ParserX::parsujr(bufor);
        pliks->texture[i].arg3 = ParserX::parsujr(bufor);
    }
    return;
};
//-----------------------------------
//Oczytanie i zapisanie sekcji vtx_states
//-----------------------------------

void SFileX::odczytajvtx_states(FileBuffer* bufor, SFile* pliks) {
    pliks->iloscv = ParserX::parsujr(bufor);
    pliks->vtxstate = new SFile::vtxs[pliks->iloscv];
    for (int i = 0; i < pliks->iloscv; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        //pliks->vtxstate[i] = new pliks->Vtxs();
        pliks->vtxstate[i].arg1 = ParserX::parsuj16(bufor);
        pliks->vtxstate[i].matrix = ParserX::parsujr(bufor);
        pliks->vtxstate[i].arg2 = ParserX::parsujr(bufor);
        pliks->vtxstate[i].arg3 = ParserX::parsujr(bufor);

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
    pliks->iloscps = ParserX::parsujr(bufor);
    pliks->primstate = new SFile::primst[pliks->iloscps];
    for (int i = 0; i < pliks->iloscps; i++) {
        while (bufor->get() != 40) {
            bufor->off++;
        }
        bufor->off++;
        //pliks->primstate[i] = new pliks->Primst();
        pliks->primstate[i].arg1 = ParserX::parsuj16(bufor);
        pliks->primstate[i].arg2 = ParserX::parsujr(bufor);
        pliks->primstate[i].arg3 = ParserX::parsujr(bufor);
        pliks->primstate[i].arg4 = -1;
        for (int j = 0; j < pliks->primstate[i].arg3; j++) {
            if (j == 0) pliks->primstate[i].arg4 = ParserX::parsujr(bufor);
            else ParserX::parsujr(bufor);
        }
        pliks->primstate[i].arg5 = ParserX::parsujr(bufor);
        pliks->primstate[i].vtx_state = ParserX::parsujr(bufor);
        pliks->primstate[i].arg6 = ParserX::parsujr(bufor);
        pliks->primstate[i].arg7 = ParserX::parsujr(bufor);
        pliks->primstate[i].arg8 = ParserX::parsujr(bufor);
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

    fvertex* vert = new fvertex[50000];

    i = 0; // odczytujemy jeden lodcontrol=0;
    sh = "lod_control";
    ParserX::szukajsekcji1(sh, bufor);
    //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
    //szukamy sekcji distance levels
    sh = "distance_levels";
    ParserX::szukajsekcji1(sh, bufor);
    //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
    // wczytujemy ilosc distancelevels
    pliks->iloscd = ParserX::parsujr(bufor);
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
        nul = ParserX::parsujr(bufor); //dlevel_selection//
        pliks->distancelevel[j].ilosch = ParserX::parsujr(bufor);
        pliks->distancelevel[j].hierarchia = new int[pliks->distancelevel[j].ilosch + 1];
        for (ii = 0; ii < pliks->distancelevel[j].ilosch; ii++) {
            pliks->distancelevel[j].hierarchia[ii] = ParserX::parsujr(bufor);
        }

        //szukamy subobjektow
        sh = "sub_objects";
        ParserX::szukajsekcji1(sh, bufor);
        //console.log("znaleziono sekcje " + sh + " na " + bufor.p);
        //ilosc subobjektow
        pliks->distancelevel[j].iloscs = ParserX::parsujr(bufor);
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
            v_ilosc = ParserX::parsujr(bufor);
            //wczytanie wierzholkow
            //System.out.println(v_ilosc); 

            iloscv = v_ilosc;
            //console.log("iloscv "+v_ilosc);
            for (jj = 0; jj < iloscv; jj++) {
                //vert[jj].arg1 = 
                vert[jj].arg1 = (short) ParserX::parsuj16(bufor);
                vert[jj].point = (short) ParserX::parsujr(bufor);
                vert[jj].normal = (short) ParserX::parsujr(bufor);
                vert[jj].arg2 = (short) ParserX::parsuj16(bufor);
                vert[jj].arg3 = (short) ParserX::parsuj16(bufor);

                //if (jj < 20) Console.WriteLine("tu jest :" + bufor[ibufor]);
                //if (jj < 20) Console.WriteLine("tu jest :" + bufor[ibufor]);
                //vert[jj].arg2 = 
                //if (jj < 20) Console.WriteLine("tu jest :" + bufor[ibufor]);
                //vert[jj].arg3 = 
                vert[jj].material = (short) ParserX::parsujr(bufor);

                for (int jjjj = 0; jjjj < vert[jj].material; jjjj++) {
                    if (jjjj == 0) vert[jj].uvpoint = (short) ParserX::parsujr(bufor);
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
            pliks->distancelevel[j].subobiekty[ii].iloscc = ParserX::parsujr(bufor);
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
                    pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv = ParserX::parsujr(bufor);

                    //pliks->distancelevel[j].subobiekty[ii][czilosc].pwierzcholki = new Float32Array(pliks->distancelevel[j].subobiekty[ii][czilosc].iloscv*8);
                    float *wierzcholki = new float[pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv * 8];
                    //var nwierzcholki = new Float32Array(pliks->distancelevel[j].subobiekty[ii][czilosc].iloscv*3);
                    //var twierzcholki = new Float32Array(pliks->distancelevel[j].subobiekty[ii][czilosc].iloscv*2);

                    //lista wierzcholkow
                    //pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].wierzcholki = new Sfile.wie[pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv];
                    for (int iii = pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv - 1; iii >= 0; iii--) {
                        //pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].wierzcholki[iii] = new Sfile.wie();
                        //qDebug() << "cc " << pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv;
                        w = ParserX::parsujr(bufor);
                        
                        n = vert[w].normal;
                        txt = vert[w].uvpoint;
                        p = vert[w].point;

                        wierzcholki[iii * 8 + 0] = pliks->tpoints.points[p].x;
                        wierzcholki[iii * 8 + 1] = pliks->tpoints.points[p].y;
                        wierzcholki[iii * 8 + 2] = pliks->tpoints.points[p].z;
                        wierzcholki[iii * 8 + 3] = pliks->tpoints.normals[n].x;
                        wierzcholki[iii * 8 + 4] = pliks->tpoints.normals[n].y;
                        wierzcholki[iii * 8 + 5] = pliks->tpoints.normals[n].z;
                        wierzcholki[iii * 8 + 6] = pliks->tpoints.uv_points[txt].x;
                        wierzcholki[iii * 8 + 7] = pliks->tpoints.uv_points[txt].y;
                    }
                    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
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
                    
                    delete[] wierzcholki;
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
        }
    }

    delete[] vert;
    delete[] pliks->tpoints.normals;
    delete[] pliks->tpoints.points;
    delete[] pliks->tpoints.uv_points;
    return;
};
