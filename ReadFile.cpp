/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ReadFile.h"
//#include "zlib.h"

//unsigned char* ReadFile::out = new unsigned char[10000000];

FileBuffer* ReadFile::read(QFile* file) {
    int size = file->size();
    unsigned char* in = new unsigned char[size];
    file->read((char*)in, size);
    //QByteArray fileData = file->readAll();
    //unsigned char* in = (unsigned char*)fileData.data();
    //unsigned char* out = NULL;
    unsigned char* data = NULL;
    //int maxSize = 25000000;
    int nLength = 0;
    unsigned short bom = *((unsigned short int*) & in[0]);
    //for (int i = 0; i < 100; i++)
    //   qDebug() << ":" << (char)in[i];
    
    if (bom != 65279 && in[7] == 'F') {
        in[12] = in[11];
        in[13] = in[10];
        in[14] = in[9];
        in[15] = in[8];
        QByteArray out = qUncompress(QByteArray::fromRawData((const char*)(in+12), size-12));
        nLength = out.length() + 16;
        data = new unsigned char[nLength];
        std::copy(in, in + 16, data);
        std::copy(out.data(), out.data() + nLength - 16, data + 16);
        delete[] in;
    } else if (bom == 65279 && in[16] == 'F') {
        in[30] = in[19];
        in[31] = in[18];
        in[32] = in[17];
        in[33] = in[13];
        QByteArray out = qUncompress(QByteArray::fromRawData((const char*)(in+30), size-30));
        nLength = out.length() + 34;
        data = new unsigned char[nLength];
        std::copy(in, in + 34, data);
        std::copy(out.data(), out.data() + nLength - 34, data + 34);
        delete[] in;
    } else {
        //data = in + 16;// new unsigned char[fileData.length() - 16];
        nLength = size;
        data = in;//new unsigned char[nLength];
        //std::copy(in + 16, in + size, data);
        //std::copy(in + 16, in + nLength + 16, data);
        //delete[] in;
    }
    //delete[] in;
    return new FileBuffer(data,nLength);
}

FileBuffer* ReadFile::readRAW(QFile* file) {
    
    QByteArray fileData = file->readAll();
    unsigned char* in = (unsigned char*)fileData.data();
    int nLength = fileData.length();
    
    unsigned char* data = new unsigned char[nLength+1];
    data[nLength] = 0;
    std::copy(in, in + nLength, data);
    //delete[] in;
    return new FileBuffer(data,nLength);
}
