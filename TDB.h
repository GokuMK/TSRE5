#ifndef TDB_H
#define	TDB_H
#include <QString>
#include "TSectionDAT.h"
#include "TRnode.h"
#include "FileBuffer.h"
#include "GLUU.h"
#include "OglObj.h"

class TDB {
public:
    bool loaded;
    TSectionDAT *tsection; 
    TRnode *trackNodes;
    int iTRnodes;
    TDB(QString path);
    TDB(const TDB& orig);
    virtual ~TDB();
    void trpin(TRnode* tr, FileBuffer* bufor );
    void renderAll(GLUU *gluu, float * playerT);
    void renderLines(GLUU *gluu, float* playerT);
private:
    int getLineBufferSize(int idx);
    void drawLine(GLUU *gluu, float* &ptr, Vector3f p, Vector3f o, int idx);
    OglObj linieSieci;
    OglObj konceSieci;
    OglObj punktySieci;
    OglObj sectionLines;
    int wysokoscSieci;
    int lineHash;
    int sectionHash;
};

#endif	/* TDB_H */

