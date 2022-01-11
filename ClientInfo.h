/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <QString>

class OglObj;
class TextObj;
class GLUU;
class QWebSocket;

class ClientInfo {
public:
    QWebSocket *socket = NULL;
    QString username;
    QString password;
    bool loggedIn = false;
    int X;
    int Z;
    float x, y, z;
    QString lastAction;
    ClientInfo();
    ClientInfo(const ClientInfo& orig);
    virtual ~ClientInfo();
    void render(float playerRot);
private:
    OglObj* stick = NULL;
    TextObj* name = NULL;
};

#endif /* CLIENTINFO_H */

