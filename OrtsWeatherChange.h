/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ORTSWEATHERCHANGE_H
#define	ORTSWEATHERCHANGE_H

#include <QString>
#include <QMap>

class FileBuffer;
class QTextStream;

class OrtsWeatherChange {
public:
    static QMap<QString, OrtsWeatherChange*> OrtsWeatherChanges;
    static void LoadList();
    
    OrtsWeatherChange();
    OrtsWeatherChange(const OrtsWeatherChange& o);
    virtual ~OrtsWeatherChange();
    
    QString name;
    float finalOvercastFactor;
    int overcastTransitionTime;
    float finalFogDistance;
    int fogTransitionTime;
    float finalPrecipitationIntensity;
    int precipitationIntensityTransitionTime;
    float finalPrecipitationLiquidity;
    int precipitationLiquidityTransitionTime;
    
    void load(FileBuffer* data);
    void save(QTextStream* out, QString off);
private:

};

#endif	/* ORTSWEATHERCHANGE_H */

