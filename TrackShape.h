/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRACKSHAPE_H
#define	TRACKSHAPE_H

#include <QString>

class QTextStream;
class FileBuffer;

class TrackShape {
public:
    struct SectionIdx{
        int n = 0;
        float pos[3];
        float rotDeg = 0;
        unsigned int sect[12];
    };
    
    QString filename;
    int id;
    int numpaths = 0;
    int mainroute = -1;
    int clearancedist = -1;
    bool crossovershape = false;
    bool tunnelshape = false;
    bool roadshape = false;
    bool manualjunctionshape = false;
    bool dyntrack = false;
    SectionIdx *path;
    int xoverpts = 0;
    float *xoverpt = NULL;
    
 //      } else if (sh == ("xoverpts")) {    
        
    TrackShape();
    TrackShape(int t);
    virtual ~TrackShape();
    QString getHashString();
    //void getShapePath();
    void saveToStream(QTextStream &out);
    void loadUtf16Data(FileBuffer *data);
private:

};

#endif	/* TRACKSHAPE_H */

