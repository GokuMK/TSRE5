/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SoundManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdbool.h>
#include "SoundSource.h"
#include "MstsSoundDefinition.h"
#include "ReadFile.h"
#include "FileBuffer.h"
#include "GLMatrix.h"

QMap<int, SoundSource*> SoundManager::Sources;
int SoundManager::SourcesCount;
ALCdevice *SoundManager::device = NULL;
ALCcontext *SoundManager::context = NULL;
float SoundManager::listenerPos[3] = {0,0,0};
int SoundManager::listenerX = 0;
int SoundManager::listenerZ = 0;

void SoundManager::InitAl() {
    ALboolean enumeration;
    ALCchar *defaultDeviceName = NULL;//argv[1];

    ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };

	enumeration = alcIsExtensionPresent(NULL, (ALCchar *)"ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	if (!defaultDeviceName)
		defaultDeviceName = (ALCchar *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	device = alcOpenDevice(defaultDeviceName);
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		return;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	alGetError();

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return;
	}
        
        ///////////////////////
	qDebug("make default context");

	/* set orientation */
	alListener3f(AL_POSITION, 0.0f, 0, 0.0f);
	qDebug("listener position");
    	alListener3f(AL_VELOCITY, 0, 0, 0);
	qDebug("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	qDebug("listener orientation");

        
                
	return;
}

void SoundManager::UpdateAll(){
    QMapIterator<int, SoundSource*> i(Sources);
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL) continue;
            i.value()->update();
        }
}

void SoundManager::UpdateListenerPos(int x, int y, float* pos, float* target, float* up){
    alListener3f(AL_POSITION, pos[0], pos[1], pos[2]);
    float o[6];
    o[0] = target[0] - pos[0];
    o[1] = target[1] - pos[1];
    o[2] = target[2] - pos[2];
    Vec3::normalize((float*)o, (float*)o);
    o[3] = up[0];
    o[4] = up[1];
    o[5] = up[2];
    alListenerfv(AL_ORIENTATION, (ALfloat*)o);

    if(listenerX != x || listenerZ != y){
        listenerX = x;
        listenerZ = y;
        QMapIterator<int, SoundSource*> i(Sources);
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL) continue;
            i.value()->updatePosition();
        }
    }
}

int SoundManager::AddSoundSource(QString resPath, QString name){
    // simple source
    Sources[SourcesCount] = new SoundSource(resPath, name);
    return SourcesCount++;
}

int SoundManager::AddSoundSource(SoundDefinitionGroup* g){
    if(g == NULL)
        return -1;
    
    Sources[SourcesCount] = new SoundSource(g);
    return SourcesCount++;
}



void SoundManager::CloseAl(){
    //alDeleteSources(1, &source);
    //alDeleteBuffers(1, &buffer);
    device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void SoundManager::list_audio_devices(const ALCchar *devices){
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}
/*
ALenum SoundManager::to_al_format(short channels, short samples)
{
	bool stereo = (channels > 1);

	switch (samples) {
	case 16:
		if (stereo)
			return AL_FORMAT_STEREO16;
		else
			return AL_FORMAT_MONO16;
	case 8:
		if (stereo)
			return AL_FORMAT_STEREO8;
		else
			return AL_FORMAT_MONO8;
	default:
		return -1;
	}
}*/
