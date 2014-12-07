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
    //trackNodes = new TRnode[iTRnodes + 100]; //przydzielenie pamieci
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

int TDB::findNearestNode(int &x, int &z, float* p, float* q) {
    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = &trackNodes[j];
        if (n->typ == 0) {
            float lenx = ((n->UiD[4] - x)*2048 + n->UiD[6] - p[0]);
            float leny = (n->UiD[7]) - p[1];
            float lenz = ((-n->UiD[5] - z)*2048 - n->UiD[8] - p[2]);
            if (fabs(lenx) < 2 && fabs(leny) < 2 && fabs(lenz) < 2) {
                //qDebug() << ":"<<len;
                x = n->UiD[4];
                z = -n->UiD[5];
                p[0] = n->UiD[6];
                p[1] = n->UiD[7];
                p[2] = -n->UiD[8];

                q[0] = n->UiD[9];
                q[1] = n->UiD[10];
                q[2] = n->UiD[11];
                //Quat::rotateY(q, q, n->UiD[10]);
                return j;
            }
        }
    }
    return -1;
}

int TDB::appendTrack(int id, int r, int sect, int uid) {
    TRnode* endNode = &trackNodes[id];
    float p[3];
    int ends[2];
    ends[0] = 0;
    ends[1] = 1;

    if (endNode->typ == 0) {
        int kierunek = endNode->TrPinK[0];
        TRnode* n = &trackNodes[endNode->TrPinS[0]];
        if (n->typ != 1) {
            qDebug() << "tdb error";
            return -1;
        }

        qDebug() << kierunek;
        n->iTrv++;
        TRnode::TRSect *newV = new TRnode::TRSect[n->iTrv];

        if (kierunek == 1) {
            std::copy(n->trVectorSection, n->trVectorSection + n->iTrv - 1, newV + 1);
        } else {
            std::copy(n->trVectorSection, n->trVectorSection + n->iTrv - 1, newV);
        }
        delete n->trVectorSection;
        n->trVectorSection = newV;

        float dlugosc = this->tsection->sekcja[sect]->getDlugosc();
        //qDebug() << dlugosc;
        Vector3f *aa = this->tsection->sekcja[sect]->getDrawPosition(dlugosc);
        aa->rotateY(M_PI + endNode->UiD[10], 0);
        float angle = this->tsection->sekcja[sect]->getAngle();
        int sid = sect;

        p[0] = endNode->UiD[6] + aa->x;
        p[1] = endNode->UiD[7] + aa->y;
        p[2] = endNode->UiD[8] - aa->z;
        int x = endNode->UiD[4];
        int z = endNode->UiD[5];
        int xx = endNode->UiD[4];
        int zz = endNode->UiD[5];
        float pp[3];
        pp[0] = endNode->UiD[6];
        pp[1] = endNode->UiD[7];
        pp[2] = endNode->UiD[8];
        Game::check_coords(x, z, p);

        float vangle = 0;
        float * vector = n->trVectorSection[n->iTrv - 1].param;
        if (kierunek == 1) {
            vector = n->trVectorSection[0].param;
            int tmp = ends[0];
            ends[0] = ends[1];
            ends[1] = tmp;
            if (angle != 0) sid++;
            vangle = angle + M_PI;
            xx = x;
            zz = z;
            pp[0] = p[0];
            pp[1] = p[1];
            pp[2] = p[2];
        }

        vector[0] = sid;
        vector[1] = r;
        vector[2] = endNode->UiD[4];
        vector[3] = endNode->UiD[5];
        vector[4] = uid;
        vector[5] = ends[0];
        vector[6] = ends[1];
        vector[7] = 0;
        vector[8] = xx;
        vector[9] = zz;
        vector[10] = pp[0];
        vector[11] = pp[1];
        vector[12] = pp[2];
        vector[13] = endNode->UiD[9];
        vector[14] = endNode->UiD[10] + vangle;
        vector[15] = endNode->UiD[11];

        endNode->UiD[0] = endNode->UiD[4];
        endNode->UiD[1] = endNode->UiD[5];
        endNode->UiD[2] = uid;
        endNode->UiD[3] = 1;
        endNode->UiD[4] = x;
        endNode->UiD[5] = z;
        endNode->UiD[6] = p[0];
        endNode->UiD[7] = p[1];
        endNode->UiD[8] = p[2];
        endNode->UiD[9] = endNode->UiD[9];
        endNode->UiD[10] = endNode->UiD[10] + angle;
        endNode->UiD[11] = endNode->UiD[11];
    }
    return id;
}

