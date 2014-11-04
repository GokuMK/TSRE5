#ifndef FILEBUFFER_H
#define	FILEBUFFER_H

class FileBuffer {
public:
    FileBuffer();
    FileBuffer(unsigned char * data, int nLength);
    FileBuffer(const FileBuffer& orig);
    virtual ~FileBuffer();
    
    int getInt();
    unsigned short int getShort();
    float getFloat();
    unsigned char get();
    
    int off = 1;
    int length;
    unsigned char * data;
private:
    
};

#endif	/* FILEBUFFER_H */

