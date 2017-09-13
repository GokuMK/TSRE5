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
}

OrtsWeatherChange::~OrtsWeatherChange() {
}

void OrtsWeatherChange::load(FileBuffer* data) {
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("ortsovercast")) {
            finalOvercastFactor = ParserX::GetNumber(data);
            overcastTransitionTime = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ortsfog")) {
            finalFogDistance = ParserX::GetNumber(data);
            fogTransitionTime = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ortsprecipitationintensity")) {
            finalPrecipitationIntensity = ParserX::GetNumber(data);
            precipitationIntensityTransitionTime = ParserX::GetNumber(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("ortsprecipitationliquidity")) {
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
    *out << off << "	ORTSOvercast ( " << finalOvercastFactor << " " << overcastTransitionTime << " )\n";
    *out << off << "	ORTSFog ( " << finalFogDistance << " " << fogTransitionTime << " )\n";
    *out << off << "	ORTSPrecipitationIntensity ( " << finalPrecipitationIntensity << " " << precipitationIntensityTransitionTime << " )\n";
    *out << off << "	ORTSPrecipitationLiquidity ( " << finalPrecipitationLiquidity << " " << precipitationLiquidityTransitionTime << " )\n";
    *out << off << ")\n";

}
