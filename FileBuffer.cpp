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
#include "ReadFile.h"
#include <QFile>
#include <QDebug>
#include <string> 

FileBuffer::FileBuffer() {
    this->off = 0;
}

FileBuffer::FileBuffer(unsigned char * data, int nLength) {
    this->data = data;
    this->length = nLength;
    this->off = 0;
}

FileBuffer::FileBuffer(const FileBuffer& orig) {
}

FileBuffer::~FileBuffer() {
    delete[] this->data;
}

int FileBuffer::getInt() {
    this->off += 4;
    return *((int*) & this->data[this->off - 4]);
}

int FileBuffer::getToken(){
    this->off += 4;
    return (*((int*) & this->data[this->off - 4]) - this->tokenOffset);
}

void FileBuffer::setTokenOffset(int val){
    this->tokenOffset = val;
}

unsigned int FileBuffer::getUint() {
    this->off += 4;
    return *((unsigned int*) & this->data[this->off - 4]);
}

unsigned short int FileBuffer::getShort() {
    this->off += 2;
    //return this->data[this->off - 2]*256 + 0;
    return *((unsigned short int*) & this->data[this->off - 2]);
}

void FileBuffer::skipBOM(){
    if(this->getShort() == 65279)
        return;
    off -= 2;
    return;
}

bool FileBuffer::isBOM(){
    if(this->getShort() == 65279){
        off -= 2;
        return true;
    }
    off -= 2;
    return false;
}

short int FileBuffer::getSignedShort() {
    this->off += 2;
    //return this->data[this->off - 2]*256 + 0;
    return *((short int*) & this->data[this->off - 2]);
}

float FileBuffer::getFloat() {
    this->off += 4;
    return *(float*) & this->data[this->off - 4];
}

unsigned char FileBuffer::get() {
    return this->data[this->off++];
}

QString* FileBuffer::getString(int start, int end) {
    QString* s = new QString();

    for (int i = start; i < end; i += 2) {
        if (data[i] == 13) continue;
        *s += QChar(data[i], data[i + 1]);
    }
    return s;

}

void FileBuffer::findToken(int id) {
    int s;
    while (length > off) {
        s = (int) getInt();
        if (s == id)
            return;
        s = getInt();
        off += s;
    }
    return;
}

void FileBuffer::toUtf16(){
    if(isBOM()) return;
    qDebug() << "converting to UTF16";
    // 
    unsigned char * newData = new unsigned char[length * 2];
    for(int i = 0; i < length; i++){
        newData[i*2] = data[i];
        newData[i*2+1] = 0;
    }
    length = length * 2;
    delete[] data;
    data = newData;
}

void FileBuffer::insertFile(QString incPath){
    int i;
    QString sh;
    incPath.replace("\\","/");
    incPath.replace("//","/");
    //qDebug() << pathid;
    QFile file(incPath);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug() << incPath << "not exist";
        return;
    }
    qDebug() << incPath;
    FileBuffer* incData = ReadFile::readRAW(&file);
    incData->toUtf16();
    int remaining = length-off;
    unsigned char * newData = new unsigned char[incData->length + remaining ];
    memcpy(newData, incData->data, incData->length);
    memcpy(newData+incData->length, data+off, remaining);
    delete[] data;
    data = newData;
    length = incData->length + remaining;
    off = 0;
    skipBOM();
}