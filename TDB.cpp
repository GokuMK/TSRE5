#include "TDB.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "GLMatrix.h"
#include "DynTrackObj.h"
#include "TrackShape.h"

TDB::TDB(TSectionDAT* tsection, bool road, QString path) {
    loaded = false;
    this->road = road;
    serial = 0;
    wysokoscSieci = 4;
    iTRitems = 0;
    qDebug() << "Wczytywanie pliku tdb: " << path;

    if(tsection == NULL)
        this->tsection = new TSectionDAT();
    else
        this->tsection = tsection;
    
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
    
    while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
        qDebug() << "sh " << sh;
        if(sh == "tracknodes"){
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
                trackNodes[t] = new TRnode();
                switch (sh.length()) {// wybranie typu sciezki ^^
                    case 9:
                        trackNodes[t]->typ = 0; //typ endnode
                        sh = "UiD";
                        ParserX::szukajsekcji1(sh, bufor);
                        for (ii = 0; ii < 12; ii++) {
                            trackNodes[t]->UiD[ii] = ParserX::parsujr(bufor);
                        }
                        sh = "TrPins";
                        ParserX::szukajsekcji1(sh, bufor);
                        trpin(trackNodes[t], bufor);
                        ParserX::pominsekcje(bufor);
                        ParserX::pominsekcje(bufor);
                        break;
                    case 12:
                        trackNodes[t]->typ = 1; //typ vector 

                        while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
                            if(sh == "trvectorsections"){
                                uu = (int) ParserX::parsujr(bufor);
                                trackNodes[t]->iTrv = uu;
                                trackNodes[t]->trVectorSection = new TRnode::TRSect[uu]; // przydzielenie pamieci dla sciezki

                                for (j = 0; j < uu; j++) {
                                    for (ii = 0; ii < 16; ii++) {
                                        xx = ParserX::parsujr(bufor);
                                        trackNodes[t]->trVectorSection[j].param[ii] = xx;
                                    }
                                    //System.out.println(
                                    //this.TrackNodes[t].TrVectorSection[j].param[13]+" "+
                                    //this.TrackNodes[t].TrVectorSection[j].param[14]+" "+
                                    //this.TrackNodes[t].TrVectorSection[j].param[15]);
                                }
                            }
                            if(sh == "tritemrefs"){
                                uu = (int) ParserX::parsujr(bufor);
                                trackNodes[t]->iTri = uu;
                                trackNodes[t]->trItemRef = new int[uu]; // przydzielenie pamieci dla sciezki

                                for (j = 0; j < uu; j++) {
                                    trackNodes[t]->trItemRef[j] = ParserX::parsujr(bufor);
                                }
                                ParserX::pominsekcje(bufor);
                            }
                            ParserX::pominsekcje(bufor);
                        }
                        sh = "TrPins";
                        ParserX::szukajsekcji1(sh, bufor);
                        trpin(trackNodes[t], bufor);
                        ParserX::pominsekcje(bufor);
                        ParserX::pominsekcje(bufor);
                        break;
                    case 14:
                        trackNodes[t]->typ = 2; //typ rozjazd
                        sh = "UiD";
                        ParserX::szukajsekcji1(sh, bufor);
                        for (ii = 0; ii < 12; ii++) {
                            trackNodes[t]->UiD[ii] = ParserX::parsujr(bufor);
                        }
                        sh = "TrPins";
                        ParserX::szukajsekcji1(sh, bufor);
                        trpin(trackNodes[t], bufor);
                        ParserX::pominsekcje(bufor);
                        ParserX::pominsekcje(bufor);
                        break;
                    default:
                        qDebug() << "Nieprawidlowa sciezka -> ERROR";
                        trackNodes[t] = NULL;
                        break;
                }
                ParserX::pominsekcje(bufor);
            }
            ParserX::pominsekcje(bufor);
        }
        if(sh == "serial"){
            this->serial = (int) ParserX::parsujr(bufor);
            ParserX::pominsekcje(bufor);
        }
        if(sh == "tritemtable"){
            iTRitems = (int) ParserX::parsujr(bufor); //odczytanie ilosci sciezek
            
            TRitem* nowy;// = new TRitem();
            
            for (int i = 0; i < iTRitems; i++) {
                sh = ParserX::nazwasekcji(bufor).toLower();

                if (sh == "") {
                    qDebug() << "tritemtable Fail";
                    ParserX::pominsekcje(bufor);
                    break;
                }
                
                nowy = new TRitem();
                if(!nowy->init(sh)){
                    ParserX::pominsekcje(bufor);
                    qDebug() << "-" << sh;
                    continue;
                }

                while (!((sh = ParserX::nazwasekcji_inside(bufor).toLower()) == "")) {
                    nowy->set(sh, bufor);
                    ParserX::pominsekcje(bufor);
                }
                this->trackItems[nowy->trItemId] = nowy;

                ParserX::pominsekcje(bufor);
                continue;
            }
            
            ParserX::pominsekcje(bufor);
        }
    }

    
    //save();
    loaded = true;
    return;
}

void TDB::trpin(TRnode *tr, FileBuffer* bufor) {

    tr->TrP1 = (int) ParserX::parsujr(bufor);
    tr->TrP2 = (int) ParserX::parsujr(bufor);

    for (int i = 0; i < (tr->TrP1 + tr->TrP2); i++) {
        tr->TrPinS[i] = (int) ParserX::parsujr(bufor);
        tr->TrPinK[i] = (int) ParserX::parsujr(bufor);
    }
}

void TDB::fillDynTrack(DynTrackObj* track){
    int tType[5];
    float tAngle[5];
    float tRadius[5];
    int count = 0;
    
    int foundIdx = -1;
    TrackShape::SectionIdx* newRShape = NULL;
    for (int i = 0; i < 5; i++) {
        if(track->sections[i].sectIdx > 1000000) {
            continue;
        }
        tType[count] = track->sections[i].type;
        tAngle[count] = track->sections[i].a;
        tRadius[count] = track->sections[i].r;
        count++;
    }

    bool success;
    for(int i = 40000; i< tsection->routeShapes; i++){
        success = true;
        if(tsection->shape[i] == NULL) continue;
        
        if(tsection->shape[i]->path[0].n == count){
            for(int j = 0; j<count; j++){
                if(!tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->type == tType[j]){
                    success = false;
                    break;
                }
                if(tType[j] == 1){
                    if(!tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->angle == tAngle[j])
                        if(!tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->radius == tRadius[j]){
                            success = false;
                            break;
                        }
                } 
                else if(tType[j] == 0){
                    if(!tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->size == tAngle[j])
                        if(!tsection->sekcja[tsection->shape[i]->path[0].sect[j]]->val1 == tRadius[j]){
                            success = false;
                            break;
                        }
                }
            }
            ////
            if(success){
                foundIdx = i;
                break;
            }
        }
    }
    //qDebug() << "foundIdx "<<foundIdx;
    if(foundIdx == -1){
        newRShape = new TrackShape::SectionIdx[1];
        newRShape->n = count;
        
        for(int j = 0; j<count; j++){
            TSection* newSect0 = new TSection(tsection->routeMaxIdx);
            TSection* newSect1 = new TSection(tsection->routeMaxIdx+1);
            
            if(tType[j] == 1){
                newSect0->type = 1;
                newSect0->angle = -fabs(tAngle[j]);
                newSect0->radius = tRadius[j];
                newSect1->type = 1;
                newSect1->angle = fabs(tAngle[j]);
                newSect1->radius = tRadius[j];
                tsection->sekcja[newSect0->id] = newSect0;
                tsection->sekcja[newSect1->id] = newSect1;
                
                if(tAngle[j]<0) 
                    newRShape[0].sect[j] = newSect0->id;
                else 
                    newRShape[0].sect[j] = newSect1->id;
            } 
            else if(tType[j] == 0){
                newSect0->type = 0;
                newSect0->size = tAngle[j];
                newSect0->val1 = tRadius[j];
                
                tsection->sekcja[newSect0->id] = newSect0;
                newRShape[0].sect[j] = newSect0->id;
            }
            //qDebug() << "sid "<< newSect0->id;
            //qDebug() << "sid "<< newSect1->id;
            tsection->routeMaxIdx+=2;
        }
        foundIdx = tsection->routeShapes;
        TrackShape* newShape = new TrackShape();
        newShape->dyntrack = true;
        newShape->path = newRShape;
        newRShape->pos[0] = 0;
        newRShape->pos[1] = 0;
        newRShape->pos[2] = 0;
        newShape->numpaths = 1;
        tsection->shape[tsection->routeShapes++] = newShape;
    }
    //qDebug() << "foundIdx "<<foundIdx;
    
    track->sectionIdx = foundIdx;
}

