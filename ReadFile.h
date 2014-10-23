/* 
 * File:   ReadFile.h
 * Author: Piotr
 *
 * Created on 19 październik 2014, 23:58
 */

#ifndef READFILE_H
#define	READFILE_H
#include <QtCore>
#include <iostream>
#include "FileBuffer.h"

class ReadFile {
public:
    static FileBuffer* read(QFile* file);
private:
    
};

#endif	/* READFILE_H */

