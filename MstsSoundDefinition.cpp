/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "MstsSoundDefinition.h"
#include <QDebug>
#include <QFile>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "GLMatrix.h"
#include <AL/al.h>
#include <AL/alc.h>
#include "SoundManager.h"
#include "SoundSource.h"

int MstsSoundDefinition::jestsms = 0;
QMap<int, MstsSoundDefinition*> MstsSoundDefinition::Definitions;

MstsSoundDefinition::MstsSoundDefinition() {
}

MstsSoundDefinition::MstsSoundDefinition(QString p, QString n) {
    path = p;
    pathid = p + "/" + n;
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    name = n;
    
    QString sh;
    pathid.replace("//", "/");
    qDebug() << "SMS" << pathid;
    QFile *file = new QFile(pathid);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << pathid << "not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    data->skipBOM();

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("simisa@@@@@@@@@@jinx0x1t______")) {
            continue;
        }
        
        if (sh == ("tr_sms")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("scalabiltygroup")) {
                    group.push_back(new SoundDefinitionGroup(ParserX::GetNumber(data)));
                    group.back()->resPath = path;
                    group.back()->load(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#tr_sms - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
            continue;
            
            continue;
        }
        qDebug() << "#SMS - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
    delete data;
    loaded = 1;

    return;
}

QString SoundDefinitionGroup::Stream::Trigger::getFileName(){
    return Game::root+"/routes/"+Game::route+"/sound/"+files.first();
}

void SoundDefinitionGroup::Stream::setRelative(bool v){
    relative = v;
    if(relative)
        alSourcei(alSid, AL_SOURCE_RELATIVE, AL_TRUE);
    else
        alSourcei(alSid, AL_SOURCE_RELATIVE, AL_FALSE);
}

bool SoundDefinitionGroup::Stream::Trigger::activate(SoundVariables *variables){
    if(type == INITIAL_TRIGGER){
        if(initialtriggeract == false){
            initialtriggeract = true;
            return true;
        }
    }
    if(type == RANDOM_TRIGGER){
        if(initialtriggeract == false){
            initialtriggeract = true;
            return true;
        }
    }
    if(type == VARIABLE_TRIGGER){
        if(variables == NULL)
            return false;
        if(variabletype == "Variable2_Inc_Past"){
            float v = variables->value[SoundVariables::VARIABLE2];
            if(lastvalue <= variablevalue && v > variablevalue){
                lastvalue = v;
                return true;
            }
        }
        if(variabletype == "Variable2_Dec_Past"){
            float v = variables->value[SoundVariables::VARIABLE2];
            if(lastvalue >= variablevalue && v < variablevalue){
                lastvalue = v;
                return true;
            }
        }
        lastvalue = variables->value[SoundVariables::VARIABLE2];
    }
    
    
    return false;
}
void SoundDefinitionGroup::Stream::Trigger::load(FileBuffer* data){
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("delay_min_max")) {
            delayMin = ParserX::GetNumber(data);
            delayMax = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("volume_min_max")) {
            volumeMin = ParserX::GetNumber(data);
            volumeMax = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("selectionmethod")) {
            selectionMethod = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("playoneshot")) {
            mode = ONESHOT_MODE;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("file")) {
                    files.push_back(ParserX::GetStringInside(data));
                    qDebug() << files.back();
                    ParserX::SkipToken(data);
                    continue;
                }

                qDebug() << "#triggers - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("startloop")) {
            mode = LOOPSTART_MODE;
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("file")) {
                    files.push_back(ParserX::GetStringInside(data));
                    qDebug() << files.back();
                    ParserX::SkipToken(data);
                    continue;
                }

                qDebug() << "#triggers - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("releaselooprelease")) {
            mode = LOOPRELEASE_MODE;
            ParserX::SkipToken(data);
            continue;
        }

        qDebug() << "#Stream - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void SoundDefinitionGroup::Stream::load(FileBuffer* data){
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("priority")) {
            priority = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("triggers")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("random_trigger")) {
                    trigger.push_back(new Trigger());
                    trigger.back()->type = Trigger::RANDOM_TRIGGER;
                    trigger.back()->load(data);
                    //priority = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("initial_trigger")) {
                    trigger.push_back(new Trigger());
                    trigger.back()->type = Trigger::INITIAL_TRIGGER;
                    trigger.back()->load(data);
                    //priority = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("variable_trigger")) {
                    trigger.push_back(new Trigger());
                    trigger.back()->type = Trigger::VARIABLE_TRIGGER;
                    trigger.back()->variabletype = ParserX::GetStringInside(data);
                    trigger.back()->variablevalue = ParserX::GetNumberInside(data);
                    trigger.back()->load(data);
                    //priority = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }

                qDebug() << "#triggers - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("frequencycurve")) {
            freqCurve = new Curve();
            freqCurve->type = ParserX::GetString(data);
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("curvepoints")) {
                    int count = ParserX::GetNumber(data);
                    for(int i = 0; i < count; i++){
                        float x = ParserX::GetNumberInside(data);
                        float y = ParserX::GetNumberInside(data);
                        freqCurve->points.push_back(Vector2f(x,y));
                    }                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#curve - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            qDebug() << "freqCurve.points" <<freqCurve->points.size();
            for(int i = 0; i < freqCurve->points.size(); i++)
                qDebug() << freqCurve->points[i].x << freqCurve->points[i].y;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("volumecurve")) {
            volumeCurve = new Curve();
            volumeCurve->type = ParserX::GetString(data);
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("curvepoints")) {
                    int count = ParserX::GetNumber(data);
                    for(int i = 0; i < count; i++){
                        float x = ParserX::GetNumberInside(data);
                        float y = ParserX::GetNumberInside(data);
                        volumeCurve->points.push_back(Vector2f(x,y));
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#curve - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            qDebug() << "volumeCurve.points" <<volumeCurve->points.size();
            for(int i = 0; i < volumeCurve->points.size(); i++)
                qDebug() << volumeCurve->points[i].x << volumeCurve->points[i].y;
            ParserX::SkipToken(data);
            continue;
        }

        qDebug() << "#Stream - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

float SoundDefinitionGroup::Stream::Curve::getValue(SoundVariables *variables){
    if(variables == NULL)
        return 0;
    //qDebug() << "X" <<x;
    if(points.size() == 0)
        return 0;

    float x = variables->value[SoundVariables::VARIABLE2];
    
    if(x <= points.first().x)
        return points.first().y;
    
    if(x >= points.last().x)
        return points.last().y;
    
    for(int i = 0; i < points.size()-1; i++){

        if(x > points[i].x && x <= points[i+1].x){
            float distance = points[i+1].x - points[i].x;
            x = (x - points[i].x) / distance;
            //qDebug() << "X Y" << x << points[i].y*(1.0 - x) + points[i+1].y*(x);
            return points[i].y*(1.0 - x) + points[i+1].y*(x);
        }
    }
    return 0;    
}

SoundDefinitionGroup::SoundDefinitionGroup(int l){
    level = l;
}

void SoundDefinitionGroup::load(FileBuffer* data){
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == ("activation")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("deactivation")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("volume")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("stereo")) {
            stereo = true;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ignore3d")) {

            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("streams")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("stream")) {
                    stream.push_back(new Stream());
                    stream.back()->load(data);
                    ParserX::SkipToken(data);
                    continue;
                }
            }
            ParserX::SkipToken(data);
            continue;
        }

        qDebug() << "#ScalabiltyGroup - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

