/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRK_H
#define	TRK_H

#include <QString>
#include <unordered_map>

class Trk {
public:
    QString idName;
    QString displayName;
    QString description;
    QString graphic;
    QString loadingScreen;
    QString routeName;
    unsigned int electrified;
    unsigned int mountains;
    int overheadWireHeight;
    int passengerRuleSet;
    int freightRuleSet;
    int signalSet;
    int gantrySet;
    int trackGauge;
    int era;
    float speedLimit;
    std::unordered_map<std::string, QString> environment;
    int terrainErrorScale;
    int startTileX;
    int startTileZ;
    float startpX;
    float startpZ;
    float distantTerrainYOffset = 0;
    bool milepostUnitsKilometers;
    int maxLineVoltage;
    QString defaultSignalSMS;
    QString defaultCrossingSMS;
    QString defaultWaterTowerSMS;
    QString defaultCoalTowerSMS;
    QString defaultDieselTowerSMS;
    float tempRestrictedSpeed;
    float gravityScale;
    float timetableTollerance;
    float forestClearDistance = 0;
    float derailScale;
    int imageLoadId;
    int imageDetailsId;
    double *tsreProjection = NULL;
    bool isModified();
    void setModified(bool val);
    Trk();
    void load();
    void load(QString path);
    void save();
    virtual ~Trk();
    
private:
    bool modified = false;
};

#endif	/* TRK_H */

