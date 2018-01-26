/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SOUNDVARIABLES_H
#define	SOUNDVARIABLES_H

#include <QMap>

class SoundVariables {
public:
    enum ValueName {
        NOVALUE = 0,
        SPEED = 1,
        DISTANCE = 2,
        BRAKECYL = 3,
        CURVEFORCE = 4,
        VARIABLE1 = 5,
        VARIABLE2 = 6,
        VARIABLE3 = 7,
    };
    QMap<ValueName, float> value;
    SoundVariables();
    SoundVariables(const SoundVariables& o);
    virtual ~SoundVariables();
    void set(SoundVariables *o);
};

#endif	/* SOUNDVARIABLES_H */

