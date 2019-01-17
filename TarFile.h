/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TARFILE_H
#define	TARFILE_H

#include <QString>
#include "FileBuffer.h"

class TarFile {
public:
    struct PosixHeader {
        char name[100];               /*   0 */
        char mode[8];                 /* 100 */
        char uid[8];                  /* 108 */
        char gid[8];                  /* 116 */
        char size[12];                /* 124 */
        char mtime[12];               /* 136 */
        char chksum[8];               /* 148 */
        char typeflag;                /* 156 */
        char linkname[100];           /* 157 */
        char magic[6];                /* 257 */
        char version[2];              /* 263 */
        char uname[32];               /* 265 */
        char gname[32];               /* 297 */
        char devmajor[8];             /* 329 */
        char devminor[8];             /* 337 */
        char prefix[155];             /* 345 */
    };
    
    TarFile();
    TarFile(QString path);
    TarFile(FileBuffer *oData, bool copy = true);
    TarFile(const TarFile& orig);
    bool extractTo(QString destination);
    virtual ~TarFile();
private:
    QString filePath;
    FileBuffer* data = NULL;
};

#endif	/* TARFILE_H */

