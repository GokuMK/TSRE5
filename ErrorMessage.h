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

class PreciseTileCoordinate;
class GameObj;

class ErrorMessage {
public:
    QString type;
    QString source;
    QString description;
    QString action;
    PreciseTileCoordinate *coords = NULL;
    GameObj* obj = NULL;
    ErrorMessage();
    ErrorMessage(QString type, QString source, QString description );
    ErrorMessage(QString type, QString source, QString description, QString action );
    ErrorMessage(const ErrorMessage& orig);
    void setLocation(PreciseTileCoordinate *c);
    void setLocationXYZ(float wX, float wZ, float x, float y, float z);
    void setObject(GameObj *o);
    virtual ~ErrorMessage();
private:

};

#endif /* ERRORMESSAGE_H */

