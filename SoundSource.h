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

class SoundDefinitionGroup;

class SoundSource {
public:
    struct Stream {
        float X;
        float Y;
        float tilePos[3];
        float distance = 1000;
        int alSid = -1;
        int alBid = -1;
        void setPosition(int x, int y, float *pos);
        void updatePosition();
    };
    
    QVector<Stream> stream;
    void setPosition(int x, int y, float *pos);
    void updatePosition();
    
    SoundSource(SoundDefinitionGroup *g);
    virtual ~SoundSource();
    
private:
    SoundDefinitionGroup *definition;
    int addNewStream(QString path, bool stereo);
    int alSourceId = -1;
};

#endif	/* SOUNDSOURCE_H */

