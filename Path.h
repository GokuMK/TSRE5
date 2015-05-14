/* 
 * File:   Path.h
 * Author: Goku
 *
 * Created on 14 maja 2015, 00:36
 */

#ifndef PATH_H
#define	PATH_H

#include "TDB.h"

class Path {
public:
    Path();
    Path(const Path& orig);
    virtual ~Path();
    static void CreatePaths(TDB * tdb);
private:

};

#endif	/* PATH_H */

