#ifndef FILEBUFFER_H
#define	FILEBUFFER_H

#include <QString>

class FileBuffer {
public:
    FileBuffer();
    FileBuffer(unsigned char * data, int nLength);
    FileBuffer(const FileBuffer& orig);
    virtual ~FileBuffer();
    
    int getInt();
    unsigned short int getShort();
    float getFloat();
    QString* getString(int start, int end);
    unsigned char get();
    
    int off = 0;
    int length = 0;
    unsigned char * data = NULL;
private:
    
};

#endif	/* FILEBUFFER_H */

