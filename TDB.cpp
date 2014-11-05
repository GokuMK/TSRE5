#include "TDB.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "GLMatrix.h"

TDB::TDB(QString path) {
    loaded = false;
    wysokoscSieci = 4;
    qDebug() << "Wczytywanie pliku tdb: " << path;

    tsection = new TSectionDAT();

    int x, i, j, ii, jj, uu;
    float xx;
    int t;
    QString sh;
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* bufor = ReadFile::read(file);

    //szukanie trackdb
    sh = "TrackDB";
    ParserX::szukajsekcji1(sh, bufor);
    qDebug() << "znaleziono sekcje na " << bufor->off;
    //szukanie tracknodes
    sh = "TrackNodes";
    ParserX::szukajsekcji1(sh, bufor);

    iTRnodes = (int) ParserX::parsujr(bufor); //odczytanie ilosci sciezek
    trackNodes = new TRnode[iTRnodes + 1]; //przydzielenie pamieci
    qDebug() << "N" << iTRnodes;
    //zapisanie x tracknodes
    for (i = 0; i < iTRnodes; i++) {
        sh = "TrackNode";
        ParserX::szukajsekcji1(sh, bufor);
        t = (int) ParserX::parsujr(bufor); // odczytanie numeru sciezki
        sh = ParserX::nazwasekcji(bufor);
        //System.out.println("----"+sh);

        switch (sh.length()) {// wybranie typu sciezki ^^
            case 9:
                trackNodes[t].typ = 0; //typ endnode
                sh = "UiD";
                ParserX::szukajsekcji1(sh, bufor);
                for (ii = 0; ii < 12; ii++) {
                    trackNodes[t].UiD[ii] = ParserX::parsujr(bufor);
                }
                trpin(&trackNodes[t], bufor);
                break;
            case 12:
                trackNodes[t].typ = 1; //typ vector 
                sh = "TrVectorSections";
                ParserX::szukajsekcji1(sh, bufor);
                uu = (int) ParserX::parsujr(bufor);
                trackNodes[t].iTrv = uu;
                trackNodes[t].trVectorSection = new TRnode::TRSect[uu]; // przydzielenie pamieci dla sciezki

                for (j = 0; j < uu; j++) {
                    for (ii = 0; ii < 16; ii++) {
                        xx = ParserX::parsujr(bufor);
                        trackNodes[t].trVectorSection[j].param[ii] = xx;
                    }
                    //System.out.println(
                    //this.TrackNodes[t].TrVectorSection[j].param[13]+" "+
                    //this.TrackNodes[t].TrVectorSection[j].param[14]+" "+
                    //this.TrackNodes[t].TrVectorSection[j].param[15]);
                }
                trpin(&trackNodes[t], bufor);
                break;
            case 14:
                trackNodes[t].typ = 2; //typ rozjazd
                sh = "UiD";
                ParserX::szukajsekcji1(sh, bufor);
                for (ii = 0; ii < 12; ii++) {
                    trackNodes[t].UiD[ii] = ParserX::parsujr(bufor);
                }
                trpin(&trackNodes[t], bufor);
                break;
            default:
                qDebug() << "Nieprawidlowa sciezka -> ERROR";
                break;
        }
    }

    loaded = true;
    return;
}

void TDB::trpin(TRnode *tr, FileBuffer* bufor) {
    QString sh;

    sh = "TrPins";
    ParserX::szukajsekcji1(sh, bufor);

    tr->TrP1 = (int) ParserX::parsujr(bufor);
    tr->TrP2 = (int) ParserX::parsujr(bufor);

    for (int i = 0; i < (tr->TrP1 + tr->TrP2); i++) {
        tr->TrPinS[i] = (int) ParserX::parsujr(bufor);
        tr->TrPinK[i] = (int) ParserX::parsujr(bufor);
    }
}

