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
#include <unordered_map>

class SignalShape;
class SignalType;

class SigCfg {
public:
    std::unordered_map<std::string, SignalShape*> signalShape;
    std::unordered_map<int, SignalShape*> signalShapeById;
    std::unordered_map<std::string, SignalType*> signalType;
    SigCfg();
    virtual ~SigCfg();
private:
};

#endif	/* SIGCFG_H */

