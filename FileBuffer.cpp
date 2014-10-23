#include "FileBuffer.h"

FileBuffer::FileBuffer() {
}

FileBuffer::FileBuffer(unsigned char * data, int nLength) {
    this->data = data;
    this->length = nLength;
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

float FileBuffer::getFloat(){
    this->off+=4;
    return *(float*)&this->data[this->off-4];
}

unsigned char FileBuffer::get(){
    this->off++;
    return *(unsigned char*)&this->data[this->off-1];
}