void TDB::renderAll(GLUU *gluu, float* playerT) {

    if (!loaded) return;
    int hash = playerT[0] * 10000 + playerT[1];
    if (!linieSieci.loaded || lineHash != hash) {
        TRnode *n;
        int lLen = 0, kLen = 0, pLen = 0;
        //qDebug() <<"update";
        lineHash = hash;

        for (int i = 0; i < iTRnodes; i++) {
            n = &trackNodes[i];
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                lLen += 6 * (n->iTrv - 1);
                if (n->TrPinS[1] != 0) {
                    lLen += 6;
                }
            } else if (n->typ == 0) {
                kLen += 6;
            } else if (n->typ == 2) {
                pLen += 6;
            }
        }
        float* linie = new float[lLen];
        float* konce = new float[kLen];
        float* punkty = new float[pLen];
        int lPtr = 0, kPtr = 0, pPtr = 0;

        for (int i = 0; i < iTRnodes; i++) {
            n = &trackNodes[i];
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                for (int i = 0; i < n->iTrv - 1; i++) {
                    linie[lPtr++] = ((n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10]);
                    linie[lPtr++] = (n->trVectorSection[i].param[11] + wysokoscSieci);
                    linie[lPtr++] = (((-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12]));

                    linie[lPtr++] = ((n->trVectorSection[i + 1].param[8] - playerT[0])*2048 + n->trVectorSection[i + 1].param[10]);
                    linie[lPtr++] = (n->trVectorSection[i + 1].param[11] + wysokoscSieci);
                    linie[lPtr++] = (((-n->trVectorSection[i + 1].param[9] - playerT[1])*2048 - n->trVectorSection[i + 1].param[12]));
                }
                if (n->TrPinS[1] != 0) {
                    linie[lPtr++] = ((n->trVectorSection[n->iTrv - 1].param[8] - playerT[0])*2048 + n->trVectorSection[n->iTrv - 1].param[10]);
                    linie[lPtr++] = (n->trVectorSection[n->iTrv - 1].param[11] + wysokoscSieci);
                    linie[lPtr++] = (((-n->trVectorSection[n->iTrv - 1].param[9] - playerT[1])*2048 - n->trVectorSection[n->iTrv - 1].param[12]));

                    linie[lPtr++] = ((trackNodes[n->TrPinS[1]].UiD[4] - playerT[0])*2048 + trackNodes[n->TrPinS[1]].UiD[6]);
                    linie[lPtr++] = (trackNodes[n->TrPinS[1]].UiD[7] + wysokoscSieci);
                    linie[lPtr++] = (((-trackNodes[n->TrPinS[1]].UiD[5] - playerT[1])*2048 - trackNodes[n->TrPinS[1]].UiD[8]));
                }
            } else if (n->typ == 0) {
                konce[kPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                konce[kPtr++] = (n->UiD[7]);
                konce[kPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);

                konce[kPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                konce[kPtr++] = (n->UiD[7] + wysokoscSieci);
                konce[kPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
            } else if (n->typ == 2) {
                punkty[pPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                punkty[pPtr++] = (n->UiD[7]);
                punkty[pPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);

                punkty[pPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                punkty[pPtr++] = (n->UiD[7] + wysokoscSieci);
                punkty[pPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
            }
        }
        linieSieci.setMaterial(0.5, 0.5, 0.5);
        linieSieci.init(linie, lPtr, linieSieci.V, GL_LINES);

        konceSieci.setMaterial(0.0, 0.0, 1.0);
        konceSieci.init(konce, kPtr, konceSieci.V, GL_LINES);

        punktySieci.setMaterial(1.0, 0.0, 0.0);
        punktySieci.init(punkty, pPtr, punktySieci.V, GL_LINES);

        delete linie;
        delete konce;
        delete punkty;
    }

    Mat4::identity(gluu->objStrMatrix);
    gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    linieSieci.render();
    konceSieci.render();
    punktySieci.render();
}

void TDB::renderLines(GLUU *gluu, float* playerT) {

    if (!loaded) return;
    int hash = playerT[0] * 10000 + playerT[1];
    if (!sectionLines.loaded || sectionHash != hash) {
        Vector3f p;
        Vector3f o;
        sectionHash = hash;
        
        int len = 0;

        for (int j = 0; j < iTRnodes; j++) {
            TRnode* n = &trackNodes[j];
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                for (int i = 0; i < n->iTrv; i++) {
                    len +=6;
                }
            }
        }
        
        float* punkty = new float[len];
        float* ptr = punkty;     
        
        for (int j = 0; j < iTRnodes; j++) {
            TRnode* n = &trackNodes[j];
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                for (int i = 0; i < n->iTrv; i++) {
                    p.set(
                            (n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10],
                            n->trVectorSection[i].param[11],
                            (-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12]
                            );
                    o.set(
                            n->trVectorSection[i].param[13],
                            n->trVectorSection[i].param[14],
                            n->trVectorSection[i].param[15]
                            );
                    drawLine(gluu, ptr, p, o, (int) n->trVectorSection[i].param[0]);
                }
            } /*else if (n.typ == 0 || n.typ == 2) {
                float x = (n.UiD[4] - aktwx)*2048 + n.UiD[6];
                float y = n.UiD[7];
                float z = (-n.UiD[5] - aktwz)*2048 - n.UiD[8];
                if (Math.sqrt(x * x + z * z) > 2000) continue;

                renderT.setSmoothing(false);
                renderT.begin3DRendering();
                if (n.typ == 0) renderT.setColor(0.0f, 0.0f, 1.0f, 1.0f);
                if (n.typ == 2) renderT.setColor(1.0f, 0.0f, 0.0f, 1.0f);
                renderT.draw3D("" + j, x, y + 4.5f, z, 0.02f);
                renderT.end3DRendering();
            }*/
        }
        sectionLines.setMaterial(1.0, 1.0, 0.0);
        qDebug() << ptr-punkty;
        sectionLines.init(punkty, ptr-punkty, sectionLines.V, GL_LINES);

        delete punkty;
    }

    Mat4::identity(gluu->objStrMatrix);
    gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    sectionLines.render();
}

void TDB::drawLine(GLUU *gluu, float* &ptr, Vector3f p, Vector3f o, int idx) {

    //if (sqrt(p.x * p.x + p.z * p.z) > 3000) return;

    //gl.glTranslatef(p.x, p.y, p.z);

    /*Vector4f q = Inne.eToQ(new Vector3f((float) M_PI, o.y, o.z));
    float scale = (float) sqrt(q.x * q.x + q.y * q.y + q.z * q.z);
    float angle = (float) ((acos(q.c)*360) / M_PI);

    gl.glRotatef(-angle, q.x*scale, q.y*scale, q.z * scale);
    gl.glRotatef(o.x * 180 / (float) M_PI, 1, 0, 0);*/
    //
    //float[] m = Inne.rotation(new Vector3f(-o.x,-o.y+(float)(Math.PI),o.z));
    //float[] m = Inne.rotation(new Vector3f(-o.x,0,0));
    //gl.glMultMatrixf(m,0);

    //gl.glBegin(GL_LINES);
    *ptr++ = p.x;
    *ptr++ = p.y;
    *ptr++ = p.z;
    *ptr++ = p.x;
    *ptr++ = p.y+2;
    *ptr++ = p.z;
    //gl.glEnd();

    //if (tsection.sekcja.containsKey(idx))
    //    tsection.sekcja.get(idx).drawSection(gl);

}

TDB::TDB(const TDB& orig) {
}

TDB::~TDB() {
}

