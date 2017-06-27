/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    static int FindTokenDomIgnore(QString sh, FileBuffer* bufor);
    static QString NextTokenDomIgnore(FileBuffer* bufor);
    static QString NextTokenInside(FileBuffer* bufor);
    static QString GetAlternativeTokenName(FileBuffer* bufor);
    static QString GetString(FileBuffer* bufor);
    static QString GetStringInside(FileBuffer* bufor);
    static float GetNumber(FileBuffer* bufor);
    static float GetNumberInside(FileBuffer* bufor, bool *ok = NULL);
    static unsigned int GetUInt(FileBuffer* bufor);
    static unsigned int GetHex(FileBuffer* bufor);
    static int SkipToken(FileBuffer* bufor);
    static QString AddComIfReq(QString n);
    static QString MakeFlagsString(unsigned int val);
    static QString SplitToMultiline(QString n, QString woff = "");

private:
    static float NumberUnit(float x, unsigned short int &b, FileBuffer* bufor);
};

#endif	/* PARSERX_H */