int TDB::findNearestNode(int &x, int &z, float* p, float* q) {
    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = trackNodes[j];
        if(n == NULL) continue;
        if (n->typ == 0 || n->typ == 2) {
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

                q[0] = 0;//n->UiD[9]; //fix ??????????
                q[1] = n->UiD[10];
                q[2] = n->UiD[11];
                //Quat::rotateY(q, q, n->UiD[10]);
                return j;
            }
        }
    }
    return -1;
}

int TDB::appendTrack(int id, int* ends, int r, int sect, int uid) {
    TRnode* endNode = trackNodes[id];
    float p[3];

    if (endNode->typ == 0) {
        int kierunek = endNode->TrPinK[0];
        TRnode* n = trackNodes[endNode->TrPinS[0]];
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
        //qDebug() <<"sect"<< sect;
        float dlugosc = this->tsection->sekcja[sect]->getDlugosc();
        //qDebug() <<"dlugosc"<< dlugosc;
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
        endNode->UiD[3] = ends[1];
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

int TDB::newTrack(int x, int z, float* p, float* qe, int* ends, int r, int sect, int uid) {
    newTrack(x, z,  p,  qe, ends, r, sect, uid, NULL);
}
int TDB::newTrack(int x, int z, float* p, float* qe, int* ends, int r, int sect, int uid, int* start) {

    //TrackShape* shp = this->tsection->shape[r->value];
    //qDebug() << shp->filename;

    int end1Id = ++this->iTRnodes;
    int vecId = ++this->iTRnodes;
    int end2Id = ++this->iTRnodes;

    int xx = x;
    int zz = z;
    float pp[3];
    pp[0] = p[0]; pp[1] = p[1]; pp[2] = p[2];
    Game::check_coords(xx, zz, pp);
    z = -z;
    zz = -zz;
    ////////////////////////////////////
    this->trackNodes[end1Id] = new TRnode();
    TRnode *newNode = this->trackNodes[end1Id];
    newNode->typ = 0;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = ends[0];
    newNode->UiD[4] = xx;
    newNode->UiD[5] = zz;
    newNode->UiD[6] = pp[0];
    newNode->UiD[7] = pp[1];
    newNode->UiD[8] = -pp[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + M_PI;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 1;

    /////////////////////////////////////////////////////
    this->trackNodes[vecId] = new TRnode();
    newNode = this->trackNodes[vecId];
    qDebug() << vecId;
    newNode->typ = 1;
    newNode->iTrv = 1;
    newNode->trVectorSection = new TRnode::TRSect[newNode->iTrv];
    newNode->trVectorSection[0].param[0] = sect;
    newNode->trVectorSection[0].param[1] = r;
    newNode->trVectorSection[0].param[2] = x;
    newNode->trVectorSection[0].param[3] = z;
    newNode->trVectorSection[0].param[4] = uid;
    newNode->trVectorSection[0].param[5] = ends[0];
    newNode->trVectorSection[0].param[6] = ends[1];
    newNode->trVectorSection[0].param[7] = 0;
    newNode->trVectorSection[0].param[8] = xx;
    newNode->trVectorSection[0].param[9] = zz;
    newNode->trVectorSection[0].param[10] = pp[0];
    newNode->trVectorSection[0].param[11] = pp[1];
    newNode->trVectorSection[0].param[12] = -pp[2];
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
    //if(qe[1] > M_PI)
    //    aa->rotateX(-qe[0], 0);
    //else
    
    aa->rotateX(qe[0], 0);  
    aa->rotateY(M_PI + qe[1], 0);
    
    
    float angle = this->tsection->sekcja[sect]->getAngle();
    //Quat::
    //float pp[3];
    pp[0] = pp[0] + aa->x;
    pp[1] = pp[1] + aa->y;
    pp[2] = -pp[2] - aa->z;
    Game::check_coords(xx, zz, pp);

    this->trackNodes[end2Id] = new TRnode();
    newNode = this->trackNodes[end2Id];
    newNode->typ = 0;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = ends[1];
    newNode->UiD[4] = xx;
    newNode->UiD[5] = zz;
    newNode->UiD[6] = pp[0];
    newNode->UiD[7] = pp[1];
    newNode->UiD[8] = pp[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + angle;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 0;

    if(start != NULL)
        *start = end1Id;
    return end2Id;
}

int TDB::newJunction(int x, int z, float* p, float* qe, int r, int uid, int end) {

    //TrackShape* shp = this->tsection->shape[r->value];
    //qDebug() << shp->filename;

    int junction = ++this->iTRnodes;
    
    int xx = x;
    int zz = z;
    float pp[3];
    pp[0] = p[0]; pp[1] = p[1]; pp[2] = p[2];
    Game::check_coords(xx, zz, pp);
    z = -z;
    zz = -zz;
    ////////////////////////////////////
    this->trackNodes[junction] = new TRnode();
    TRnode *newNode = this->trackNodes[junction];
    newNode->typ = 2;
    newNode->UiD[0] = x;
    newNode->UiD[1] = z;
    newNode->UiD[2] = uid;
    newNode->UiD[3] = end;
    newNode->UiD[4] = xx;
    newNode->UiD[5] = zz;
    newNode->UiD[6] = pp[0];
    newNode->UiD[7] = pp[1];
    newNode->UiD[8] = -pp[2];
    newNode->UiD[9] = qe[0];
    newNode->UiD[10] = qe[1] + M_PI;
    newNode->UiD[11] = qe[2];

    newNode->TrP1 = 1;
    newNode->TrP2 = 2;
    newNode->TrPinS[0] = 0;
    newNode->TrPinK[0] = 0;
    newNode->TrPinS[1] = 0;
    newNode->TrPinK[1] = 0;
    newNode->TrPinS[2] = 0;
    newNode->TrPinK[2] = 0;
    
    newNode->args[1] = r;
    
    return junction;
}

int TDB::joinTracks(int iendp) {
    TRnode* endp = trackNodes[iendp];

    if(endp->typ == 0){
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if(n == NULL) continue;
            if (n->typ == 0) {
                if (j == iendp)
                    continue;
                if (endp->equals(n)) {
                    qDebug() << "polacze " << iendp << " " << j;
                    qDebug() << n->TrPinS[0] << " " << n->TrPinK[0];
                    qDebug() << endp->TrPinS[0] << " " << endp->TrPinK[0];
                    joinVectorSections(endp->TrPinS[0], n->TrPinS[0]);
                    return 0;
                }
            }
        }
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if(n == NULL) continue;
            if (n->typ == 2) {
                if (j == iendp)
                    continue;
                if (endp->equalsIgnoreType(n)) {
                    qDebug() << "polacze rozjazd " << iendp << " " << j;
                    appendToJunction(j, iendp, 0);
                    return 0;
                }
            }
        }
    }
    
    if(endp->typ == 2){
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if(n == NULL) continue;
            if (n->typ == 0) {
                if (j == iendp)
                    continue;
                if (endp->equalsIgnoreType(n)) {
                    qDebug() << "polacze rozjazd " << iendp << " " << j;
                    appendToJunction(iendp, j, 0);
                    return 0;
                }
            }
        }
    }
}

int TDB::fillJunction(int id){
    
    return 0;
}

int TDB::joinVectorSections(int id1, int id2) {
    TRnode* section1 = trackNodes[id1];
    TRnode* section2 = trackNodes[id2];
    if(section1 == section2)
        return 0;
    int endpk1 = section1->TrPinS[1];
    int endpk2 = section2->TrPinS[0];
    TRnode* section1e1 = trackNodes[section1->TrPinS[0]];
    TRnode* section1e2 = trackNodes[section1->TrPinS[1]];
    TRnode* section2e1 = trackNodes[section2->TrPinS[0]];
    TRnode* section2e2 = trackNodes[section2->TrPinS[1]];
    
    if (section1e2->equals(section2e1)) {
        qDebug() << "ok";
    }
    else if (section2e2->equals(section1e1)) {
        qDebug() << "switch";
        return joinVectorSections(id2, id1);
    }
    else if (section1e2->equals(section2e2)) {
        qDebug() << "rot2";
        rotate(id2);
        return joinVectorSections(id1, id2);
    }
    else if (section1e1->equals(section2e1)) {
        qDebug() << "rot1";
        rotate(id1);
        return joinVectorSections(id1, id2);
    }

    TRnode::TRSect *newV = new TRnode::TRSect[section1->iTrv + section2->iTrv];

    std::copy(section1->trVectorSection, section1->trVectorSection + section1->iTrv, newV);
    std::copy(section2->trVectorSection, section2->trVectorSection + section2->iTrv, newV + section1->iTrv);
    section1->iTrv = section1->iTrv + section2->iTrv;
    
    delete section1->trVectorSection;
    delete section2->trVectorSection;
    
    
    section1->trVectorSection = newV;
    section1->TrPinS[1] = section2->TrPinS[1];
    section1->TrPinK[1] = section2->TrPinK[1];
    section2e2->podmienTrPin(id2, id1);
    //section2e2->TrPinS[0] = section1e2->TrPinS[0];
    
    trackNodes[id2] = NULL;
    trackNodes[endpk1] = NULL;
    trackNodes[endpk2] = NULL;
}

int TDB::splitVectorSection(int id, int j){
    TRnode* vect = trackNodes[id];
    TRnode* end2 = trackNodes[vect->TrPinS[1]];
    TRnode* newNode;
    
    int end1Id = ++this->iTRnodes;
    int vecId = ++this->iTRnodes;
    int end2Id = ++this->iTRnodes;
    end2->podmienTrPin(id, vecId);
    
    this->trackNodes[vecId] = new TRnode();
    newNode = this->trackNodes[vecId];
    newNode->typ = 1;
    newNode->iTrv = vect->iTrv - j;
    
    TRnode::TRSect *newV = new TRnode::TRSect[newNode->iTrv];
    std::copy(vect->trVectorSection + j, vect->trVectorSection + vect->iTrv, newV);
    newNode->trVectorSection = newV;
    
    newNode->TrP1 = 1;
    newNode->TrP2 = 1;
    newNode->TrPinS[0] = end2Id;
    newNode->TrPinK[0] = 1;
    newNode->TrPinS[1] = vect->TrPinS[1];
    newNode->TrPinK[1] = vect->TrPinK[1];
    
    newV = new TRnode::TRSect[j];
    std::copy(vect->trVectorSection, vect->trVectorSection + j, newV);
    
    vect->iTrv = j;
    vect->TrPinS[1] = end1Id;
    vect->TrPinK[1] = 1;
    
    
    ////////////////////
    this->trackNodes[end1Id] = new TRnode();
    newNode = this->trackNodes[end1Id];
    newNode->typ = 0;
    newNode->UiD[0] = vect->trVectorSection[j-1].param[2];
    newNode->UiD[1] = vect->trVectorSection[j-1].param[3];
    newNode->UiD[2] = vect->trVectorSection[j-1].param[4];
    newNode->UiD[3] = vect->trVectorSection[j-1].param[6];
    newNode->UiD[4] = vect->trVectorSection[j].param[8];
    newNode->UiD[5] = vect->trVectorSection[j].param[9];
    newNode->UiD[6] = vect->trVectorSection[j].param[10];
    newNode->UiD[7] = vect->trVectorSection[j].param[11];
    newNode->UiD[8] = vect->trVectorSection[j].param[12];
    newNode->UiD[9] = vect->trVectorSection[j].param[13];
    newNode->UiD[10] = vect->trVectorSection[j].param[14];
    newNode->UiD[11] = vect->trVectorSection[j].param[15];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = id;
    newNode->TrPinK[0] = 0;
    /////////////////
    this->trackNodes[end2Id] = new TRnode();
    newNode = this->trackNodes[end2Id];
    newNode->typ = 0;
    newNode->UiD[0] = vect->trVectorSection[j].param[2];
    newNode->UiD[1] = vect->trVectorSection[j].param[3];
    newNode->UiD[2] = vect->trVectorSection[j].param[4];
    newNode->UiD[3] = vect->trVectorSection[j].param[5];
    newNode->UiD[4] = vect->trVectorSection[j].param[8];
    newNode->UiD[5] = vect->trVectorSection[j].param[9];
    newNode->UiD[6] = vect->trVectorSection[j].param[10];
    newNode->UiD[7] = vect->trVectorSection[j].param[11];
    newNode->UiD[8] = vect->trVectorSection[j].param[12];
    newNode->UiD[9] = vect->trVectorSection[j].param[13];
    newNode->UiD[10] = vect->trVectorSection[j].param[14] + M_PI;
    newNode->UiD[11] = vect->trVectorSection[j].param[15];

    newNode->TrP1 = 1;
    newNode->TrPinS[0] = vecId;
    newNode->TrPinK[0] = 1;
    
    delete vect->trVectorSection;
    vect->trVectorSection = newV;
    
    return vecId;
}

void TDB::deleteJunction(int id){
    TRnode* junction = trackNodes[id];
    if(junction->typ != 2) return;
    
    int count = 0;
    
    for(int i = 0; i < 3; i++){
        if(junction->TrPinS[i] != 0) count++;
    }
    if(count > 1){
        qDebug() << "junction delete fail";
        return;
    }
    if(count == 0){
        delete trackNodes[id];
        trackNodes[id] = NULL;
        return;
    }
    if(count == 1){
        int vecId = 0;
        for(int i = 0; i < 3; i++){
            if(junction->TrPinS[i] != 0){
                junction->TrPinS[0] = junction->TrPinS[i];
                junction->TrPinK[0] = junction->TrPinK[i];
                vecId = junction->TrPinS[i];
                break;
            }
        }
        TRnode* vect = trackNodes[vecId];
        vect->setTrPinK(id, 1);

        if(junction->TrPinK[0] == 1){
            junction->UiD[2] = vect->trVectorSection[0].param[4];
            junction->UiD[3] = vect->trVectorSection[0].param[5];
        } else if(junction->TrPinK[0] == 0){
            junction->UiD[2] = vect->trVectorSection[vect->iTrv-1].param[4];
            junction->UiD[3] = vect->trVectorSection[vect->iTrv-1].param[6];
        }
        junction->UiD[10] += M_PI;
        
        junction->typ = 0;
        junction->TrP1 = 1;
        junction->TrP2 = 0;
    }
}

void TDB::deleteVectorSection(int id){
    TRnode* vect = trackNodes[id];
    TRnode* end1 = trackNodes[vect->TrPinS[0]];
    TRnode* end2 = trackNodes[vect->TrPinS[1]];
    
    delete trackNodes[id];
    trackNodes[id] = NULL;
    
    if(end1->typ == 0){
        delete trackNodes[vect->TrPinS[0]];
        trackNodes[vect->TrPinS[0]] = NULL;
    } else if (end1->typ == 2) {
        end1->podmienTrPin(id, 0);
        end1->setTrPinK(0, 0);
    } 
    
    if(end2->typ == 0){
        delete trackNodes[vect->TrPinS[1]];
        trackNodes[vect->TrPinS[1]] = NULL;
    } else if (end2->typ == 2) {
        end2->podmienTrPin(id, 0);
        end2->setTrPinK(0, 0);
    }
}

bool TDB::deleteFromVectorSection(int id, int j){
    TRnode* vect = trackNodes[id];
    if(vect->iTrv == 1){
        deleteVectorSection(id);
        return false;
    }
    if(j > 0 && j < vect->iTrv - 1){
        vect = trackNodes[splitVectorSection(id, j)];
        j = 0;
    }
    
    TRnode* end1 = trackNodes[vect->TrPinS[0]];
    TRnode* end2 = trackNodes[vect->TrPinS[1]];
    
    TRnode::TRSect *newV = new TRnode::TRSect[vect->iTrv - 1];
    if(j == 0){
        std::copy(vect->trVectorSection + 1, vect->trVectorSection + vect->iTrv, newV);
        if(end1->typ == 2){
            end1->podmienTrPin(id, 0);
            end1->setTrPinK(0, 0);
            
            int endNId = ++this->iTRnodes;
            this->trackNodes[endNId] = new TRnode();
            end1 = trackNodes[endNId];
            end1->typ = 0;
            end1->TrP1 = 1;
            end1->TrPinS[0] = id;
            end1->TrPinK[0] = 1;
            vect->TrPinS[0] = endNId;
            vect->TrPinK[0] = 1;
        }
        
            end1->UiD[0] = vect->trVectorSection[1].param[2];
            end1->UiD[1] = vect->trVectorSection[1].param[3];
            end1->UiD[2] = vect->trVectorSection[1].param[4];
            end1->UiD[3] = vect->trVectorSection[1].param[5];
            end1->UiD[4] = vect->trVectorSection[1].param[8];
            end1->UiD[5] = vect->trVectorSection[1].param[9];
            end1->UiD[6] = vect->trVectorSection[1].param[10];
            end1->UiD[7] = vect->trVectorSection[1].param[11];
            end1->UiD[8] = vect->trVectorSection[1].param[12];
            end1->UiD[9] = vect->trVectorSection[1].param[13];
            end1->UiD[10] = vect->trVectorSection[1].param[14] + M_PI;
            end1->UiD[11] = vect->trVectorSection[1].param[15];

    } else if(j == vect->iTrv - 1) {
        std::copy(vect->trVectorSection , vect->trVectorSection + vect->iTrv - 1, newV);
        if(end2->typ == 2){
            end2->podmienTrPin(id, 0);
            end2->setTrPinK(0, 0);
            
            int endNId = ++this->iTRnodes;
            this->trackNodes[endNId] = new TRnode();
            end2 = trackNodes[endNId];
            end2->typ = 0;
            end2->TrP1 = 1;
            end2->TrPinS[0] = id;
            end2->TrPinK[0] = 0;
            vect->TrPinS[1] = endNId;
            vect->TrPinK[1] = 1;
        }
            end2->UiD[0] = vect->trVectorSection[vect->iTrv-2].param[2];
            end2->UiD[1] = vect->trVectorSection[vect->iTrv-2].param[3];
            end2->UiD[2] = vect->trVectorSection[vect->iTrv-2].param[4];
            end2->UiD[3] = vect->trVectorSection[vect->iTrv-2].param[6];
            end2->UiD[4] = vect->trVectorSection[vect->iTrv-1].param[8];
            end2->UiD[5] = vect->trVectorSection[vect->iTrv-1].param[9];
            end2->UiD[6] = vect->trVectorSection[vect->iTrv-1].param[10];
            end2->UiD[7] = vect->trVectorSection[vect->iTrv-1].param[11];
            end2->UiD[8] = vect->trVectorSection[vect->iTrv-1].param[12];
            end2->UiD[9] = vect->trVectorSection[vect->iTrv-1].param[13];
            end2->UiD[10] = vect->trVectorSection[vect->iTrv-1].param[14];
            end2->UiD[11] = vect->trVectorSection[vect->iTrv-1].param[15];

    }
    
    vect->iTrv -= 1;
    delete vect->trVectorSection;
    vect->trVectorSection = newV;
    
    return true;
}

int TDB::rotate(int id){
    TRnode* vect = trackNodes[id];
    TRnode* e1 = trackNodes[vect->TrPinS[0]];
    TRnode* e2 = trackNodes[vect->TrPinS[1]];
    
    for(int i = 0; i < vect->iTrv; i++){
        if(i < vect->iTrv - 1){
            vect->trVectorSection[i].param[8] = vect->trVectorSection[i+1].param[8];
            vect->trVectorSection[i].param[9] = vect->trVectorSection[i+1].param[9];
            vect->trVectorSection[i].param[10] = vect->trVectorSection[i+1].param[10];
            vect->trVectorSection[i].param[11] = vect->trVectorSection[i+1].param[11];
            vect->trVectorSection[i].param[12] = vect->trVectorSection[i+1].param[12];
            vect->trVectorSection[i].param[13] = -vect->trVectorSection[i].param[13]; //fix ????
            vect->trVectorSection[i].param[14] = vect->trVectorSection[i+1].param[14];
            vect->trVectorSection[i].param[15] = vect->trVectorSection[i+1].param[15];
        } else {
            vect->trVectorSection[i].param[8] = e2->UiD[4];
            vect->trVectorSection[i].param[9] = e2->UiD[5];
            vect->trVectorSection[i].param[10] = e2->UiD[6];
            vect->trVectorSection[i].param[11] = e2->UiD[7];
            vect->trVectorSection[i].param[12] = e2->UiD[8];
            vect->trVectorSection[i].param[13] = -vect->trVectorSection[i].param[13];
            vect->trVectorSection[i].param[14] = e2->UiD[10];
            vect->trVectorSection[i].param[15] = e2->UiD[11];
        }
        float angle = this->tsection->sekcja[vect->trVectorSection[i].param[0]]->getAngle();
        if (angle > 0) vect->trVectorSection[i].param[0]--;
        if (angle < 0) vect->trVectorSection[i].param[0]++;
        
        int tmp = vect->trVectorSection[i].param[5];
        vect->trVectorSection[i].param[5] = vect->trVectorSection[i].param[6];
        vect->trVectorSection[i].param[6] = tmp;
        
        vect->trVectorSection[i].param[14] += M_PI;
        if(vect->trVectorSection[i].param[14] > 2*M_PI)
            vect->trVectorSection[i].param[14] -= 2*M_PI;
    }
    for(int i = 0; i < vect->iTrv/2; i++){
        TRnode::TRSect t = vect->trVectorSection[i];
        vect->trVectorSection[i] = vect->trVectorSection[vect->iTrv - 1 - i];
        vect->trVectorSection[vect->iTrv - 1 - i] = t;
    }
    
    e1->setTrPinK(id, 0);
    e2->setTrPinK(id, 1);
    /*e1->UiD[10] += M_PI;
    if(e1->UiD[10] > 2*M_PI)
        e1->UiD[10] -= 2*M_PI;
    e2->UiD[10] += M_PI;
    if(e2->UiD[10] > 2*M_PI)
        e2->UiD[10] -= 2*M_PI;*/
    
    int tmp = vect->TrPinS[0];
    vect->TrPinS[0] = vect->TrPinS[1];
    vect->TrPinS[1] = tmp;
    tmp = vect->TrPinK[0];
    vect->TrPinK[0] = vect->TrPinK[1];
    vect->TrPinK[1] = tmp;

}

int TDB::appendToJunction(int junctionId, int eId, int idx){
    TRnode* junction = trackNodes[junctionId];

    if(idx == 1){
        if(junction->TrPinS[idx] != 0)
            idx++;
        if(junction->TrPinS[idx] != 0)
            return 0;
    } else {
        if(junction->TrPinS[idx] != 0)
            return 0;
    }
    
    TRnode* e1 = trackNodes[eId];
    int trackId = e1->TrPinS[0];
    TRnode* track = trackNodes[trackId];
    trackNodes[eId] = NULL;
    
    junction->TrPinS[idx] = trackId;
    
    
    int j = track->podmienTrPin(eId, junctionId);
    //track->TrPinS[0] = junctionId;
    if(idx == 0) {
        track->TrPinK[j] = 1;
        //junction->TrPinK[idx] = 0;
        junction->TrPinK[idx] = e1->TrPinK[0];
    } else {
        track->TrPinK[j] = 0;
        //junction->TrPinK[idx] = 1;
        junction->TrPinK[idx] = e1->TrPinK[0];
    }
}

void TDB::setDefaultEnd(int val){
    this->defaultEnd = val;
}

void TDB::nextDefaultEnd(){
    this->defaultEnd++;
}

bool TDB::findPosition(int x, int z, float* p, float* q, float* endp, int sectionIdx, int uid){
    float qe[3];
    qe[0] = 0;
    qe[1] = 0;
    qe[2] = 0;
    findNearestNode(x, z, p, (float*) &qe);
    
    bool b;
    
    if(sectionIdx < 0){
        Quat::rotateY(q, q, -qe[1]);
        return true;
    }
    
    TrackShape* shp = this->tsection->shape[sectionIdx];
    qDebug() << shp->filename;
    float startPos[3];
     
    while(defaultEnd >= shp->numpaths*2){
        defaultEnd -= shp->numpaths*2;
    }
    
    qDebug() << "defaultEnd" << defaultEnd;
    
    int startEnd = defaultEnd/2;
    int endend = defaultEnd - (startEnd)*2;
    
    Vector3f aa;
    Vector3f bb;
    Vector3f* aa2;
    
    if(endend == 1){
        float angle = 0;
        float dlugosc = 0;
        for (int i = 0; i < shp->path[startEnd].n; i++) {
            dlugosc = this->tsection->sekcja[shp->path[startEnd].sect[i]]->getDlugosc();
            //qDebug() << dlugosc;
            aa2 = this->tsection->sekcja[shp->path[startEnd].sect[i]]->getDrawPosition(dlugosc);
            aa2->rotateY(angle, 0);
            //qDebug() << "aa " << aa2->x << " "<<aa2->z;
            aa.x+=aa2->x;
            aa.z+=aa2->z;
            angle += this->tsection->sekcja[shp->path[startEnd].sect[i]]->getAngle();
        }
        endp[0] = -aa.x;
        endp[1] = 0;
        endp[2] = -aa.z;
        aa.rotateY(-qe[1], 0);
        startPos[0] = aa.x;
        startPos[2] = aa.z;
        qe[1] -= angle - M_PI;
        endp[4] = -qe[1] + shp->path[startEnd].rotDeg*M_PI/180;
    } else {
        endp[0] = aa.x;
        endp[1] = 0;
        endp[2] = aa.z;
        endp[4] = 0;
        startPos[0] = aa.x;
        startPos[2] = aa.z;
    }
    
    bb.x = shp->path[0].pos[0];
    bb.z = shp->path[0].pos[2];
    bb.rotateY(-qe[1], 0);
    aa.set(shp->path[0].pos[0] - shp->path[startEnd].pos[0], shp->path[0].pos[1], shp->path[0].pos[2] - shp->path[startEnd].pos[2]);
    aa.rotateY(-qe[1] + shp->path[startEnd].rotDeg*M_PI/180, 0);
    p[0] = p[0] + aa.x + startPos[0];
    p[1] = p[1] + shp->path[0].pos[1];
    p[2] = p[2] - aa.z - startPos[2];
    p[0] -= bb.x;
    p[2] += bb.z;
    
    Quat::rotateY(q, q, -qe[1] + shp->path[startEnd].rotDeg*M_PI/180);
    
    if(endend == 0)
        endp[3] = 1;
    if(endend == 1)
        endp[3] = -1;
    
    qDebug() << "ccc";
    qDebug() << startPos[0] << " " << startPos[2];
    
    return true;
}

bool TDB::placeTrack(int x, int z, float* p, float* q, int sectionIdx, int uid) {
    placeTrack(x, z, p, q, sectionIdx, uid, 0);
}

bool TDB::placeTrack(int x, int z, float* p, float* q, int sectionIdx, int uid, float elevation) {
    float qe[4];
    float vect[3];
    vect[0] = 0; vect[1] = 0; vect [2] = 10;
    Vec3::transformQuat(vect, vect, q);
    
    //float roll = atan2((2*(q[0]*q[1] + q[2]*q[3])),(1-((q[0]*q[0])+(q[1]*q[1]))));
    float pitch = asin(2*(q[0]*q[2] - q[1]*q[3]));
    //float yaw = atan2((2*(q[0]*q[3] + q[1]*q[2])),(1-((q[2]*q[2])+(q[3]*q[3]))));
    
    if(vect[2] < 0)
        pitch = M_PI - pitch;
    if(vect[2] == 0 && vect[0] < 0)
        pitch = -M_PI/2;
    if(vect[2] == 0 && vect[0] > 0)
        pitch = M_PI/2;

    qe[0] = tan((vect[1]/10.0));
    qe[1] = pitch;
    qe[2] = 0;
    
    TrackShape* shp = this->tsection->shape[sectionIdx];
    qDebug() << shp->filename;
    float pp[3];
    float qee[3];
    float startPos[3];
    float objPosOffset[3];
    int endp;
    // if (append > 0) {
        /*     endp = appendTrack(append, r->value, shp->path[0].sect[0], uid);
     } else {
     */
    //p[0] += shp->path[0].pos[0];
    //p[1] += shp->path[0].pos[1];
    //p[2] += shp->path[0].pos[2];

    int start;
    int ends[2];
    ////////////////////////////////
    int *endsNumbres = new int[shp->numpaths*2];
    std::unordered_map<int, int> endsIds;
    std::unordered_map<int, int> isJunction;
    std::unordered_map<int, int> junctionId;
    int nextNumber = 0;
    //int junctions[6];
    //int junctionCount = 0;
    
    for (int i = 0; i < shp->numpaths; i++) {
        int posIdx = shp->path[i].pos[0]*10000 + shp->path[i].pos[1]*100 + shp->path[i].pos[2];
        std::unordered_map<int, int>::iterator iter = endsIds.find(posIdx);
        if(iter == endsIds.end()){
            endsNumbres[i*2] = nextNumber++;
            endsIds[posIdx] = endsNumbres[i*2];
            isJunction[endsNumbres[i*2]] = 0;
        } else {
            endsNumbres[i*2] = endsIds[posIdx];
            isJunction[endsNumbres[i*2]] = 1;
            //qDebug() << "rozjazd";
            //junctions[junctionCount++] = newJunction(x, z, pp, qee, r->value, uid, ends[0]);
        }
        junctionId[endsNumbres[i*2]] = 0;
        endsNumbres[i*2+1] = nextNumber++;
        //qDebug() << "ends: "<< ends[0] <<" "<<ends[1];
    }
    //////
    
    //while(defaultEnd >= shp->numpaths*2){
    //    defaultEnd -= shp->numpaths*2;
    //}
    
    //qDebug() << "defaultEnd" << defaultEnd;
    
    int startEnd = 0;//defaultEnd/2;
    //int endend = defaultEnd - (startEnd)*2;
    
    Vector3f aa;
    Vector3f bb;
    Vector3f* aa2;
    
    /*if(endend == 1){
        float angle = 0;
        float dlugosc = 0;
        for (int i = 0; i < shp->path[startEnd].n; i++) {
            dlugosc = this->tsection->sekcja[shp->path[startEnd].sect[i]]->getDlugosc();
            //qDebug() << dlugosc;
            aa2 = this->tsection->sekcja[shp->path[startEnd].sect[i]]->getDrawPosition(dlugosc);
            aa2->rotateY(angle, 0);
            //qDebug() << "aa " << aa2->x << " "<<aa2->z;
            aa.x+=aa2->x;
            aa.z+=aa2->z;
            angle += this->tsection->sekcja[shp->path[startEnd].sect[i]]->getAngle();
        }
        aa.rotateY(-qe[1], 0);
        startPos[0] = aa.x;
        startPos[2] = aa.z;
        qe[1] -= angle - M_PI;
        
    } else {*/
        startPos[0] = aa.x;
        startPos[2] = aa.z;
    //}
    //int append = findNearestNode(x, z, p, (float*) &qe);

    for (int i = 0; i < shp->numpaths; i++) {
        aa.set(shp->path[i].pos[0], shp->path[i].pos[1], shp->path[i].pos[2]);
        //aa.rotateY(-qe[1] + shp->path[i].rotDeg*M_PI/180 - shp->path[startEnd].rotDeg*M_PI/180, 0);
        aa.rotateY(-qe[1], 0);

        pp[0] = p[0] + aa.x + startPos[0];
        pp[1] = p[1] + shp->path[i].pos[1];
        pp[2] = p[2] - aa.z - startPos[2];
        qee[0] = qe[0];
        qee[1] = qe[1] + shp->path[i].rotDeg*M_PI/180;
        qee[2] = qe[2];
        
        ends[0] = endsNumbres[i*2];
        ends[1] = endsNumbres[i*2+1];
        
        if(isJunction[ends[0]] == 1){
            isJunction[ends[0]] = 0;
            qDebug() << "rozjazd";
            junctionId[ends[0]] = newJunction(x, z, pp, qee, sectionIdx, uid, ends[0]);
        }

        endp = newTrack(x, z, pp, qee, (int*)ends, sectionIdx, shp->path[i].sect[0], uid, &start);
        
        for (int j = 1; j < shp->path[i].n; j++) {
            if (endp > 0) {
                endp = appendTrack(endp, (int*)ends, sectionIdx, shp->path[i].sect[j], uid);
            }
        }
        
        if(junctionId[ends[0]] != 0){
            qDebug() << "append to junction";
            appendToJunction(junctionId[ends[0]], start, 1);
            joinTracks(junctionId[ends[0]]);
        } else {
            joinTracks(start);
        }
        
         joinTracks(endp);
    }
    
    /*bb.x = shp->path[0].pos[0];
    bb.z = shp->path[0].pos[2];
    bb.rotateY(-qe[1], 0);
    aa.set(shp->path[0].pos[0] - shp->path[startEnd].pos[0], shp->path[0].pos[1], shp->path[0].pos[2] - shp->path[startEnd].pos[2]);
    aa.rotateY(-qe[1] + shp->path[startEnd].rotDeg*M_PI/180, 0);
    p[0] = p[0] + aa.x + startPos[0];
    p[1] = p[1] + shp->path[0].pos[1];
    p[2] = p[2] - aa.z - startPos[2];
    p[0] -= bb.x;
    p[2] += bb.z;
    
    q[0] = 0; q[1] = 0; q[2] = 0; q[3] = 1;
    //qe[1] += shp->path[startEnd].rotDeg*M_PI/180;
    //qe[2] = -qe[2];
    //float rot[3];
    //rot[0] = M_PI; rot[1] = qe[1]; rot[2] = qe[2];
    //Quat::fromRotationXYZ(q, rot);
    Quat::rotateY(q, q, -qe[1] + shp->path[startEnd].rotDeg*M_PI/180);
    Quat::rotateX(q, q, -qe[0]);*/
    
    refresh();
    return true;
}

void TDB::removeTrackFromTDB(int x, int y, int UiD){
    y = -y;
    qDebug() << "usune Track " << x << " " << y << " " << UiD; 
    
    TRnode *n;
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 1) {
                for(int j = 0; j < n->iTrv; j++)
                    if(n->trVectorSection[j].param[2] == x)
                        if(n->trVectorSection[j].param[3] == y)
                            if(n->trVectorSection[j].param[4] == UiD){
                                qDebug() << "jest";
                                if(deleteFromVectorSection(i, j))
                                    j = -1;
                    }
            }
        }
    for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
            if (n ->typ == -1) continue;
            if (n->typ == 2) {
                if(n->UiD[0] == x)
                    if(n->UiD[1] == y)
                        if(n->UiD[2] == UiD){
                            qDebug() << "jest j";
                            deleteJunction(i);
                }
            }
        }
    TDB::refresh();
}