MstsSoundDefinition::~MstsSoundDefinition() {
}

int MstsSoundDefinition::AddDefinition(QString path, QString name){
    QString pathid = path + "/" + name;
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    //console.log(pathid);
    QMapIterator<int, MstsSoundDefinition*> i(Definitions);
    while (i.hasNext()) {
    //for ( auto it = route->ref->refItems.begin(); it != route->ref->refItems.end(); ++it ){
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        i.next();
        if(i.value() == NULL) continue;
        if (i.value()->pathid.length() == pathid.length())
            if (i.value()->pathid == pathid) {
                i.value()->ref++;
                return (int)i.key();
            }
    }
    qDebug() << "Nowy " << jestsms << " sms: " << pathid;

    Definitions[jestsms] = new MstsSoundDefinition(path, name);
    Definitions[jestsms]->pathid = pathid;

    return jestsms++;
}

SoundDefinitionGroup::Stream::Trigger::Trigger(){
    
}

SoundDefinitionGroup::Stream::Trigger::Trigger(Trigger* o){
    type = o->type;
    mode = o->mode;
    files = o->files;
    delayMin = o->delayMin;
    delayMax = o->delayMax;
    volumeMin = o->volumeMin;	
    volumeMax = o->volumeMax;		
    selectionMethod = o->selectionMethod;
    alBid = -1;
    
    variabletype = o->variabletype;
    variablevalue = o->variablevalue;
    lastvalue = -1;
}

SoundDefinitionGroup::Stream::Stream(){
    
}

SoundDefinitionGroup::Stream::Stream(Stream *o){
    priority = o->priority;
    volume = o->volume;
    for(int i = 0; i < o->trigger.size(); i++)
        trigger.push_back(new Trigger(o->trigger[i]));
    volumeCurve = o->volumeCurve;
    freqCurve = o->freqCurve;
            
    X = o->X;
    Y = o->Y;
    Vec3::copy(tilePos, o->tilePos);
    distance = o->distance;
    alSid = -1;
}

void SoundDefinitionGroup::Stream::setPosition(int x, int y, float *pos){
    X = x;
    Y = y;
    tilePos[0] = pos[0];
    tilePos[1] = pos[1];
    tilePos[2] = pos[2];
    
    alSource3f(alSid, AL_POSITION, tilePos[0] - 2048*(SoundManager::listenerX-x), tilePos[1], tilePos[2] - 2048*(SoundManager::listenerZ-y));
}

