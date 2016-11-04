/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIGNALSHAPE_H
#define	SIGNALSHAPE_H

#include <QString>

class FileBuffer;

class SignalShape {
public:
    enum SigSubType {
        UNDEFINED = 0,
        SIGNAL_HEAD = 1,
        NUMBER_PLATE = 2,
        GRADIENT_PLATE = 4,
        USER1 = 5, 
        USER2 = 6,
        USER3 = 7,
        USER4 = 8,
        DECOR = 9
    };
    
    static QMap< QString, SigSubType > SigSubTypeStringToId;
    
    struct SubObj {
        QString type;
        QString desc;
        QString sigSubType;
        int* sigSubJnLinkIf;
        int iLink;
        QString sigSubSType;
        bool isJnLink = false;
        bool optional = false;
        bool defaultt = false;
        bool backFacing = false;
        int sigSubTypeId = 0;
        int faceidx;
    };
    int listId;
    int iSubObj;
    SubObj* subObj;
    QString name;
    QString desc;
    //bool isJnLink = false;
    
    SignalShape();
    virtual ~SignalShape();
    void set(QString sh, FileBuffer* data);
private:

};

#endif	/* SIGNALSHAPE_H */

