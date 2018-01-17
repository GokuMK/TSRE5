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
        stream.push_back(new SoundDefinitionGroup::Stream(g->stream[i]));
        stream.back()->init(g->resPath, g->stereo);


        
        //ALint source_state;
	//alGetSourcei(stream.back().alSid, AL_SOURCE_STATE, &source_state);
	//qDebug("source state get");
        
    }
}

SoundSource::~SoundSource() {
}

void SoundSource::setPosition(int x, int y, float *pos){
    for(int i = 0; i < stream.size(); i++)
        stream[i]->setPosition(x, y, pos);
}

void SoundSource::updatePosition(){
    for(int i = 0; i < stream.size(); i++)
        stream[i]->updatePosition();
}

void SoundSource::update(){

    for(int i = 0; i < stream.size(); i++){
        stream[i]->update();
        
        
        return;
    }
}

void SoundSource::setRelative(bool v){
    for(int i = 0; i < stream.size(); i++)
        stream[i]->setRelative(v);
}
