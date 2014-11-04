#ifndef TDB_H
#define	TDB_H
#include <QString>
#include "TSectionDAT.h"
#include "TRnode.h"
#include "FileBuffer.h"

class TDB {
public:
    bool loaded;
    TSectionDAT *tsection; 
    TRnode *trackNodes;
    TDB(QString path);
    TDB(const TDB& orig);
    virtual ~TDB();
    void trpin(TRnode* tr, FileBuffer* bufor );
private:
    
};

#endif	/* TDB_H */

