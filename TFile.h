#include "FileBuffer.h"
#include <QString>
#include <unordered_map>

#ifndef TFILE_H
#define	TFILE_H

class TFile {
public:
    struct mat {
        QString* tex[2];
        QString* name = NULL;
        int atex[2][2];
        int itex[2][4];
        int count153 = 0;
        int count155 = 0;
    };
    bool loaded = false;
    bool used = false;
    float* tdata;
    float* erroeBias;
    //mat* materials;
    std::unordered_map<int, mat> materials;
    int* flags;
    float WSW;
    float WSE;
    float WNE;
    float WNW;
    float* errthresholdScale = NULL;
    float* alwaysselectMaxdist = NULL;
    int* nsamples = NULL;
    float* sampleRotation = NULL;
    float* sampleSize = NULL;
    float floor;
    float scale;
    
    bool waterLevel = false;
    int materialsCount = 0;
    
    QString* sampleFbuffer = NULL;
    QString* sampleYbuffer = NULL;
    QString* sampleEbuffer = NULL;
    QString* sampleNbuffer = NULL;
    char* sampleASbuffer = NULL;
    
    int patchsetDistance;
    int patchsetNpatches;
    
    TFile();
    TFile(const TFile& orig);
    virtual ~TFile();
    bool readT(QString fSfile);
    void save(QString name);
    int cloneMat(int id);
    int getMatByTexture(QString tname);
    
private:
    void get139(FileBuffer* data, int length);
    void get151(FileBuffer* data);
    void get153(FileBuffer* data, TFile::mat* m);
    void get156(FileBuffer* data, TFile::mat* m);
    void get157(FileBuffer* data);
    void get163(FileBuffer* data, int n);
    void get251(FileBuffer* data);
};

#endif	/* TFILE_H */

