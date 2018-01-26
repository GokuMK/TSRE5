/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#include "SoundVariables.h"

SoundVariables::SoundVariables() {
}

SoundVariables::SoundVariables(const SoundVariables& o) {
    QMapIterator<ValueName, float> i(o.value);
    while (i.hasNext()) {
        i.next();
        value[i.key()] = i.value();
    }
}

void SoundVariables::set(SoundVariables* o){
    QMapIterator<ValueName, float> i(o->value);
    while (i.hasNext()) {
        i.next();
        value[i.key()] = i.value();
    }
}

SoundVariables::~SoundVariables() {
}

