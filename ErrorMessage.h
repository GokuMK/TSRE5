/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QString>
#include <QHash>

class PreciseTileCoordinate;
class GameObj;

class ErrorMessage {
public:
    enum MsgType {
        Type_Info = 0,
        Type_Warning = 1,
        Type_Error = 3,
        Type_AutoFix = 4
    };
    
    enum SourceType {
        Source_Other,
        Source_TDB,
        Source_RDB,
        Source_World,
        Source_Editor
    };
    
    static QHash<MsgType, QString> TypeNames;
    static QHash<SourceType, QString> SourceNames;
    
    unsigned long long time = 0;
    MsgType type;
    SourceType source;
    QString description;
    QString action;
    PreciseTileCoordinate *coords = NULL;
    GameObj* obj = NULL;
    ErrorMessage();
    ErrorMessage(MsgType type, SourceType source, QString description );
    ErrorMessage(MsgType type, SourceType source, QString description, QString action );
    ErrorMessage(const ErrorMessage& orig);
    void setLocation(PreciseTileCoordinate *c);
    void setLocationXYZ(float wX, float wZ, float x, float y, float z);
    void setObject(GameObj *o);
    virtual ~ErrorMessage();
private:

};

#endif /* ERRORMESSAGE_H */

