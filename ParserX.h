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
    static float parsujr(FileBuffer* bufor);
    static int parsuj16(FileBuffer* bufor);
    static int pominsekcje(FileBuffer* bufor);
    static int sekcjap(FileBuffer* bufor);

private:

};

#endif	/* PARSERX_H */

