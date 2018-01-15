/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SoundSource.h"
#include "SoundManager.h"
#include "MstsSoundDefinition.h"
#include "ReadFile.h"
#include "FileBuffer.h"

SoundSource::SoundSource(SoundDefinitionGroup *g) {
    definition = g;
    for(int i = 0; i < g->stream.size(); i++){
        //this->stream.push_back(new Stream());
        
        
        addNewStream(g->stream[i]->trigger.first()->getFileName(), g->stereo);
        
        alSourcePlay(stream.back().alSid);
	qDebug("source playing");
        
        ALint source_state;
	alGetSourcei(stream.back().alSid, AL_SOURCE_STATE, &source_state);
	qDebug("source state get");
        
    }
}

SoundSource::~SoundSource() {
}

void SoundSource::setPosition(int x, int y, float *pos){
    for(int i = 0; i < stream.size(); i++)
        stream[i].setPosition(x, y, pos);
}

void SoundSource::updatePosition(){
    for(int i = 0; i < stream.size(); i++)
        stream[i].updatePosition();
}

void SoundSource::Stream::setPosition(int x, int y, float *pos){
    X = x;
    Y = y;
    tilePos[0] = pos[0];
    tilePos[1] = pos[1];
    tilePos[2] = pos[2];
    
    alSource3f(alSid, AL_POSITION, tilePos[0] - 2048*(SoundManager::listenerX-x), tilePos[1], tilePos[2] - 2048*(SoundManager::listenerZ-y));
}

void SoundSource::Stream::updatePosition(){
    alSource3f(alSid, AL_POSITION, tilePos[0] - 2048*(SoundManager::listenerX-X), tilePos[1], tilePos[2] - 2048*(SoundManager::listenerZ-Y));    
}

int SoundSource::addNewStream(QString path, bool stereo){
    stream.push_back(Stream());
    
    
	alGenSources((ALuint)1, (ALuint*)&stream.back().alSid);
	qDebug("source generation");

        float pitch = 1.0;
        if(stereo)
            pitch = 2.0;
        
	alSourcef(stream.back().alSid, AL_PITCH, pitch);
	qDebug("source pitch");
	alSourcef(stream.back().alSid, AL_GAIN, 1.0);
	qDebug("source gain");
        
        alSourcei(stream.back().alSid, AL_SOURCE_RELATIVE, AL_FALSE);
        
        alSourcef(stream.back().alSid, AL_REFERENCE_DISTANCE, 1);
        alSourcef(stream.back().alSid, AL_MAX_DISTANCE, 1000);
        alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
        
	alSource3f(stream.back().alSid, AL_POSITION, 0, 0, 0);
	qDebug("source position");
	alSource3f(stream.back().alSid, AL_VELOCITY, 0, 0, 0);
	qDebug("source velocity");
	alSourcei(stream.back().alSid, AL_LOOPING, AL_FALSE);
	qDebug("source looping");

	alGenBuffers(1, (ALuint*)&stream.back().alBid);
	qDebug("buffer generation");

        QFile file(path);
        if (!file.open(QIODevice::ReadOnly)){
            qDebug() << "fail vaw file "<< file.fileName();
            return -1;
        }
        FileBuffer* filedata = ReadFile::read(&file);
        
        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();
        
        qDebug() << filedata->getInt();

        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();

        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();

        unsigned short chunkSize = filedata->getUint();
        
        unsigned short audioFormat = filedata->getShort();
        qDebug() <<"audioFormat"<<audioFormat;
        unsigned short channels = filedata->getShort();
        qDebug() <<"channels"<<channels;
        unsigned int sampleRate = filedata->getUint();
        qDebug() <<"sampleRate"<<sampleRate;
        unsigned int byteRate = filedata->getUint();
        qDebug() <<"byteRate" << byteRate;
        filedata->getSignedShort();
        unsigned short bitsPerSample = filedata->getShort();
        qDebug() <<"bitsPerSample"<<bitsPerSample;

        if (chunkSize != 16) {
                unsigned short extraParams = filedata->getShort();
                filedata->off += extraParams;
        }

        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();
        qDebug() << filedata->get();

        unsigned int dataChunkSize = filedata->getUint();
        qDebug() << "dataChunkSize" << dataChunkSize;
        unsigned char* bufferData = new unsigned char[dataChunkSize];
        for(int ii = 0; ii < dataChunkSize; ii++)
            bufferData[ii] = filedata->get();

        float duration = float(dataChunkSize) / byteRate;
        ALenum formatinfo = AL_FORMAT_STEREO16;
        //if (channels == 1)
                formatinfo = AL_FORMAT_MONO16;
        alBufferData(stream.back().alBid, formatinfo, bufferData, dataChunkSize, sampleRate);

	qDebug("loading wav file");

	qDebug("buffer copy");

	alSourcei(stream.back().alSid, AL_BUFFER, stream.back().alBid);
	qDebug("buffer binding");
}
