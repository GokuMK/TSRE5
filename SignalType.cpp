/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SignalType.h"
#include "FileBuffer.h"
#include "ParserX.h"
#include <QDebug>

SignalType::SignalType() {
}

SignalType::~SignalType() {
}

void SignalType::set(QString sh, FileBuffer* data) {
        if (sh == "signalflags") {   
            return;
        }
        if (sh == "signalfntype") {   
            type = ParserX::GetString(data);
            return;
        }
        if (sh == "signallighttex") {   
            type = ParserX::GetString(data);
            return;
        }
        if (sh == "semaphoreinfo") {   
            ParserX::GetNumber(data);
            return;
        }
        if (sh == "signallights") {  
            ParserX::GetNumber(data);
            Light *newLight;
            
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "_info" || sh == "skip" || sh == "_skip") {
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "signallight") {
                    newLight = new Light();
                    newLight->id = ParserX::GetNumber(data);
                    newLight->defName = ParserX::GetString(data);

                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        newLight->set(sh, data);
                        ParserX::SkipToken(data);
                    }

                    lights[newLight->id] = newLight;
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#signallights - undefined token: "<<sh;
                ParserX::SkipToken(data);
                continue;
            }
            return;
        }
        if (sh == "signaldrawstates") {   
            ParserX::GetNumber(data);
            DrawState *newState;
            
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                //qDebug() << "- " << sh;
                if (sh == "") {
                    break;
                }
                if (sh == "_info" || sh == "skip" || sh == "_skip") {
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == "signaldrawstate") {
                    newState = new DrawState();
                    newState->id = ParserX::GetNumber(data);
                    newState->name = ParserX::GetString(data);

                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        newState->set(sh, data);
                        ParserX::SkipToken(data);
                    }

                    drawStates[newState->name] = newState;
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#signaldrawstates - undefined token: "<<sh;
                ParserX::SkipToken(data);
                continue;
            }
            return;
        }
        if (sh == "signalaspects") {   
            ParserX::GetNumber(data);
            
            return;
        }
        if (sh == "signalnumclearahead") {   
            return;
        }
        if (sh == "sigflashduration") {   
            return;
        }
        
    qDebug() << "#signaltype - undefined token: " << sh;
    return;
}

SignalType::Light::Light(){
    
}

void SignalType::Light::set(QString sh, FileBuffer* data){
    if (sh == "position") {   
        position[0] = ParserX::GetNumber(data);
        position[1] = ParserX::GetNumber(data);
        position[2] = ParserX::GetNumber(data);
        return;
    }
    if (sh == "radius") {   
        radius = ParserX::GetNumber(data);
        return;
    }
    if (sh == "signalflags") {   
        QString flags = ParserX::GetStringInside(data);
        return;
    }
    
    qDebug() << "-- " << sh;
    return;
}

SignalType::DrawState::DrawState(){
    
}

void SignalType::DrawState::set(QString sh, FileBuffer* data){
    if (sh == "drawlights") {   
        int count = ParserX::GetNumber(data);
        while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
            if (sh == "") {
                break;
            }
            if (sh == "_info" || sh == "skip" || sh == "_skip") {
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == "drawlight") {
                lightDrawList.push_back(ParserX::GetNumber(data));
                int mode = 0;
                if(ParserX::NextTokenInside(data).toLower() == "signalflags"){
                    QString optionalFlags = ParserX::GetStringInside(data);
                    if(optionalFlags == "FLASHING")
                        mode = 1;
                    ParserX::SkipToken(data);
                }
                lightModeList.push_back(mode);
                ParserX::SkipToken(data);
                continue;
            }
            qDebug() << "#drawlights - undefined token: "<<sh;
            ParserX::SkipToken(data);
            continue;
        }
        return;
    }
    if (sh == "semaphorepos") {   
        int semaphorepos = ParserX::GetNumber(data);
        return;
    }

    qDebug() << "-- " << sh;
    return;
}
