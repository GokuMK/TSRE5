/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ROUTECLIENT_H
#define ROUTECLIENT_H

#include "Route.h"

class RouteClient : public Route {
    Q_OBJECT
public:
    RouteClient();
    RouteClient(const RouteClient& orig);
    virtual ~RouteClient();
    Tile * requestTile(int x, int z, bool allowNew = true);
    void save();
    void load();
    void getUnsavedInfo(QVector<QString> &items);
    
signals:
    void initDone();
    
private:


};

#endif /* ROUTECLIENT_H */

