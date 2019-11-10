/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIGNALTYPE_H
#define	SIGNALTYPE_H

#include <QString>
#include <QVector>
#include <QHash>

class FileBuffer;

class SignalType {
    
public:
    
    struct Light {
        int id;
        QString defName;
        float position[3];
        float radius;
        
        Light();
        void set(QString sh, FileBuffer* data);
    };
    
    struct DrawState {
        int id;
        QString name;
        QVector<int> lightDrawList;
        QVector<int> lightModeList;
        
        DrawState();
        void set(QString sh, FileBuffer* data);
    };
    
    QHash<int, Light*> lights;
    QHash<QString, DrawState*> drawStates;
    
    QString type;
    QString signalFnType;// = NORMAL;
    QString signalLightTex;// ( "ltex" )
    QVector<float> sigFlashDuration;// ( 0.6 0.25 )
    QString signalFlags;//( SEMAPHORE )
    

    SignalType();
    virtual ~SignalType();
    void set(QString sh, FileBuffer* data);
private:

};

#endif	/* SIGNALTYPE_H */

