/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "FileBuffer.h"
#include <QString>
#include <unordered_map>

#ifndef TFILE_H
#define	TFILE_H

class TFile {
public:
    struct Mat {
        QString* tex[2];
        QString* name = NULL;
        int atex[2][2];
        int itex[2][4];
        int count153 = 0;
        int count155 = 0;
        
        Mat(){
            tex[0] = NULL;
            tex[1] = NULL;
        }
    };
    bool loaded = false;
    bool used = false;
    float* tdata;
    float* erroeBias;
    //mat* materials;
    std::unordered_map<int, Mat> materials;
    std::unordered_map<int, Mat> amaterials;
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
    int newMat();
    int cloneMat(int id);
    int getMatByTexture(QString tname);
    void removeMat(int id);
    void setBufferNames(QString name);
    
private:
    void get139(FileBuffer* data, int length);
    void get151(FileBuffer* data);
    void get153(FileBuffer* data, TFile::Mat* m);
    void get156(FileBuffer* data, TFile::Mat* m);
    void get157(FileBuffer* data);
    void get163(FileBuffer* data, int n);
    void get251(FileBuffer* data);
    
    void print();
    int cloneAMat(int id);
};

#endif	/* TFILE_H */

