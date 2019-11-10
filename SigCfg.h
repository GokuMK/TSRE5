/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIGCFG_H
#define	SIGCFG_H

#include <QString>
#include <QHash>

class SignalShape;
class SignalType;
//class SignalLightTexture;
//class SignalLightDefinition;

class SigCfg {
public:
    //QHash<QString, SignalLightTexture*> lightTextures;
    //QHash<QString, SignalLightDefinition*> lights;
    QHash<QString, SignalShape*> signalShape;
    QHash<int, SignalShape*> signalShapeById;
    QHash<QString, SignalType*> signalType;
    SigCfg();
    virtual ~SigCfg();
private:
};

#endif	/* SIGCFG_H */

