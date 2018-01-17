/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SOUNDSOURCE_H
#define	SOUNDSOURCE_H

#include <QVector>
#include <QString>

#include "MstsSoundDefinition.h"

class SoundSource {
public:
    QVector<SoundDefinitionGroup::Stream*> stream;
    void setPosition(int x, int y, float *pos);
    void update();
    void updatePosition();
    void setRelative(bool v);
    
    SoundSource(SoundDefinitionGroup *g);
    virtual ~SoundSource();
    
private:
    SoundDefinitionGroup *definition;
    int addNewStream(SoundDefinitionGroup::Stream* s, bool stereo);
    int alSourceId = -1;
};

#endif	/* SOUNDSOURCE_H */

