/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRNODE_H
#define	TRNODE_H

#include "Vector2i.h"
#include <QString>

class QTextStream;
class FileBuffer;

class TRnode {
public:
    struct TRSect {
        float param[16];
    };
    int typ;
    int args[3];
    float UiD[12];
    int iTrv = 0;
    TRSect *trVectorSection = NULL;
    int iTri = 0;
    int *trItemRef = NULL;
    int TrP1 = 0;
    int TrP2 = 0;
    int TrPinS[3];
    int TrPinK[3];
    
    TRnode();
    TRnode(const TRnode& orig);
    virtual ~TRnode();
    Vector2i* getTile();
    void loadUtf16Data(FileBuffer *data);
    void saveToStream(QTextStream &out, int nid);
    bool isEnd();
    bool equals(TRnode* r);
    bool equalsIgnoreType(TRnode* r);
    int podmienTrPin(int stare, int nowe);
    int setTrPinK(int id, int nowe);
    float getVectorSectionXRot(int id);
    void addPositionOffset(float offsetXYZ[3]);
    void addTrackNodeItemOffset(unsigned int trackNodeOffset, unsigned int trackItemOffset);
private:

};

#endif	/* TRNODE_H */