void TDB::refresh() {
    isInitSectLines = false;
    isInitLines = false;
}

void TDB::renderAll(GLUU *gluu, float* playerT, float playerRot) {

    if (!loaded) return;
    int hash = playerT[0] * 10000 + playerT[1];
    if (!isInitLines || lineHash != hash) {
        TRnode *n;
        int lLen = 0, kLen = 0, pLen = 0;
        //qDebug() <<"update";
        lineHash = hash;
        isInitLines = true;
        
        for (auto it = endIdObj.begin(); it != endIdObj.end(); ++it) {
            TextObj* obj = (TextObj*) it->second;
            obj->inUse = false;
        }
        
        for (int i = 1; i <= iTRnodes; i++) {
            n = trackNodes[i];
            if (n == NULL) continue;
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
            n = trackNodes[i];
            if (n == NULL) continue;
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

                    linie[lPtr++] = ((trackNodes[n->TrPinS[1]]->UiD[4] - playerT[0])*2048 + trackNodes[n->TrPinS[1]]->UiD[6]);
                    linie[lPtr++] = (trackNodes[n->TrPinS[1]]->UiD[7] + wysokoscSieci);
                    linie[lPtr++] = (((-trackNodes[n->TrPinS[1]]->UiD[5] - playerT[1])*2048 - trackNodes[n->TrPinS[1]]->UiD[8]));
                }
            } else if (n->typ == 0) {
                if(fabs(n->UiD[4] - playerT[0]) > 2) continue;
                if(fabs(-n->UiD[5] - playerT[1]) > 2) continue;
                
                konce[kPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                konce[kPtr++] = (n->UiD[7]);
                konce[kPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);

                konce[kPtr++] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                konce[kPtr++] = (n->UiD[7] + wysokoscSieci);
                konce[kPtr++] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
                
                if(!road){
                    if(endIdObj[i] == NULL){
                        endIdObj[i] = new TextObj(i);
                    }    
                    endIdObj[i]->inUse = true;
                    endIdObj[i]->pos[0] = ((n->UiD[4] - playerT[0])*2048 + n->UiD[6]);
                    endIdObj[i]->pos[1] = n->UiD[7] + wysokoscSieci;
                    endIdObj[i]->pos[2] = ((-n->UiD[5] - playerT[1])*2048 - n->UiD[8]);
                }
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
    
    if(!road)
        for (auto it = endIdObj.begin(); it != endIdObj.end(); ++it) {
            //console.log(obj.type);
            TextObj* obj = (TextObj*) it->second;
            if(obj->inUse) obj->render(playerRot);
        }
}

void TDB::renderLines(GLUU *gluu, float* playerT, float playerRot) {

    if (!loaded) return;
    int hash = playerT[0] * 10000 + playerT[1];
    if (!sectionLines.loaded || sectionHash != hash || !isInitSectLines) {
        Vector3f p;
        Vector3f o;
        sectionHash = hash;
        isInitSectLines = true;

        int len = 0;

        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if (n == NULL) continue;
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
            TRnode* n = trackNodes[j];
            if (n == NULL) continue;
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
        if(road)
            sectionLines.setMaterial(0.0, 0.0, 1.0);
        else
            sectionLines.setMaterial(1.0, 1.0, 0.0);
        sectionLines.init(punkty, ptr - punkty, sectionLines.V, GL_LINES);

        delete punkty;
    }

    Mat4::identity(gluu->objStrMatrix);
    gluu->m_program->setUniformValue(gluu->msMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->objStrMatrix));
    gluu->m_program->setUniformValue(gluu->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    sectionLines.render();
}

bool TDB::getDrawPositionOnTrNode(float* out, int id, float metry){
    TRnode* n = trackNodes[id];
    if (n == NULL) return false;
    if (n->typ != 1) return false;
    
    float sectionLength = 0;
    float length = 0;
    int idx = 0;
    for (int i = 0; i < n->iTrv; i++) {
        idx = n->trVectorSection[i].param[0];
        
        try {
            sectionLength = tsection->sekcja.at(idx)->getDlugosc();
            
        } catch (const std::out_of_range& oor) {
            qDebug() << "nie ma sekcji " << idx;
        }
        
        length += sectionLength;
        if(length < metry)
            continue;
        
        Vector3f *position = tsection->sekcja.at(idx)->getDrawPosition(metry - length + sectionLength);
        
        float matrix[16];
        float q[4];
        q[0] = q[1] = q[2] = 0; q[3] = 1;
        float rot[3];
        rot[0] = M_PI;
        rot[1] = n->trVectorSection[i].param[14];
        rot[2] = n->trVectorSection[i].param[15];

        float pos[3];
        pos[0] = n->trVectorSection[i].param[10];
        pos[1] = n->trVectorSection[i].param[11];
        pos[2] = n->trVectorSection[i].param[12];
        
        Quat::fromRotationXYZ(q, rot);
        Mat4::fromRotationTranslation(matrix, q, pos);
        Mat4::rotate(matrix, matrix, -n->trVectorSection[i].param[13], 1, 0, 0);

        pos[0] = position->x;
        pos[1] = position->y;
        pos[2] = -position->z;
        Vec3::transformMat4(pos, pos, matrix);
        
        out[0] = pos[0];
        out[1] = pos[1];
        out[2] = pos[2];
        out[3] = -n->trVectorSection[i].param[14] - tsection->sekcja.at(idx)->getDrawAngle(metry - length + sectionLength);
        out[4] = n->trVectorSection[i].param[8];
        out[5] = n->trVectorSection[i].param[9];
        
        //position->x += (n->trVectorSection[i].param[8] - playerT[0])*2048 + n->trVectorSection[i].param[10];
        //position->y += n->trVectorSection[i].param[11];
        //position->z += (-n->trVectorSection[i].param[9] - playerT[1])*2048 - n->trVectorSection[i].param[12];
        return true;
    }
    return false;
}

int TDB::findTrItemNodeId(int id){
    for (int j = 1; j <= iTRnodes; j++) {
        TRnode* n = trackNodes[j];
        if (n == NULL) continue;
        if (n->typ == 1) {
            for (int i = 0; i < n->iTri; i++) {
                if(n->trItemRef[i] == id)
                    return j;
            }
        }
    }
    return -1;
}

void TDB::renderItems(GLUU *gluu, float* playerT, float playerRot) {
    for (auto it = this->trackItems.begin(); it != this->trackItems.end(); ++it) {
        //console.log(obj.type);
        TRitem* obj = (TRitem*) it->second;

        obj->render(this, gluu, playerT, playerRot);
    }
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

float TDB::setTerrainToTrackObj(int x, int y, int uid, float * &ptr){
        //Vector3f p;
        Vector3f o;
        //qDebug() << x << " "<< y << " "<< uid;
        //int len = 0;
        float matrix[16];
        float q[4];
        float p[3];
        q[0] = 0; q[1] = 0; q[2] = 0; q[3] = 1;
        float rot[3];
        y = -y;
        
        //float* punkty = new float[10000];
        //float* ptr = punkty;
        
        for (int j = 1; j <= iTRnodes; j++) {
            TRnode* n = trackNodes[j];
            if (n == NULL) continue;
            if (n->typ == -1) continue;
            if (n->typ == 1) {
                for (int i = 0; i < n->iTrv; i++) {
                    if(n->trVectorSection[i].param[2] == x && n->trVectorSection[i].param[3] == y && n->trVectorSection[i].param[4] == uid ){
                        qDebug() << "mam";
                    
                        //p.set(
                        //    (n->trVectorSection[i].param[8] - x)*2048 + n->trVectorSection[i].param[10],
                        //    n->trVectorSection[i].param[11],
                        //    (-n->trVectorSection[i].param[9] - y)*2048 - n->trVectorSection[i].param[12]
                        //    );
                        //o.set(
                        //rot = n->trVectorSection[i].param[13];
                        //    n->trVectorSection[i].param[14],
                        //    n->trVectorSection[i].param[15]
                        //    );
                    
                        //q[0] = q[1] = q[2] = 0;  q[3] = 1;
                        //rot[0] = M_PI; rot[1] = -o.y; rot[2] = o.z;
                        
                        //Quat::fromRotationXYZ(q, rot);
                        
                        //Mat4::fromRotationTranslation(matrix, q, reinterpret_cast<float *> (&p));
                        //Mat4::rotate(matrix, matrix, o.x, 1, 0, 0);
                        //Mat4::identity(matrix);
                        //Mat4:: rotate(matrix, matrix, o.x, 1, 0, 0);
                        //Mat4::multiply(matrix, matrix, objMatrix);
                        //matrix = Mat4::clone(objMatrix);
                        
                        //float matrix[16];
                        //float q[4];
                        //q[0] = q[1] = q[2] = 0;
                        //q[3] = 1;
                        //float rot[3];
                        rot[0] = M_PI;
                        rot[1] = -n->trVectorSection[i].param[14];
                        rot[2] = n->trVectorSection[i].param[15];
                        p[0] = n->trVectorSection[i].param[10];
                        p[1] = n->trVectorSection[i].param[11];
                        p[2] = -n->trVectorSection[i].param[12];
                        
                        Quat::fromRotationXYZ(q, rot);
                        Mat4::fromRotationTranslation(matrix, q, p);
                        Mat4::rotate(matrix, matrix, n->trVectorSection[i].param[13], 1, 0, 0);
                        
                        //Mat4::fromRotationTranslation(matrix, q, objMatrix);
                        try {
                            tsection->sekcja.at((int) n->trVectorSection[i].param[0])->getPoints(ptr, matrix);
                        } catch (const std::out_of_range& oor) {
                            qDebug() << "nie ma sekcji " << (int) n->trVectorSection[i].param[0];
                        }
                    }
                }
            }
            
    }
    return 0;
}

 bool TDB::deleteNulls() {
        for(int i = 1; i <= iTRnodes; i++){
            if(trackNodes[i] == NULL){
                qDebug() << "Usuwam NULL na "<<i;
                int stare = findBiggest();
                if(stare <= i) {
                    qDebug() << "Juz nie ma nulli ";
                    iTRnodes = stare;
                    return false;
                }
                trackNodes[i] = trackNodes[stare];
                trackNodes[stare] = NULL;
                qDebug() << "zastopiony przez " << stare;
                
                for(int j = 0; j < 3; j++){
                    if(trackNodes[i]->TrPinS[j] == 0) 
                        continue;
                    trackNodes[trackNodes[i]->TrPinS[j]]->podmienTrPin(stare, i);
                }
                return true;
            }
        }
        qDebug() << "Juz nie ma nulli ";
        return false;
    }

int TDB::findBiggest() {
        for(int i = iTRnodes; i > 0; i--){
            if(trackNodes[i] != NULL)
                return i;
        }
        return 1;
    }

void TDB::saveEmpty() {    
    QString sh;
    QString path;
    path = Game::root + "/routes/" + Game::route + "/" + Game::route + ".tdb";
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out.setRealNumberPrecision(8);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "SIMISA@@@@@@@@@@JINX0T0t______\n\n";
    out << "TrackDB (\n";
    out << "	Serial ( 0 )\n";
    out << "	TrackNodes ( 0 \n";
    out << "	)\n";
    out << ")";
    file.close();
}
    
void TDB::save() {
    if(!Game::writeEnabled) return;
    if(!Game::writeTDB) return;
    
    while(deleteNulls());
    this->isInitLines = false;
    
    QString sh;
    QString path;
    QString extension = "tdb";
    if(this->road) extension = "rdb";
    path = Game::root + "/routes/" + Game::route + "/" + Game::route + "222." + extension;
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
    out << "	Serial ( " << this->serial << " )\n";
    out << "	TrackNodes ( " << (this->iTRnodes) << "\n";

    for (int i = 1; i <= this->iTRnodes; i++) {
        out << "		TrackNode ( " << i << "\n";
        if (trackNodes[i] == NULL) continue;
        switch (trackNodes[i]->typ) {
            case 0:
                out << "			TrEndNode ( " << trackNodes[i]->args[0] << " )\n";
                out << "			UiD ( ";
                for (int j = 0; j < 12; j++) {
                    out << trackNodes[i]->UiD[j] << " ";
                }
                out << ")\n";
                out << "			TrPins ( 1 0\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[0] << " " << trackNodes[i]->TrPinK[0] << " )\n";
                out << "			)\n";
                break;
            case 1:
                out << "			TrVectorNode ( \n";
                out << "				TrVectorSections ( " << trackNodes[i]->iTrv << "";
                for (int j = 0; j < trackNodes[i]->iTrv; j++) {
                    for (int jj = 0; jj < 7; jj++) {
                        out << " " << trackNodes[i]->trVectorSection[j].param[jj];
                    }
                    out << " 00";
                    for (int jj = 8; jj < 16; jj++) {
                        out << " " << trackNodes[i]->trVectorSection[j].param[jj];
                    }
                    if (j % 11 == 0 && j > 0 && j < trackNodes[i]->iTrv - 1)
                        out << "\n					";
                }
                out << " )\n";
                if(trackNodes[i]->trItemRef != 0){
                    out << "				TrItemRefs ( "<<trackNodes[i]->iTri<<"\n";
                    for(int j = 0; j<trackNodes[i]->iTri; j++){
                        out << "					TrItemRef ( "<<trackNodes[i]->trItemRef[j]<<" )\n";
                    }
                    out << "				)\n";
                }
                out << "			)\n";
                out << "			TrPins ( 1 1\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[0] << " " << trackNodes[i]->TrPinK[0] << " )\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[1] << " " << trackNodes[i]->TrPinK[1] << " )\n";
                out << "			)\n";
                break;
            case 2:
                out << "			TrJunctionNode ( " << trackNodes[i]->args[0] << " " << trackNodes[i]->args[1] << " " << trackNodes[i]->args[2] << " )\n";
                out << "			UiD ( ";
                for (int j = 0; j < 12; j++) {
                    out << trackNodes[i]->UiD[j] << " ";
                }
                out << ")\n";
                out << "			TrPins ( 1 2\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[0] << " " << trackNodes[i]->TrPinK[0] << " )\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[1] << " " << trackNodes[i]->TrPinK[1] << " )\n";
                out << "				TrPin ( " << trackNodes[i]->TrPinS[2] << " " << trackNodes[i]->TrPinK[2] << " )\n";
                out << "			)\n";
                break;
        }

        out << "		)\n";
    }
    out << "	)\n";
    
    if(this->iTRitems > 0){
        out << "	TrItemTable ( " << (this->iTRitems) << "\n";
        for (int i = 0; i <= this->iTRitems; i++) {
            if(this->trackItems[i] != NULL)
                this->trackItems[i]->save(&out);
        }
        out << "	)\n";
    }
    
    
    out << ")";
    file.close();

    qDebug() << "Zapisane";
}

TDB::TDB(const TDB& orig) {
}

TDB::~TDB() {
}

