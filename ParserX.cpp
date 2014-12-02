#include "ParserX.h"
#include "SFile.h"
#include <QDebug>
//-----------------------------------
//Next line
//-----------------------------------
int ParserX::nextLine(FileBuffer* bufor){
    char b;
    while (bufor->length >= bufor->off + 2) {
        b = bufor->get();
        //qDebug() << b;
        bufor->off++;
        if(b == '\n'){
            return 1;
        }
    }
    return 0;
}
//-----------------------------------
//Szukanie sekcji
//-----------------------------------
int ParserX::szukajsekcji1(QString sh, FileBuffer* bufor){
    char b;
    int i = 0, czytam = 0;
    int poziom = 0;
    QString sekcja = "";

    while (bufor->length >= bufor->off + 2) {
        b = bufor->get();
        //qDebug() << b;
        bufor->off++;
        if (b == 40) {
            poziom++;
        }
        if (b == 41) {
            poziom--;
        }
        if (poziom > 0) continue;
        if ((b > 64) || (b>47 && b<58 && czytam == 1)) {
            czytam = 1;
            sekcja += b;
            i++;
        } else {
            if (czytam == 1) {
                if (b == 40) bufor->off -= 2;
                i = 0;
                //qDebug() << "mam " << sekcja << " " << sh;
                if (sekcja.toLower() == sh.toLower()) {
                    //if (b == '(') ibufor--;
                    for (;;) {
                        //if(bufor->length <= bufor->off + 2)
                        //    return 0;
                        b = bufor->get();
                        bufor->off++;
                        if (b == 40) {
                            return 1;
                        }
                    }
                }
                sekcja = "";
                czytam = 0;
            }
        }
    }
    //console.log("fail");
    return 0;
}
//-----------------------------------
//Szukanie sekcji
//-----------------------------------
QString ParserX::nazwasekcji(FileBuffer* bufor){
    char b;
    int i = 0, czytam = 0;
    int poziom = 0;
    QString sekcja = "";

    while (bufor->length >= bufor->off + 2) {
        b = bufor->get();
        bufor->off++;
        if (b == 40 && czytam == 0) {
            poziom++;
        }
        if (b == 41) {
            poziom--;
        }
        if (poziom > 0) continue;

        if ((b > 64) || (b>47 && b<58 && czytam == 1)) {
            czytam = 1;
            sekcja += b;
            i++;
        } else {
            if (czytam == 1) {
                if (b == 40) bufor->off -= 2;
                i = 0;

                for (;;) {
                    //if(bufor->length <= bufor->off + 2)
                    //    return "";
                    b = bufor->get();
                    bufor->off++;
                    if (b == 40) {
                        return sekcja;
                    }
                }
            }
        }
    }
    return "";
}
//-----------------------------------
//Szukanie sekcji
//-----------------------------------
QString ParserX::nazwasekcji_inside(FileBuffer* bufor){
    char b;
    int i = 0, czytam = 0;
    int poziom = 0;
    //bool koniec = false;
    QString sekcja = "";

    while (bufor->length >= bufor->off + 2) {
        b = bufor->get();
        bufor->off++;
        if (b == 40) {
            poziom++;
        }
        if (b == 41) {
            poziom--;
        }
        if (poziom > 0) continue;
        if (poziom < 0) {
            bufor->off -= 2;
            return "";
        }
        //console.log("p "+String.fromCharCode(b)+" "+poziom);
        if ((b > 64) || (b>47 && b<58 && czytam == 1)) {
            czytam = 1;
            sekcja += b;
            i++;
        } else {
            if (czytam == 1) {
                if (b == 40) bufor->off -= 2;
                i = 0;

                for (;;) {
                    //if(bufor->length <= bufor->off + 2)
                    //    return "";
                    b = bufor->get();
                    bufor->off++;
                    if (b == 40) {
                        //qDebug() << sekcja;
                        return sekcja;
                    }
                }
            }
        }
    }
    //console.log("sekcja");
    bufor->off -= 2;
    return "";
}
//-----------------------------------
//Szukanie 2 sekcji
//-----------------------------------
int ParserX::szukajsekcji2(QString sh1, QString sh2, FileBuffer* bufor){
    char b;
    int i = 0, czytam = 0;
    int poziom = 0;
    QString sekcja = "";

    while (bufor->length >= bufor->off + 2) {
        b = bufor->get();
        bufor->off++;
        if (b == 40 && czytam == 0) {
            poziom++;
        }
        if (b == 41 && czytam == 0) {
            poziom--;
        }
        if (poziom > 0) continue;
        //if (poziom < 0) return -1;
        if ((b > 64) || (b>47 && b<58 && czytam == 1)) {
            czytam = 1;
            sekcja += b;
            i++;
        } else {
            if (czytam == 1) {
                if (b == 40) bufor->off -= 2;
                i = 0;
                if (sekcja.toLower() == sh1.toLower()) {
                    for (;;) {
                        b = bufor->get();
                        bufor->off++;
                        if (b == 40) {
                            return 1;
                        }
                    }
                }
                if (sekcja.toLower() == sh2.toLower()) {
                    for (;;) {
                        b = bufor->get();
                        bufor->off++;
                        if (b == 40) {
                            return 2;
                        }
                    }

                }
                sekcja = "";
                czytam = 0;
            }
        }
    }
    return -1;
}
//-----------------------------------
//Szukanie n sekcji
//-----------------------------------
int ParserX::szukajsekcjiN(QString sh, FileBuffer* bufor){
    /*var b;
    var i = 0, czytam = 0;
    var poziom = 0;
    var sekcja = "";

    while (bufor.d.length > bufor.p + 2) {
        b = bufor.d[bufor.p++];
        bufor.p++;
        if (b == = 40) {
            poziom++;
        }
        if (b == = 41) {
            poziom--;
        }
        if (poziom > 0) continue;
        if (poziom < -1) return -1;
        if (b > 64) {
            czytam = 1;
            sekcja += String.fromCharCode(b);
            i++;
        } else {
            if (czytam == = 1) {
                if (b == = 40) bufor.p -= 2;
                i = 0;
                for (var ii = 0; ii < sh.length; ii++) {
                    if (sekcja.toUpperCase() == = sh[ii].toUpperCase()) {
                        //if (b == '(') ibufor--;
                        for (;;) {
                            b = bufor.d[bufor.p++];
                            bufor.p++;
                            if (b == = 40) {
                                return ii;
                            }
                        }
                    }
                }
                sekcja = "";
                czytam = 0;
            }
        }
    }
    return -1;*/
}
//-----------------------------------
//Pominiecie sekcji
//-----------------------------------
int ParserX::pominsekcjec(FileBuffer* bufor){
    char b;
    int poziom = 0;
    while (bufor->length >= bufor->off + 2) {
        b = bufor->get();
        bufor->off++;
        if (b == 40) {
            poziom++;
        }
        if (b == 41) {
            poziom--;
        }
        if (poziom > 0) continue;
        if (poziom < 0) {
            bufor->off -= 2;
            return -1;
        }
    }
    return 0;
}
//-----------------------------------
//Parsowanie stringa
//-----------------------------------
QString ParserX::odczytajtc(FileBuffer* bufor){
    QString sciezka = "";
    char b = 0;
    while ((b < 46) && (b != 34)) {
        b = bufor->get();
        bufor->off++;
    }
    //bufor.position(bufor.position()-2); 
    if (b == 34) {
        while ((b = bufor->get()) != 34) {
            //bufor->off++;
            sciezka += QChar(b, bufor->get());
        }
    } else {
        bufor->off -= 2;
        while (((b = bufor->get()) != 32) && (b != 10)) {
            //bufor->off++;
            sciezka += QChar(b, bufor->get());
        }
    }

    bufor->off++;
    //qDebug() << sciezka;
    return sciezka;
}
//-----------------------------------
//Parsowanie liczby rzeczywistej
//-----------------------------------
float ParserX::parsujr(FileBuffer* bufor){
    char b = 0;
    int j;
    float x, t;
    int liczba = 1, ujemna = 0;

    while (b < 45 || (b > 45 && b < 48) || b > 57) {
        b = bufor->get();
        bufor->off++;
    }
    x = 0;
    liczba = 1;
    ujemna = 0;
    if (b == 45) {
        ujemna = 1;
        b = bufor->get();
        bufor->off++;
    }
    while (b > 47 && b < 58) {
        x = x * 10 + b - 48;
        b = bufor->get();
        bufor->off++;
    }
    if (b == 46 || b == 44) {
        b = bufor->get();
        bufor->off++;
        while (b > 47 && b < 58) {
            liczba = liczba * 10;
            t = b;
            x = x + (t - 48) / liczba;
            b = bufor->get();
            bufor->off++;
        }
    }
    if (ujemna == 1) x = -x;
    if (b == 69 || b == 101) {
        b = bufor->get();
        bufor->off++;
        if (b == 45) {
            ujemna = 1;
            b = bufor->get();
            bufor->off++;
        } else ujemna = 0;
        liczba = 0;
        while (b > 47 && b < 58) {
            liczba = liczba * 10 + b - 48;
            b = bufor->get();
            bufor->off++;
        }
        if (ujemna == 1) {
            for (j = 0; j < liczba; j++) {
                x = x / 10;
            }
        } else {
            for (j = 0; j < liczba; j++) {
                x = x * 10;
            }
        }
    }
    bufor->off -= 2;
    return x;
}
//-----------------------------------
//Parsowanie liczby uint
//-----------------------------------
unsigned int ParserX::parsujUint(FileBuffer* bufor){
    char b = 0;
    unsigned int x;

    while (b < 45 || (b > 45 && b < 48) || b > 57) {
        b = bufor->get();
        bufor->off++;
    }
    x = 0;
    while (b > 47 && b < 58) {
        x = x * 10 + b - 48;
        b = bufor->get();
        bufor->off++;
    }
    bufor->off -= 2;
    return x;
}
//-----------------------------------
//Parsowanie liczby szesnastkowej
//-----------------------------------
int ParserX::parsuj16(FileBuffer* bufor){
    char b = 0, sb = 0;
    int x;

    while (((b < 48) || (b > 57 && b < 65) || (b > 70 && b < 97) || (b > 102)) || sb != 32) {
        sb = b;
        b = bufor->get();
        bufor->off++;
    }
    x = 0;
    while ((b > 47 && b < 58) || (b > 64 && b < 71) || (b > 96 && b < 103)) {
        if (b > 96) b = (b - 87 + 48);
        else {
            if (b > 64) b = (b - 55 + 48);
        }
        x = x * 16 + b - 48;
        b = bufor->get();
        bufor->off++;
    }
    bufor->off -= 2;
    return x;
}
//-----------------------------------
//Pominiecie sekcji
//-----------------------------------
int ParserX::pominsekcje(FileBuffer* bufor){
    char b;
    int poziom = 0;

    for (int tt = 0; tt < 20000; tt++){
    //for (;;){
        if(bufor->length <= bufor->off + 2)
            return 0;
        b = bufor->get();
        bufor->off++;
        //System.out.print(b);
        //if (b === 0) { return -3; }
        if (b == 40) {
            poziom++;
        }
        if (b == 41) {
            poziom--;
        }
        //console.log(",, "+String.fromCharCode(b)+" "+poziom);
        if (poziom < 0) return 0;
    }
}
//-----------------------------------
//Wybranie sekcji primitives
//-----------------------------------
int ParserX::sekcjap(FileBuffer* bufor){
    char b;
    int i = 0, czytam = 0;
    int w;
    QString psi = "prim_state_idx";
    int poziom = 0;
    QString sekcja = "";

    while (bufor->length >= bufor->off + 2) {
        b = bufor->get();
        bufor->off++;
        if (b == 40) {
            poziom++;
        }
        if (b == 41) {
            poziom--;
        }
        if (poziom > 0) continue;
        if (b > 65) {
            czytam = 1;
            sekcja += b;
            i++;
        } else {
            if (czytam == 1) {
                i = 0;
                //(sekcja + " bylo");
                if (sekcja.toLower() == psi.toLower()) {
                    w = ParserX::parsujr(bufor);
                    return w;
                } else {
                    for (;;) {
                        b = bufor->get();
                        bufor->off++;
                        if (b == 40) {
                            return -1;
                        }
                    }
                }
            }
        }
    }
    return -3;
}
