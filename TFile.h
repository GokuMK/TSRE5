#include "FileBuffer.h"
#include <QString>

#ifndef TFILE_H
#define	TFILE_H

class TFile {
public:
    struct mat {
        QString tex[2];
        int atex[2];
        int itex[2][4];
    };
    bool loaded = false;
    bool used = false;
    float floor;
    float scale;
    float* tdata;
    mat* materials;
    int* flags;
    float WSW;
    float WSE;
    float WNE;
    float WNW;
    TFile();
    TFile(const TFile& orig);
    virtual ~TFile();
    bool readT(QString fSfile);
    void get139(FileBuffer* data);
    void get151(FileBuffer* data);
    void get153(FileBuffer* data, TFile::mat* m);
    void get156(FileBuffer* data, TFile::mat* m);
    void get157(FileBuffer* data);
    void get163(FileBuffer* data, int n);
    void get251(FileBuffer* data);
private:

};

#endif	/* TFILE_H */

