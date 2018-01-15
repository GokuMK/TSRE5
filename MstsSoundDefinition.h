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

class FileBuffer;

class SoundDefinitionGroup {
    public:
        struct Activation {
        
        };

        struct Deactivation {

        };
        
        struct Stream {
            struct Trigger {
                QVector<QString> playOneShotFiles;
                int delayMin = 500;
                int delayMax = 500;
                float volumeMin = 1.0;	
                float volumeMax = 1.0;		
                QString selectionMethod;
                void load(FileBuffer* data);
                QString getFileName();
            };

            struct VolumeCurve {

            };

            struct FrequencyCurve {

            };

            int priority = 0;
            float volume = 1.0;
            QVector<Trigger*> trigger;
            VolumeCurve* volumeCurve = NULL;
            FrequencyCurve* freqCurve = NULL;
            
            void load(FileBuffer* data);
        };
        
        Activation activation;
        Deactivation deactivation;
        QVector<Stream*> stream;
        
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
    static int AddDefinition(QString pathid);
        
    int ref = 0;
    QString pathid;
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

