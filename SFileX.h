/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SFILEX_H
#define	SFILEX_H

#include "FileBuffer.h"
class SFile;

class SFileX {
public:
    
    struct fvertex {
        unsigned short arg1;
        unsigned int point;
        unsigned int normal;
        unsigned short arg2;
        unsigned short arg3;
        unsigned short material;
        unsigned int uvpoint;
    };
    
    SFileX();
    SFileX(const SFileX& orig);
    virtual ~SFileX();
    static void odczytajshaders(FileBuffer* bufor, SFile* pliks);
    static void odczytajpunkty(FileBuffer* bufor, SFile* pliks);
    static void odczytajuvpunkty(FileBuffer* bufor, SFile* pliks);
    static void odczytajnormalne(FileBuffer* bufor, SFile* pliks);
    static void odczytajmatrices(FileBuffer* bufor, SFile* pliks);
    static void odczytajimages(FileBuffer* bufor, SFile* pliks);
    static void odczytajtextures(FileBuffer* bufor, SFile* pliks);
    static void odczytajvtx_states(FileBuffer* bufor, SFile* pliks);
    static void odczytajprim_states(FileBuffer* bufor, SFile* pliks);
    static void odczytajlodd(FileBuffer* bufor, SFile* pliks);    
private:

};

#endif	/* SFILEX_H */

