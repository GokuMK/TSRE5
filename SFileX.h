#ifndef SFILEX_H
#define	SFILEX_H

#include "FileBuffer.h"
class SFile;

class SFileX {
public:
    
    struct fvertex {
        unsigned short arg1;
        unsigned short point;
        unsigned short normal;
        unsigned short arg2;
        unsigned short arg3;
        unsigned short material;
        unsigned short uvpoint;
    };
    
    SFileX();
    SFileX(const SFileX& orig);
    virtual ~SFileX();
    
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

