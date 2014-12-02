#ifndef READFILE_H
#define	READFILE_H
#include <QtCore>
#include <iostream>
#include "FileBuffer.h"

class ReadFile {
public:
    static FileBuffer* read(QFile* file);
    static FileBuffer* readRAW(QFile* file);
private:
    
};

#endif	/* READFILE_H */