int TDB::newTrack(int x, int z, float* p, float* qe, int r, int sect, int uid) {

    //TrackShape* shp = this->tsection->shape[r->value];
    //qDebug() << shp->filename;

    int end1Id = ++this->iTRnodes;
    int vecId = ++this->iTRnodes;
    int end2Id = ++this->iTRnodes;

    z = -z;
    ////////////////////////////////////
    TRnode *newNode = &this->trackNodes[end1Id];
    newNode->typ = 0;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = 0;
    newNode->UiD[4] = x;
    newNode->UiD[5] = z;
    newNode->UiD[6] = p[0];
    newNode->UiD[7] = p[1];
    newNode->UiD[8] = -p[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + M_PI;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 1;

    /////////////////////////////////////////////////////
    newNode = &this->trackNodes[vecId];
    qDebug() << vecId;
    newNode->typ = 1;
    newNode->iTrv = 1;
    newNode->trVectorSection = new TRnode::TRSect[newNode->iTrv];
    newNode->trVectorSection[0].param[0] = sect;
    newNode->trVectorSection[0].param[1] = r;
    newNode->trVectorSection[0].param[2] = x;
    newNode->trVectorSection[0].param[3] = z;
    newNode->trVectorSection[0].param[4] = uid;
    newNode->trVectorSection[0].param[5] = 0;
    newNode->trVectorSection[0].param[6] = 1;
    newNode->trVectorSection[0].param[7] = 0;
    newNode->trVectorSection[0].param[8] = x;
    newNode->trVectorSection[0].param[9] = z;
    newNode->trVectorSection[0].param[10] = p[0];
    newNode->trVectorSection[0].param[11] = p[1];
    newNode->trVectorSection[0].param[12] = -p[2];
    newNode->trVectorSection[0].param[13] = qe[0];
    newNode->trVectorSection[0].param[14] = qe[1];
    newNode->trVectorSection[0].param[15] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrP2 = 1;
    newNode->TrPinS[0] = end1Id;
    newNode->TrPinK[0] = 1;
    newNode->TrPinS[1] = end2Id;
    newNode->TrPinK[1] = 1;
    /////////////////////////////////////////////////////
    qDebug() << sect;
    float dlugosc = this->tsection->sekcja[sect]->getDlugosc();
    qDebug() << dlugosc;
    Vector3f *aa = this->tsection->sekcja[sect]->getDrawPosition(dlugosc);
    aa->rotateY(M_PI + qe[1], 0);
    float angle = this->tsection->sekcja[sect]->getAngle();
    //Quat::
    float pp[3];
    pp[0] = p[0] + aa->x;
    pp[1] = p[1] + aa->y;
    pp[2] = -p[2] - aa->z;
    Game::check_coords(x, z, pp);

    newNode = &this->trackNodes[end2Id];
    newNode->typ = 0;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = 1;
    newNode->UiD[4] = x;
    newNode->UiD[5] = z;
    newNode->UiD[6] = pp[0];
    newNode->UiD[7] = pp[1];
    newNode->UiD[8] = pp[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + angle;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 0;

    return end2Id;
}

bool TDB::placeTrack(int x, int z, float* p, float* q, Ref::RefItem* r, int uid) {
    float qe[3];
    qe[0] = 0;
    qe[1] = 0;
    qe[2] = 0;
    int append = findNearestNode(x, z, p, (float*) &qe);
    Quat::rotateY(q, q, -qe[1]);
    bool b;
    TrackShape* shp = this->tsection->shape[r->value];
    qDebug() << shp->filename;
    float pp[3];
    int endp;
   // if (append > 0) {
        Vector3f aa(shp->path[0].pos[0], shp->path[0].pos[1], shp->path[0].pos[2]);
        aa.rotateY(-qe[1], 0);
        p[0] += aa.x;
        p[1] += shp->path[0].pos[1];
        p[2] -= aa.z;
   /*     endp = appendTrack(append, r->value, shp->path[0].sect[0], uid);
    } else {
*/
//p[0] += shp->path[0].pos[0];
//p[1] += shp->path[0].pos[1];
//p[2] += shp->path[0].pos[2];
            
        for (int i = 0; i < shp->numpaths; i++) {
            //aa.set(0,0,0);
            aa.set(shp->path[i].pos[0], shp->path[i].pos[1], shp->path[i].pos[2]);
            aa.rotateY(-qe[1], 0);
            pp[0] = p[0] + aa.x;
            pp[1] = p[1] + shp->path[i].pos[1];
            pp[2] = p[2] - aa.z;
            endp = newTrack(x, z, pp, qe, r->value, shp->path[i].sect[0], uid);
            for (int j = 1; j < shp->path[i].n; j++) {
                if (endp > 0) {
                    endp = appendTrack(endp, r->value, shp->path[i].sect[j], uid);
                }
            }
        }
  //  }

    ////////////////////////////////////////////////////
    //save();
    refresh();
    return true;
}

void TDB::refresh() {
    isInitSectLines = false;
    isInitLines = false;
}

void TDB::renderAll(GLUU *gluu, float* playerT) {

    if (!loaded) return;
    int hash = playerT[0] * 10000 + playerT[1];
    if (!isInitLines || lineHash != hash) {
        TRnode *n;
        int lLen = 0, kLen = 0, pLen = 0;
        //qDebug() <<"update";
        lineHash = hash;
        isInitLines = true;

        for (int i = 1; i <= iTRnodes; i++) {
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

        for (int i = 1; i <= iTRnodes; i++) {
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
    if (!sectionLines.loaded || sectionHash != hash || !isInitSectLines) {
        Vector3f p;
        Vector3f o;
        sectionHash = hash;
        isInitSectLines = true;

        int len = 0;

        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = &trackNodes[j];
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                for (int i = 0; i < n->iTrv; i++) {
                    p.set(
                            (n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10],
                            n->trVectorSection[i].param[11],
                            (-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12]
                            );
                    if (sqrt(p.x * p.x + p.z * p.z) > 3000) continue;
                    len += getLineBufferSize((int) n->trVectorSection[i].param[0]);
                }
            }
        }

        float* punkty = new float[len];
        float* ptr = punkty;

        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = &trackNodes[j];
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                //qDebug() << j;
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
                    if (sqrt(p.x * p.x + p.z * p.z) > 3000) continue;
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
        sectionLines.init(punkty, ptr - punkty, sectionLines.V, GL_LINES);

        delete punkty;
    }

    Mat4::identity(gluu->objStrMatrix);
    gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    sectionLines.render();
}

int TDB::getLineBufferSize(int idx) {
    try {
        return tsection->sekcja.at(idx)->getLineBufferSize() + 6;
    } catch (const std::out_of_range& oor) {

    }
    return 6;
}

void TDB::drawLine(GLUU *gluu, float* &ptr, Vector3f p, Vector3f o, int idx) {

    float matrix[16];
    float q[4];
    q[0] = q[1] = q[2] = 0;
    q[3] = 1;
    float rot[3];
    rot[0] = M_PI;
    rot[1] = -o.y;
    rot[2] = o.z;

    Quat::fromRotationXYZ(q, rot);
    Mat4::fromRotationTranslation(matrix, q, reinterpret_cast<float *> (&p));
    Mat4::rotate(matrix, matrix, o.x, 1, 0, 0);

    float point1[3];
    point1[0] = 0;
    point1[1] = 0;
    point1[2] = 0;
    float point2[3];
    point2[0] = 0;
    point2[1] = 2;
    point2[2] = 0;
    Vec3::transformMat4(point1, point1, matrix);
    Vec3::transformMat4(point2, point2, matrix);
    *ptr++ = point1[0];
    *ptr++ = point1[1];
    *ptr++ = point1[2];
    *ptr++ = point2[0];
    *ptr++ = point2[1];
    *ptr++ = point2[2];

    try {
        tsection->sekcja.at(idx)->drawSection(ptr, matrix);
    } catch (const std::out_of_range& oor) {
        qDebug() << "nie ma sekcji " << idx;
    }

}

void TDB::save() {

    //while(usunNulle());

    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/" + Game::route + ".tdb";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);

    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setRealNumberPrecision(8);
    //out.setRealNumberNotation(QTextStream::FixedNotation);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0T0t______\n\n";
    out << "TrackDB (\n";
    //out << "	Serial ( "+serial+" )\n";
    out << "	TrackNodes ( " << (this->iTRnodes) << "\n";

    for (int i = 1; i <= this->iTRnodes; i++) {
        out << "		TrackNode ( " << i << "\n";

        switch (trackNodes[i].typ) {
            case 0:
                out << "			TrEndNode ( " << trackNodes[i].args[0] << " )\n";
                out << "			UiD ( ";
                for (int j = 0; j < 12; j++) {
                    out << trackNodes[i].UiD[j] << " ";
                }
                out << ")\n";
                out << "			TrPins ( 1 0\n";
                out << "				TrPin ( " << trackNodes[i].TrPinS[0] << " " << trackNodes[i].TrPinK[0] << " )\n";
                out << "			)\n";
                break;
            case 1:
                out << "			TrVectorNode ( \n";
                out << "				TrVectorSections ( " << trackNodes[i].iTrv << "";
                for (int j = 0; j < trackNodes[i].iTrv; j++) {
                    for (int jj = 0; jj < 7; jj++) {
                        out << " " << trackNodes[i].trVectorSection[j].param[jj];
                    }
                    out << " 00";
                    for (int jj = 8; jj < 16; jj++) {
                        out << " " << trackNodes[i].trVectorSection[j].param[jj];
                    }
                    if (j % 10 == 0 && j > 0)
                        out << "\n					";
                }
                out << " )\n";


                /*if(trackNodes[i].TrItemRefs != null){
                    out << "				TrItemRefs ( "+trackNodes[i].TrItemRefs.length+"\n";
                    for(int j = 0; j<trackNodes[i].TrItemRefs.length; j++){
                        out << "					TrItemRef ( "+trackNodes[i].TrItemRefs[j]+" )\n";
                    }
                    out << "				)\n";
                }*/
                out << "			)\n";
                out << "			TrPins ( 1 1\n";
                out << "				TrPin ( " << trackNodes[i].TrPinS[0] << " " << trackNodes[i].TrPinK[0] << " )\n";
                out << "				TrPin ( " << trackNodes[i].TrPinS[1] << " " << trackNodes[i].TrPinK[1] << " )\n";
                out << "			)\n";
                break;
            case 2:
                out << "			TrJunctionNode ( " << trackNodes[i].args[0] << " " << trackNodes[i].args[1] << " " << trackNodes[i].args[2] << " )\n";
                out << "			UiD ( ";
                for (int j = 0; j < 12; j++) {
                    out << trackNodes[i].UiD[j] << " ";
                }
                out << ")\n";
                out << "			TrPins ( 1 2\n";
                out << "				TrPin ( " << trackNodes[i].TrPinS[0] << " " << trackNodes[i].TrPinK[0] << " )\n";
                out << "				TrPin ( " << trackNodes[i].TrPinS[1] << " " << trackNodes[i].TrPinK[1] << " )\n";
                out << "				TrPin ( " << trackNodes[i].TrPinS[2] << " " << trackNodes[i].TrPinK[2] << " )\n";
                out << "			)\n";
                break;
        }

        out << "		)\n";
    }
    out << "	)\n";
    out << ")";
    file.close();

    qDebug() << "Zapisane";
}

TDB::TDB(const TDB& orig) {
}

TDB::~TDB() {
}

