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

void FileBuffer::insertFile(QString incPath){
    int i;
    QString sh;
    incPath.replace("\\","/");
    incPath.replace("//","/");
    //qDebug() << pathid;
    QFile *file = new QFile(incPath);
    if (!file->open(QIODevice::ReadOnly)){
        qDebug() << incPath << "not exist";
        return;
    }
    qDebug() << incPath;
    FileBuffer* incData = ReadFile::readRAW(file);
    int remaining = length-off;
    unsigned char * newData = new unsigned char[incData->length + remaining ];
    memcpy(newData, incData->data, incData->length);
    memcpy(newData+incData->length, data+off, remaining);
    delete[] data;
    data = newData;
    length = incData->length + remaining;
    off = 0;
}