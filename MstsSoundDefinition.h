/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef MSTSSOUNDDEFINITION_H
#define	MSTSSOUNDDEFINITION_H

#include <QString>
#include <QVector>
#include <QMap>
#include "Vector2f.h"

class FileBuffer;

class SoundDefinitionGroup {
    public:
        struct Activation {
        
        };

        struct Deactivation {

        };
        
        class Stream {
        public:
            struct Trigger {
                enum TriggerType {
                    EMPTY_TRIGGER = 0,
                    DISTANT_TRIGGER = 1,
                    DISCRETE_TRIGGER = 2,
                    RANDOM_TRIGGER = 3,
                    VARIABLE_TRIGGER = 4,
                    INITIAL_TRIGGER = 5
                };
                enum TriggerMode {
                    EMPTY_MODE = 0,
                    ONESHOT_MODE = 1,
                    LOOPSTART_MODE = 2,
                    LOOPRELEASE_MODE = 3
                };
                
                TriggerType type = EMPTY_TRIGGER;
                TriggerMode mode = EMPTY_MODE;
                QVector<QString> files;
                int delayMin = 500;
                int delayMax = 500;
                float volumeMin = 1.0;	
                float volumeMax = 1.0;		
                QString selectionMethod;
                int alBid = -1;
                
                bool initialtriggeract = false;
                
                Trigger();
                Trigger(Trigger *o);
                bool activate();
                void load(FileBuffer* data);
                QString getFileName();
            };

            struct Curve {
                QString type;
                QVector<Vector2f> points;
                
                float getValue(float x);
            };
            
            bool relative = false;
            int priority = 0;
            float volume = 1.0;
            QVector<Trigger*> trigger;
            Curve* volumeCurve = NULL;
            Curve* freqCurve = NULL;
            
            Stream();
            Stream(Stream *o);
            
            float X;
            float Y;
            float tilePos[3];
            float distance = 1000;
            int alSid = -1;
            bool isInit = false;
            
            float lastVolume = -1;
        
            void load(FileBuffer* data);
            void setPosition(int x, int y, float *pos);
            void update();
            void updatePosition();
            void setRelative(bool v);
            void init(QString path, bool stereo);
            void bindTo(int i);
        };
        
        Activation activation;
        Deactivation deactivation;
        QVector<Stream*> stream;
        QString resPath;
        
        int level = 0;
        float volume = 1.0;
        bool stereo = false;
        bool ignore3d = false;
        
        SoundDefinitionGroup(int l);
        void load(FileBuffer* data);
    };

class MstsSoundDefinition {
public:
    static QMap<int, MstsSoundDefinition*> Definitions;
    static int AddDefinition(QString path, QString name);
        
    int ref = 0;
    QString pathid;
    QString path;
    QString name;
    
    QVector<SoundDefinitionGroup*> group;
    SoundDefinitionGroup* currentGroup = NULL;
    
    MstsSoundDefinition();
    MstsSoundDefinition(QString p, QString n);
    virtual ~MstsSoundDefinition();
private:
    static int jestsms;
    bool loaded = false;
    
};

#endif	/* MSTSSOUNDDEFINITION_H */