void SoundDefinitionGroup::Stream::update(SoundVariables *variables){
    for(int i = 0; i < trigger.size(); i++){
        if(trigger[i]->activate(variables)){
            if(trigger[i]->mode == Trigger::ONESHOT_MODE){
                bindTo(trigger[i]->alBid);
                qDebug() << "bind" << i << alSid << trigger[i]->alBid;
                alSourcei(alSid, AL_LOOPING, AL_FALSE);
                alSourcePlay(alSid);
                isInit = true;
            }
            if(trigger[i]->mode == Trigger::LOOPSTART_MODE){
                bindTo(trigger[i]->alBid);
                qDebug() << "bind" << i << alSid << trigger[i]->alBid;
                alSourcei(alSid, AL_LOOPING, AL_TRUE);
                alSourcePlay(alSid);
                isInit = true;
            }
            if(trigger[i]->mode == Trigger::LOOPRELEASE_MODE){
                bindTo(0);
                qDebug() << "u-bind" << i << alSid << "NULL";
                alSourceStop(alSid);
                isInit = true;
            }
        }
        /*if(gain < 0.5 && i == 2 && isInit){
            alSourceStop(alSid);
            bindTo(trigger[i]->alBid);
            qDebug() << alSid << trigger[i]->alBid;
            alSourcePlay(alSid);
            gain = 1.0;
        }*/
    }

    if(volumeCurve != NULL){
        float newv = volumeCurve->getValue(variables);
        alSourcef(alSid, AL_GAIN, newv);
    }
    if(freqCurve != NULL){
        float newv = freqCurve->getValue(variables);
        alSourcef(alSid, AL_PITCH, newv/12025.0);
    }
}

void SoundDefinitionGroup::Stream::updatePosition(){
    if(relative)
        alSource3f(alSid, AL_POSITION, 0, 0, 0);    
    else
        alSource3f(alSid, AL_POSITION, tilePos[0] - 2048*(SoundManager::listenerX-X), tilePos[1], tilePos[2] - 2048*(SoundManager::listenerZ-Y));    
}

void SoundDefinitionGroup::Stream::bindTo(int i){
    alSourcei(alSid, AL_BUFFER, i);
    //qDebug("buffer binding");
}

void SoundDefinitionGroup::Stream::init(QString path, bool stereo){

    alGenSources((ALuint)1, (ALuint*)&alSid);
    qDebug("source generation");

    float pitch = 1.0;
    //if(stereo)
    //    pitch = 2.0;
        
    alSourcef(alSid, AL_PITCH, pitch);
    qDebug("source pitch");
    alSourcef(alSid, AL_GAIN, 1.0);
    qDebug("source gain");
        
    alSourcei(alSid, AL_SOURCE_RELATIVE, AL_FALSE);
        
    alSourcef(alSid, AL_REFERENCE_DISTANCE, 1);
    alSourcef(alSid, AL_MAX_DISTANCE, 1000);
    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
        
    alSource3f(alSid, AL_POSITION, 0, 0, 0);
    qDebug("source position");
    alSource3f(alSid, AL_VELOCITY, 0, 0, 0);
    qDebug("source velocity");
    alSourcei(alSid, AL_LOOPING, AL_TRUE);
    qDebug("source looping");


    for(int i = 0; i < trigger.size(); i++){
        if(trigger[i]->files.size() < 1)
            continue;
        if(trigger[i]->mode != Trigger::LOOPSTART_MODE && trigger[i]->mode != Trigger::ONESHOT_MODE )
            continue;
        alGenBuffers(1, (ALuint*)&trigger[i]->alBid);
        qDebug("buffer generation");
        QString filepath = path + "/" + trigger[i]->files.first();
        filepath.replace("//", "/");
        QFile file(filepath);
        qDebug()<< "vaw" << file.fileName();
        if (!file.open(QIODevice::ReadOnly)){
            qDebug() << "fail vaw file "<< file.fileName();
            return;
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
        dataChunkSize = dataChunkSize/channels;
        unsigned char* bufferData = new unsigned char[dataChunkSize];
        if (channels == 1)
            for(int ii = 0; ii < dataChunkSize; ii++)
                bufferData[ii] = filedata->get();
        if (channels == 2)
            for(int ii = 0; ii < dataChunkSize; ii+=2){
                filedata->get();
                filedata->get();
                bufferData[ii] = filedata->get();
                bufferData[ii+1] = filedata->get();
            }

        float duration = float(dataChunkSize) / byteRate;
        ALenum formatinfo = AL_FORMAT_STEREO16;
        //if (channels == 1)
                formatinfo = AL_FORMAT_MONO16;
        alBufferData(trigger[i]->alBid, formatinfo, bufferData, dataChunkSize, sampleRate);

	qDebug("loading wav file");

	qDebug("buffer copy");

    }
    

}
