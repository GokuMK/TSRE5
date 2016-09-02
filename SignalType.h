/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIGNALTYPE_H
#define	SIGNALTYPE_H

#include <QString>

class FileBuffer;

class SignalType {
public:
    QString type;
    SignalType();
    virtual ~SignalType();
    void set(QString sh, FileBuffer* data);
private:

};

#endif	/* SIGNALTYPE_H */

