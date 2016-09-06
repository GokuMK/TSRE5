/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

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
    short int getSignedShort();
    unsigned int getUint();
    float getFloat();
    QString* getString(int start, int end);
    unsigned char get();
    void findToken(int id);
    int getToken();
    void setTokenOffset(int val);
    void insertFile(QString incPath);
    bool isBOM();
    void skipBOM();
    void toUtf16();
    
    int off = 0;
    int length = 0;
    int tokenOffset = 0;
    unsigned char * data = NULL;
private:
    
};

#endif	/* FILEBUFFER_H */

