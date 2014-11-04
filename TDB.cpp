#include "TDB.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"

TDB::TDB(QString path) {
    loaded = false;
    qDebug() << "Wczytywanie pliku tdb: " << path;

    tsection = new TSectionDAT();

    int x, i, j, ii, jj, iloscts, uu;
    float xx;
    int t;
    QString sh;
    path.replace("//","/");
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

    iloscts = (int) ParserX::parsujr(bufor); //odczytanie ilosci sciezek
    trackNodes = new TRnode[iloscts + 1]; //przydzielenie pamieci
    qDebug() << "N" << iloscts;
    //zapisanie x tracknodes
    for (i = 0; i < iloscts; i++) {
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

void TDB::trpin(TRnode *tr, FileBuffer* bufor ){ 
          QString sh;

          sh = "TrPins";
          ParserX::szukajsekcji1(sh,bufor);              
          
          tr->TrP1 =(int)ParserX::parsujr(bufor);
          tr->TrP2 =(int)ParserX::parsujr(bufor);

          for(int i=0; i<(tr->TrP1+tr->TrP2); i++){
             tr->TrPinS[i] = (int)ParserX::parsujr(bufor);
             tr->TrPinK[i] = (int)ParserX::parsujr(bufor);
          }
    }

TDB::TDB(const TDB& orig) {
}

TDB::~TDB() {
}

