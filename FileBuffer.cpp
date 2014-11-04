#include "FileBuffer.h"

FileBuffer::FileBuffer() {
}

FileBuffer::FileBuffer(unsigned char * data, int nLength) {
    this->data = data;
    this->length = nLength;
    this->off = 0;
}

FileBuffer::FileBuffer(const FileBuffer& orig){
}

FileBuffer::~FileBuffer() {
    delete this->data;
}

int FileBuffer::getInt(){
    this->off+=4;
    return *((int*)&this->data[this->off-4]);
}

unsigned short int FileBuffer::getShort(){
    this->off+=2;
    return this->data[this->off-2]*256 + 0;
    //return *((unsigned short int*)&this->data[this->off-2]);
}

float FileBuffer::getFloat(){
    this->off+=4;
    return *(float*)&this->data[this->off-4];
}

unsigned char FileBuffer::get(){
    this->off++;
    return this->data[this->off-1];
}