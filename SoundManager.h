/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SOUNDMANAGER_H
#define	SOUNDMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <QString>
#include <QMap>

class SoundSource;
class SoundDefinitionGroup;

class SoundManager {
public:
    static QMap<int, SoundSource*> Sources;
    static int SourcesCount;
    static int listenerX;
    static int listenerZ;
    
    static void InitAl();
    static void CloseAl();
    static void UpdateListenerPos(int x, int y, float *pos, float* target, float* up);
    
    static int AddSoundSource(SoundDefinitionGroup* g);
    static int InitSource(QString path);
    
private:

    static ALCdevice *device;
    static ALCcontext *context;
    
    static void list_audio_devices(const ALCchar *devices);
    //static ALenum to_al_format(short channels, short samples);

    static float listenerPos[3];
};

#endif	/* SOUNDMANAGER_H */

