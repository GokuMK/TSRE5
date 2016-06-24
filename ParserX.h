#ifndef PARSERX_H
#define	PARSERX_H

#include "FileBuffer.h"
#include <QString>

class ParserX {
public:
    ParserX();
    ParserX(const ParserX& orig);
    virtual ~ParserX();
    
    static int NextLine(FileBuffer* bufor);
    static int szukajsekcji1(QString sh, FileBuffer* bufor);
    static QString nazwasekcji(FileBuffer* bufor);
    static QString NextTokenInside(FileBuffer* bufor);
    static int szukajsekcji2(QString sh1, QString sh2, FileBuffer* bufor);
    static int szukajsekcjiN(QString sh, FileBuffer* bufor);
    static int pominsekcjec(FileBuffer* bufor);
    static QString GetString(FileBuffer* bufor);
    static QString GetStringInside(FileBuffer* bufor);
    static float GetNumber(FileBuffer* bufor);
    static float GetNumberInside(FileBuffer* bufor, bool *ok = NULL);
    static unsigned int GetUInt(FileBuffer* bufor);
    static unsigned int GetHex(FileBuffer* bufor);
    static int SkipToken(FileBuffer* bufor);
    static int sekcjap(FileBuffer* bufor);
    static QString AddComIfReq(QString n);
    static QString MakeFlagsString(unsigned int val);
    static QString SplitToMultiline(QString n, QString woff = "");

private:
    static float NumberUnit(float x, unsigned char &b, FileBuffer* bufor);
};

#endif	/* PARSERX_H */

