#ifndef PARSERX_H
#define	PARSERX_H

#include "FileBuffer.h"
#include <QString>
class ParserX {
public:
    ParserX();
    ParserX(const ParserX& orig);
    virtual ~ParserX();
    
    static int nextLine(FileBuffer* bufor);
    static int szukajsekcji1(QString sh, FileBuffer* bufor);
    static QString nazwasekcji(FileBuffer* bufor);
    static QString nazwasekcji_inside(FileBuffer* bufor);
    static int szukajsekcji2(QString sh1, QString sh2, FileBuffer* bufor);
    static int szukajsekcjiN(QString sh, FileBuffer* bufor);
    static int pominsekcjec(FileBuffer* bufor);
    static QString odczytajtc(FileBuffer* bufor);
    static QString odczytajtcInside(FileBuffer* bufor);
    static float parsujr(FileBuffer* bufor);
    static float parsujrInside(FileBuffer* bufor, bool *ok = NULL);
    static float numberUnit(float x, char b, FileBuffer* bufor);
    static unsigned int parsujUint(FileBuffer* bufor);
    static unsigned int parsuj16(FileBuffer* bufor);
    static int pominsekcje(FileBuffer* bufor);
    static int sekcjap(FileBuffer* bufor);
    static QString addComIfReq(QString n);

private:

};

#endif	/* PARSERX_H */

