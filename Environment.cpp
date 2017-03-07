/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Environment.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include "Game.h"
#include <QDebug>
#include <QFile>
#include "GLUU.h"

Environment::Environment(QString path) {
    loaded = false;
    texturePath = Game::root + "/routes/" + Game::route + "/envfiles/textures/";
            
    qDebug() << "Wczytywanie pliku env: " << path;

    QString sh;
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    data->toUtf16();
    data->skipBOM();

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == "simisa@@@@@@@@@@jinx0w0t______") {
            continue;
        }
        if (sh == "include") {
            QString incPath = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            data->insertFile(path + "/" + incPath);
            continue;
        }
        if (sh == "world") {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == "include") {
                    QString incPath = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    data->insertFile(path + "/" + incPath);
                    continue;
                }
                if (sh == "world_water") {
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == "world_water_layers") {
                            waterCount = ParserX::GetNumber(data);
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

                                if (sh == "world_water_layer") {
                                    water.push_back(WaterLayer());
                                    loadWaterLayer(data);
                                    //traffic->service.emplace_back();
                                    //traffic->service.back().load(data);
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                ParserX::SkipToken(data);
                                continue;
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        qDebug() << "#world_water_layers - undefined token: " << sh;
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#world - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#ENV - undefined token: " << sh;
        ParserX::SkipToken(data);
    }
    delete data;
    loaded = 1;
qDebug() << water.size();
    for(int i = 0; i < water.size(); i++){
        qDebug() << water[i].height;
qDebug() << water[i].tex;
    }
    return;
}

void Environment::loadWaterLayer(FileBuffer* data){
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == "world_water_layer_height") {
            water.back().height = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "world_anim_shader") {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == "world_shader") {
                    ParserX::GetString(data);
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == "terrain_texslots") {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if (sh == "terrain_texslot") {
                                    water.back().tex = texturePath + ParserX::GetString(data).toLower();
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#terrain_texslots - undefined token: " << sh;
                                ParserX::SkipToken(data);
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == "terrain_uvcalc") {
                            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                                if (sh == "terrain_uvcalc") {
                                    ParserX::SkipToken(data);
                                    continue;
                                }
                                qDebug() << "#terrain_uvcalc - undefined token: " << sh;
                                ParserX::SkipToken(data);
                            }
                            ParserX::SkipToken(data);
                            continue;
                        }
                        qDebug() << "#world_anim_shader - undefined token: " << sh;
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#world_anim_shader - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#world_water_layer - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

Environment::~Environment() {
}

void Environment::renderWater(){
    
}
