/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TarFile.h"
#include "ReadFile.h"
#include <QtMath>

TarFile::TarFile() {
}

TarFile::TarFile(QString path) {
    filePath = path;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() <<"#TAR file not found: "<< filePath;
        return;
    }
    data = ReadFile::readRAW(&file);
}

TarFile::TarFile(FileBuffer *oData, bool copy) {
    if(copy){
        data = new FileBuffer(oData);
    } else {
        data = oData;
    }
}

bool TarFile::extractTo(QString destination){
    if(data == NULL)
        return false;
    
    qDebug() << "#TarFile Extracting "<< filePath << data->length;
    
    PosixHeader *tFileHeader;

    while(data->off < data->length - 512){
        tFileHeader = (PosixHeader*)(&data->data[data->off]);
        //qDebug() << tFileHeader->name;
        //qDebug() << tFileHeader->size;
        unsigned int tFilesize = QString(tFileHeader->size).toInt(NULL, 8);
        //qDebug() << tFilesize;
        data->off += 512;
        QString tFileName = QString(tFileHeader->name);
        if(tFileHeader->typeflag == '5' && tFileName.length() > 0){
            qDebug() << "MkDir: " << tFileName;
            QDir().mkdir(destination + tFileName);
        } else if(tFileName.length() > 0){
            QFile ttFile(destination+tFileName);
            qDebug() << "Write: " << tFileName << tFilesize;
            if (!ttFile.open(QIODevice::WriteOnly)){
                qDebug() << "Write error - can't open file.";
            } else {
                ttFile.write((const char*)&data->data[data->off], tFilesize);
                ttFile.close();
            }
        }
        data->off += tFilesize + (512 - (tFilesize % 512)) % 512;
    }
    return true;
}

TarFile::TarFile(const TarFile& orig) {
}

TarFile::~TarFile() {
    delete data;
}

