/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TDBCLIENT_H
#define TDBCLIENT_H

#include "TDB.h"

class TDBClient : public TDB {
public:
    TDBClient(TSectionDAT* tsection, bool road);
    TDBClient(const TDBClient& o);
    virtual ~TDBClient();
    int getNextItrNode();
    int getNewTRitemId();
    void updateTrItem(int iid);
    void updateTrNode(int nid);
    void updateTrackSection(int id);
    void updateTrackShape(int id);
private:

};

#endif /* TDBCLIENT_H */

