/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SFileC.h"
#include "SFile.h"
#include <QDebug>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "GLUU.h"
#include "TS.h"

    void SFileC::odczytajshaders(FileBuffer* bufor, SFile* pliks) {
        int temp;

        bufor->off += 1;
        pliks->ishaders = bufor->getInt();

        pliks->shader = new SFile::fshader[pliks->ishaders];
        for (int i = 0; i < pliks->ishaders; i++) {
            bufor->off += 4;
            temp = bufor->get();
            bufor->off += 6;
            //pliks->image[i] = new SFile::imgs();
            pliks->shader[i].name = "";
            for (int j = 0; j < (temp - 3) / 2; j++) {
                pliks->shader[i].name += bufor->get();
                bufor->get();
            }
            pliks->shader[i].name = pliks->shader[i].name.toLower();
            if(pliks->shader[i].name == "texdiff") 
                pliks->shader[i].alpha = 1;
            else 
                pliks->shader[i].alpha = 0;
            //qDebug() << pliks->shaderName[i];
        }
        return;
    }

    //-----------------------------------
    //Szukanie sekcji
    //-----------------------------------
    /*void SFileC::szukajsekcjic(int szukane, FileBuffer* bufor) {
        int s;
        while (bufor->length>bufor->off) {
            s = (int)bufor->getInt();
            if (s == szukane) {
                return;
            }

            s = bufor->getInt();
            bufor->off += s;
        }
        //Console.WriteLine("nie znaleziono 7");
        return;
    }*/
    //-----------------------------------
    //Oczytanie i zapisanie sekcji points
    //-----------------------------------

    void SFileC::odczytajpunktyc(FileBuffer* bufor, SFile* pliks) {
        bufor->off += 1;
        pliks->tpoints.ipoints = bufor->getInt();

        pliks->tpoints.points = new SFile::fpoint[ pliks->tpoints.ipoints + 1];
        for (int i = 0; i <  pliks->tpoints.ipoints; i++) {
            bufor->off += 9;
            //pliks->tpoints.points[i] = new SFile::fpoint();
            pliks->tpoints.points[i].x = bufor->getFloat();
            pliks->tpoints.points[i].y = bufor->getFloat();
            pliks->tpoints.points[i].z = bufor->getFloat();
        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji uv_points
    //-----------------------------------

    void SFileC::odczytajuvpunktyc(FileBuffer* bufor, SFile* pliks) {
        int ilosc;

        bufor->off += 1;
        ilosc = bufor->getInt();
        
        pliks->tpoints.uv_points = new SFile::fpoint[ilosc + 1];
        for (int i = 0; i < ilosc; i++) {
            bufor->off += 9;
            //pliks->tpoints.uv_points[i] = new SFile::fpoint();
            pliks->tpoints.uv_points[i].x = bufor->getFloat();
            pliks->tpoints.uv_points[i].y = bufor->getFloat();
        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji normals
    //-----------------------------------

    void SFileC::odczytajnormalnec(FileBuffer* bufor, SFile* pliks) {
        int ilosc;

        bufor->off += 1;
        ilosc = bufor->getInt();
        pliks->tpoints.normals = new SFile::fpoint[ilosc + 1];
        for (int i = 0; i < ilosc; i++) {
            bufor->off += 9;
            //pliks->tpoints.normals[i] = new SFile::fpoint();
            pliks->tpoints.normals[i].x = bufor->getFloat();
            pliks->tpoints.normals[i].y = bufor->getFloat();
            pliks->tpoints.normals[i].z = bufor->getFloat();
        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji matrices
    //-----------------------------------

    void SFileC::odczytajmatricesc(FileBuffer* bufor, SFile* pliks) {
        int temp;

        bufor->off += 1;
        pliks->iloscm = bufor->getInt();

        pliks->macierz = new SFile::matrt[pliks->iloscm + 1];
        for (int i = 0; i < pliks->iloscm; i++) {
            bufor->off += 8;
            temp = bufor->get();
            pliks->macierz[i].name = *bufor->getString(bufor->off, bufor->off + temp*2);
            //qDebug() << pliks->macierz[i].name;
            bufor->off += temp*2;
            //pliks->macierz[i] = new SFile::matrt();
            for (int j = 0; j < 16; j++) {
                if(j==3 || j==7 || j==11) {
                    pliks->macierz[i].param[j] = 0;
                    continue;
                }
                if(j==15) {
                    pliks->macierz[i].param[j] = 1;
                    continue;
                }
                pliks->macierz[i].param[j] = bufor->getFloat();
            }
        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji images
    //-----------------------------------

    void SFileC::odczytajimagesc(FileBuffer* bufor, SFile* pliks) {
        int temp;

        bufor->off += 1;
        pliks->ilosci = bufor->getInt();

        pliks->image = new SFile::imgs[pliks->ilosci + 1];
        for (int i = 0; i < pliks->ilosci; i++) {
            bufor->off += 4;
            temp = bufor->get();
            bufor->off += 6;
            //pliks->image[i] = new SFile::imgs();
            pliks->image[i].name = "";
            for (int j = 0; j < (temp - 3) / 2; j++) {
                pliks->image[i].name += bufor->get();
                bufor->get();
            }
            pliks->image[i].tex = -1;
        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji textures
    //-----------------------------------

    void SFileC::odczytajtexturesc(FileBuffer* bufor, SFile* pliks) {

        bufor->off += 1;

        pliks->ilosct = bufor->getInt();

        pliks->texture = new SFile::text[pliks->ilosct];

        for (int i = 0; i < pliks->ilosct; i++) {
            //pliks->texture[i] = new SFile::text();
            bufor->off += 9;
            pliks->texture[i].image = bufor->getInt();
            pliks->texture[i].arg1 = bufor->getInt();
            pliks->texture[i].arg2 = bufor->getInt();
            pliks->texture[i].arg3 = bufor->getInt();
        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji vtx_states
    //-----------------------------------

    void SFileC::odczytajvtx_statesc(FileBuffer* bufor, SFile* pliks) {

        bufor->off += 1;
        pliks->iloscv = bufor->getInt();

        pliks->vtxstate = new SFile::vtxs[pliks->iloscv];

        for (int i = 0; i < pliks->iloscv; i++) {
            bufor->off += 9;
            //pliks->vtxstate[i] = new SFile::vtxs();
            pliks->vtxstate[i].arg1 = bufor->getInt();
            pliks->vtxstate[i].matrix = bufor->getInt();
            pliks->vtxstate[i].arg2 = bufor->getInt();
            pliks->vtxstate[i].arg3 = bufor->getInt();
            pliks->vtxstate[i].arg4 = bufor->getInt();
        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji prim_states
    //-----------------------------------

    void SFileC::odczytajprim_statesc(FileBuffer* bufor, SFile* pliks) {
        int temp;

        bufor->off += 1;
        pliks->iloscps = bufor->getInt();

        pliks->primstate = new SFile::primst[pliks->iloscps];
        for (int i = 0; i < pliks->iloscps; i++) {
            bufor->off += 8;
            temp = bufor->get();
            bufor->off += temp*2;

            //pliks->primstate[i] = new SFile::primst();

            pliks->primstate[i].arg1 = bufor->getInt();
            pliks->primstate[i].arg2 = bufor->getInt();

            bufor->off += 9;

            pliks->primstate[i].arg3 = bufor->getInt();
            pliks->primstate[i].arg4 = -1;
            for(int j = 0; j<pliks->primstate[i].arg3; j++){
                if(j == 0) pliks->primstate[i].arg4 = bufor->getInt();
                else bufor->getInt();
            }
            
            pliks->primstate[i].arg5 = bufor->getInt();

            pliks->primstate[i].vtx_state = bufor->getInt();

            pliks->primstate[i].arg6 = bufor->getInt();
            pliks->primstate[i].arg7 = bufor->getInt();
            pliks->primstate[i].arg8 = bufor->getInt();

        }
        return;
    }
    //-----------------------------------
    //Oczytanie i zapisanie sekcji loddetail
    //-----------------------------------

    void SFileC::odczytajloddc(FileBuffer* bufor, SFile* pliks) {
        int temp;
        int i, w, n, p, txt;
        int v_ilosc;
        
        QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
        GLUU* gluu = GLUU::get();
        fvertex* vert = new fvertex[120000];

        bufor->off += 5; // pominiecie ilosci lodcontrols;

        //Odczytanie jednego LOD_Control=0;
        i = 0;

        bufor->off += 13;
        // pominiecie sekcji;
        temp = bufor->getInt();

        bufor->off += temp+9;

        // odczytanie ilosci distance levels
        pliks->iloscd = bufor->getInt();

        // przydzielenie dla nich pamieci
        pliks->distancelevel = new SFile::dist[pliks->iloscd];

        for (int j = 0; j < pliks->iloscd; j++) {
            //pliks.distancelevel[j] = new Sfile.dist();

            bufor->off += 27;
            // pomieniecie sekcji dlevel selection
            bufor->off += 4;
            bufor->off += 9;
            // odczytanie dlugosci hierarchii obiektow
            pliks->distancelevel[j].ilosch = bufor->getInt();

            // przydzielenie dla niej pamieci
            pliks->distancelevel[j].hierarchia = new int[pliks->distancelevel[j].ilosch + 1];
            // wczytanie hierarchii
            for (int ii = 0; ii < pliks->distancelevel[j].ilosch; ii++) {
                pliks->distancelevel[j].hierarchia[ii] = bufor->getInt();
            }

            // odczytanie ilosci subobjektow
            bufor->off += 9;
            pliks->distancelevel[j].iloscs = bufor->getInt();
            //qDebug () << "--iloscs " << pliks->distancelevel[j].iloscs;

            // przydzielenie im pamieci
            pliks->distancelevel[j].subobiekty = new SFile::sub[pliks->distancelevel[j].iloscs + 1];
            // wczytanie subobjektow
            for (int ii = 0; ii < pliks->distancelevel[j].iloscs; ii++) {
                //pliks->distancelevel[j].subobiekty[ii] = new SFile::sub();

                
                bufor->off += 9;
                bufor->findToken(TS::sub_object_header);
                //qDebug() << "sub_object_header";
                int offset1 = bufor->getInt();
                int akto1 = bufor->off;
                bufor->off++;
                bufor->off += 20;
                for (;;) {
                    int pozycja2,offset2,akto2;
                    pozycja2 = bufor->getInt();
                    offset2 = bufor->getInt();
                    akto2 = bufor->off;
                    switch (pozycja2) {
                        case TS::subobject_shaders:
                            break;
                        case TS::subobject_light_cfgs:
                            break;
                        case TS::geometry_info:
                            int pozycja3,offset3,akto3;
                            bufor->off++;
                            bufor->off+=40;
                            for (;;) {
                                pozycja3 = bufor->getInt();
                                offset3 = bufor->getInt();
                                akto3 = bufor->off;
                                int count;
                                switch (pozycja3) {
                                    case TS::geometry_nodes:
                                        break;
                                    case TS::geometry_node_map:
                                        bufor->off++;
                                        count = bufor->getInt();
                                        for(int ignm = 0; ignm < count; ignm++ ){
                                            pliks->distancelevel[j].subobiekty[ii].header.geometryNodeMap.push_back(bufor->getInt());
                                        }
                                        break;
                                    default:
                                        qDebug() << "#SFile geometry_info - unknown token: "<< pozycja3 << TS::IdName[pozycja3];
                                        break;
                                }
                                bufor->off = akto3 + offset3;
                                if(bufor->off >= akto2 + offset2) break;
                            }
                            break;
                        default:
                            //qDebug() << "#SFile sub_object_header - unknown token: "<< pozycja2 << TS::IdName[pozycja2];
                            break;
                    }
                    bufor->off = akto2 + offset2;
                    if(bufor->off >= akto1 + offset1) break;
                }
                bufor->off = akto1 + offset1;
                
                // Wczytanie wierzcholkow
                bufor->findToken(TS::vertices);
                bufor->off += 5;
                v_ilosc = bufor->getInt();
                if(v_ilosc > 120000){
                    delete[] vert;
                    vert = new fvertex[v_ilosc];
                }
                
                //qDebug () << "iloscv " << v_ilosc;
                
                for (int jj = 0; jj < v_ilosc; jj++) {
                    bufor->off += 9;
                    //vert[jj] = new fvertex();
                    vert[jj].arg1 = (short) bufor->getInt();
                    vert[jj].point = (unsigned int) bufor->getInt();
                    vert[jj].normal = (unsigned int) bufor->getInt();
                    vert[jj].arg2 = (short) bufor->getInt();
                    vert[jj].arg3 = (short) bufor->getInt();

                    bufor->off += 9;
                    vert[jj].material = (short) bufor->getInt();
                    
                    for(int jjjj = 0; jjjj<vert[jj].material; jjjj++){
                        if(jjjj == 0) vert[jj].uvpoint = (unsigned int) bufor->getInt();
                    }
                    
                }

                // odczytanie primitives-'czesci'
                bufor->findToken(53);
                bufor->off += 5;
                // odczytanie ich ilosci
                pliks->distancelevel[j].subobiekty[ii].iloscc = bufor->getInt();
                //System.out.println("iloscc "+pliks.distancelevel[j].subobiekty[ii].iloscc);
                int czilosc = 0, aktidx = 0;
                // przydzielenie im pamieci   
                pliks->distancelevel[j].subobiekty[ii].czesci = new SFile::czes[pliks->distancelevel[j].subobiekty[ii].iloscc + 1];

                // wczytanie czesci  
                for (int jj = 0; jj < pliks->distancelevel[j].subobiekty[ii].iloscc; jj++) {
                    //pliks->distancelevel[j]->subobiekty[ii].czesci[czilosc] = new SFile::czes();
                    //qDebug() << "cc " << pliks->distancelevel[j].subobiekty[ii].iloscc;
                    temp = bufor->getInt();

                    if (temp == 56) {
                        bufor->off += 5;
                        aktidx = bufor->getInt();

                    } else {
                        bufor->off += 5 + 9;
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].prim_state_idx = aktidx;
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv = bufor->getInt();

                        //float *wierzcholki = new float[pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv*8];

                        // wczytanie listy wierzcholkow
                        //int directxSmierdzi = 0;
                        //for (int iii = 0; iii < pliks.distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv; iii++) {
                        
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].idx = new int[pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv];
                        for (int iii = pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv - 1; iii >= 0; iii--) {
                            //pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].wierzcholki[iii] = new SFile::wie();
                            pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].idx[iii] = bufor->getInt();
                            //w = bufor->getInt();
                            //System.out.println("----v "+w);
                            /*n = vert[w].normal;
                            txt = vert[w].uvpoint;
                            p = vert[w].point;

                            wierzcholki[iii*8+0] = pliks->tpoints.points[p].x;
                            wierzcholki[iii*8+1] = pliks->tpoints.points[p].y;
                            wierzcholki[iii*8+2] = pliks->tpoints.points[p].z;
                            wierzcholki[iii*8+3] = pliks->tpoints.normals[n].x;
                            wierzcholki[iii*8+4] = pliks->tpoints.normals[n].y;
                            wierzcholki[iii*8+5] = pliks->tpoints.normals[n].z;
                            wierzcholki[iii*8+6] = pliks->tpoints.uv_points[txt].x;
                            wierzcholki[iii*8+7] = pliks->tpoints.uv_points[txt].y;
                        */
                            //directxSmierdzi-=2;
                            //if(directxSmierdzi<-2) directxSmierdzi = 2;
                        }
                        /*QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VAO.create();
                        QOpenGLVertexArrayObject::Binder vaoBinder(&pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VAO);
                        
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.create();
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.bind();
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.allocate(wierzcholki, pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].iloscv * 8 * sizeof(GLfloat));
                        f->glEnableVertexAttribArray(0);
                        f->glEnableVertexAttribArray(1);
                        f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
                        f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
                        pliks->distancelevel[j].subobiekty[ii].czesci[czilosc].VBO.release();
                        
                        delete[] wierzcholki;*/
                        
                        bufor->off += 4;
                        temp = bufor->getInt();
                        bufor->off += temp;

                        bufor->off += 4;
                        temp = bufor->getInt();
                        bufor->off += temp;

                        czilosc++;
                        
                    }
                }
                pliks->distancelevel[j].subobiekty[ii].iloscc = czilosc;
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
    }
