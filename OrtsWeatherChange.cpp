/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "OrtsWeatherChange.h"
#include <QTextStream>
#include <qDebug>
#include "FileBuffer.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "ReadFile.h"
#include <QFile>
#include "Game.h"

QMap<QString, OrtsWeatherChange*> OrtsWeatherChange::OrtsWeatherChanges;

OrtsWeatherChange::OrtsWeatherChange() {
}

OrtsWeatherChange::OrtsWeatherChange(const OrtsWeatherChange& o) {
    finalOvercastFactor = o.finalOvercastFactor;
    overcastTransitionTime = o.overcastTransitionTime;
    finalFogDistance = o.finalFogDistance;
    fogTransitionTime = o.fogTransitionTime;
    finalPrecipitationIntensity = o.finalPrecipitationIntensity;
    precipitationIntensityTransitionTime = o.precipitationIntensityTransitionTime;
    finalPrecipitationLiquidity = o.finalPrecipitationLiquidity;
    precipitationLiquidityTransitionTime = o.precipitationLiquidityTransitionTime;
    isOvercast = o.isOvercast;
    isFog = o.isFog;
    isIntensity = o.isIntensity;
    isLiquidity = o.isLiquidity;
}

OrtsWeatherChange::~OrtsWeatherChange() {
}

void OrtsWeatherChange::LoadList(){
    QString path = Game::root + "/routes/" + Game::route + "/weathertransitions.dat";
    path.replace("//", "/");
    qDebug() << path;
    
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    data->toUtf16();
    data->skipBOM();
    QString sh;
    
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh.startsWith("simisa@@@@@@@@@@")) {
            continue;
        }
        if(sh == "ortsweatherchangelist"){
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("ortsweatherchange")) {
                    OrtsWeatherChange* wh = new OrtsWeatherChange();
                    wh->load(data);
                    OrtsWeatherChanges[wh->name] = wh;
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#OrtsWeatherChangeList - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#WeatherTransitions.dat  - undefined token: " << sh;
        ParserX::SkipToken(data);
    }
}

void OrtsWeatherChange::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("name")) {
            name = ParserX::GetString(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ortsovercast")) {
            isOvercast = true;
            finalOvercastFactor = ParserX::GetNumber(data);
            overcastTransitionTime = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ortsfog")) {
            isFog = true;
            finalFogDistance = ParserX::GetNumber(data);
            fogTransitionTime = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ortsprecipitationintensity")) {
            isIntensity = true;
            finalPrecipitationIntensity = ParserX::GetNumber(data);
            precipitationIntensityTransitionTime = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ortsprecipitationliquidity")) {
            isLiquidity = true;
            finalPrecipitationLiquidity = ParserX::GetNumber(data);
            precipitationLiquidityTransitionTime = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        
        qDebug() << "#OrtsWeatherChange - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
}

void OrtsWeatherChange::save(QTextStream* out, QString off) {
    *out << off << "ORTSWeatherChange (\n";
    if(name.length() > 0)
        *out << off << "	Name ( " << ParserX::AddComIfReq(name) << " )\n";
    if(isOvercast)
    *out << off << "	ORTSOvercast ( " << finalOvercastFactor << " " << overcastTransitionTime << " )\n";
    if(isFog)
    *out << off << "	ORTSFog ( " << finalFogDistance << " " << fogTransitionTime << " )\n";
    if(isIntensity)
    *out << off << "	ORTSPrecipitationIntensity ( " << finalPrecipitationIntensity << " " << precipitationIntensityTransitionTime << " )\n";
    if(isLiquidity)
    *out << off << "	ORTSPrecipitationLiquidity ( " << finalPrecipitationLiquidity << " " << precipitationLiquidityTransitionTime << " )\n";
    *out << off << ")\n";

}
