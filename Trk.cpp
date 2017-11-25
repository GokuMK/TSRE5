/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Trk.h"
#include "Game.h"
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "ParserX.h"
#include "ReadFile.h"
#include "FileBuffer.h"
#include "TexLib.h"

Trk::Trk() {
    graphic = "graphic.ace";
    loadingScreen = "load.ace";
    electrified = 0;
    mountains = 0;
    overheadWireHeight = 0;
    passengerRuleSet = 0;
    freightRuleSet = 0;
    signalSet = 0;
    gantrySet = 0;
    trackGauge = 0;
    era = 0;
    speedLimit = 44.44444;
    terrainErrorScale = 1.0;
    startTileX = 0;
    startTileZ = 0;
    startpX = 0;
    startpZ = 0;
    milepostUnitsKilometers = true;
    maxLineVoltage = 0;
    defaultSignalSMS = "signal.sms";
    defaultCrossingSMS = "crossing.sms";
    defaultWaterTowerSMS = "wtower.sms";
    defaultCoalTowerSMS = "ctower.sms";
    defaultDieselTowerSMS = "dtower.sms";
    tempRestrictedSpeed = 0;
    gravityScale = -1;
    timetableTollerance = -1;
    derailScale = -1;
    environment.clear();
    environment["SpringClear"] = "sun.env";
    environment["SpringRain"] = "rain.env";
    environment["SpringSnow"] = "snow.env";
    environment["SummerClear"] = "sun.env";
    environment["SummerRain"] = "rain.env";
    environment["SummerSnow"] = "snow.env";
    environment["AutumnClear"] = "sun.env";
    environment["AutumnRain"] = "rain.env";
    environment["AutumnSnow"] = "snow.env";
    environment["WinterClear"] = "sun.env";
    environment["WinterRain"] = "rain.env";
    environment["WinterSnow"] = "snow.env";
    modified = true;
}

Trk::~Trk() {
}

void Trk::load(){
    QString path = Game::root + "/routes/" + Game::route + "/" + Game::trkName + ".trk";
    path.replace("//", "/");
    qDebug() << "# TRK file: " <<path;
    load(path);
}

void Trk::load(QString path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    data->toUtf16();
    ParserX::NextLine(data);
    
    this->milepostUnitsKilometers = false;
    QString sh = "";
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if(sh == "tr_routefile"){
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("routeid")) {
                    this->idName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("name")) {
                    this->displayName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("description")) {
                    this->description = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("graphic")) {
                    this->graphic = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("loadingscreen")) {
                    this->loadingScreen = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("filename")) {
                    this->routeName = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("electrified")) {
                    this->electrified = ParserX::GetHex(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("mountains")) {
                    this->mountains = ParserX::GetHex(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("overheadwireheight")) {
                    this->overheadWireHeight = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("passengerruleset")) {
                    this->passengerRuleSet = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("freightruleset")) {
                    this->freightRuleSet = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("signalset")) {
                    this->signalSet = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("gantryset")) {
                    this->gantrySet = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("trackgauge")) {
                    this->trackGauge = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("era")) {
                    this->era = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("speedlimit")) {
                    this->speedLimit = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("terrainerrorscale")) {
                    this->terrainErrorScale = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("routestart")) {
                    startTileX = ParserX::GetNumber(data);
                    startTileZ = ParserX::GetNumber(data);
                    //qDebug() << startTileX << startTileY;
                    //break;
                    startpX = ParserX::GetNumber(data);
                    startpZ = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("milepostunitskilometers")) {
                    this->milepostUnitsKilometers = true;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("milepostunitsmiles")) {
                    this->milepostUnitsKilometers = false;
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("maxlinevoltage")) {
                    this->maxLineVoltage = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("defaultsignalsms")) {
                    this->defaultSignalSMS = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("defaultcrossingsms")) {
                    this->defaultCrossingSMS = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("defaultwatertowersms")) {
                    this->defaultWaterTowerSMS = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("defaultcoaltowersms")) {
                    this->defaultCoalTowerSMS = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("defaultdieseltowersms")) {
                    this->defaultDieselTowerSMS = ParserX::GetString(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("temprestrictedspeed")) {
                    this->tempRestrictedSpeed = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("gravityscale")) {
                    this->gravityScale = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("environment")) {
                    this->environment.clear();
                    while (!((sh = ParserX::NextTokenInside(data)) == "")) {
                        this->environment[sh.toStdString()] = ParserX::GetString(data);
                        ParserX::SkipToken(data);
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("derailscale")) {
                    this->derailScale = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("timetabletollerance")) {
                    this->timetableTollerance = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                if (sh == ("ortsuserpreferenceforestcleardistance")) {
                    this->forestClearDistance = ParserX::GetNumber(data);
                    ParserX::SkipToken(data);
                    continue;
                }

                qDebug() << "#TRK tr_routefile - undefined token: " << sh;
                ParserX::SkipToken(data);
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << "#TRK - undefined token: " << sh;
        ParserX::SkipToken(data);
    }

    imageLoadId = TexLib::addTex(Game::root+"/routes/"+idName+"/load.ace");
    imageDetailsId = TexLib::addTex(Game::root+"/routes/"+idName+"/details.ace");
    modified = false;
}

bool Trk::isModified(){
    return modified;
}

void Trk::setModified(bool val){
    modified = val;
}

void Trk::save() {
    if (!Game::writeEnabled) return;
    if (!modified) return;
    QFile file;
    QTextStream out;
    QString filepath;

    filepath = Game::root + "/routes/" + Game::route + "/" + Game::trkName + ".trk";
    file.setFileName(filepath);
    //qDebug() << filepath;
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    out.setDevice(&file);
    out.setRealNumberPrecision(8);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);

    out << "SIMISA@@@@@@@@@@JINX0r1t______" << "\n\n";
    out << "Tr_RouteFile (" << "\n";
    out << "	RouteID ( " << ParserX::AddComIfReq(this->idName) << " )" << "\n";
    out << "	Name ( " << ParserX::AddComIfReq(this->displayName) << " )" << "\n";
    out << "	Description ( " << ParserX::SplitToMultiline(this->description, "		") <<" )" << "\n";
    out << "	Graphic ( " << ParserX::AddComIfReq(this->graphic) << " )" << "\n";
    out << "	LoadingScreen ( " << ParserX::AddComIfReq(this->loadingScreen) << " )" << "\n";
    out << "	FileName ( " << ParserX::AddComIfReq(this->routeName) << " )" << "\n";
    out << "	Electrified ( " << ParserX::MakeFlagsString(this->electrified) << " )" << "\n";
    out << "	Mountains ( " << ParserX::MakeFlagsString(this->electrified) << " )" << "\n";
    out << "	OverheadWireHeight ( " << this->overheadWireHeight << " )" << "\n";
    out << "	PassengerRuleSet ( " << this->passengerRuleSet << " )" << "\n";
    out << "	FreightRuleSet ( " << this->freightRuleSet << " )" << "\n";
    out << "	SignalSet ( " << this->signalSet << " )" << "\n";
    out << "	GantrySet ( " << this->gantrySet << " )" << "\n";
    out << "	TrackGauge ( " << this->trackGauge << " )" << "\n";
    out << "	Era ( " << this->era << " )" << "\n";
    out << "	SpeedLimit ( " << this->speedLimit << " )" << "\n";
    out << "	Environment (" << "\n";
    //for (auto it = this->environment.begin(); it != this->environment.end(); ++it)
    //    out << "		" << QString::fromStdString(it->first) << " ( " << it->second << " )" << "\n";
    out << "		SpringClear ( " << this->environment["SpringClear"] << " )" << "\n";
    out << "		SpringRain ( " << this->environment["SpringRain"] << " )" << "\n";
    out << "		SpringSnow ( " << this->environment["SpringSnow"] << " )" << "\n";
    out << "		SummerClear ( " << this->environment["SummerClear"] << " )" << "\n";
    out << "		SummerRain ( " << this->environment["SummerRain"] << " )" << "\n";
    out << "		SummerSnow ( " << this->environment["SummerSnow"] << " )" << "\n";
    out << "		AutumnClear ( " << this->environment["AutumnClear"] << " )" << "\n";
    out << "		AutumnRain ( " << this->environment["AutumnRain"] << " )" << "\n";
    out << "		AutumnSnow ( " << this->environment["AutumnSnow"] << " )" << "\n";
    out << "		WinterClear ( " << this->environment["WinterClear"] << " )" << "\n";
    out << "		WinterRain ( " << this->environment["WinterRain"] << " )" << "\n";
    out << "		WinterSnow ( " << this->environment["WinterSnow"] << " )" << "\n";
    out << "	)" << "\n";
    out << "	TerrainErrorScale ( " << this->terrainErrorScale << " )" << "\n";
    out << "	RouteStart ( "<< this->startTileX <<" "<< this->startTileZ <<" " << this->startpX << " " << this->startpZ << " )" << "\n";
    if(this->milepostUnitsKilometers)
        out << "	MilepostUnitsKilometers ( )" << "\n";
    else
        out << "	MilepostUnitsMiles ( )" << "\n";
    out << "	DefaultCrossingSMS ( " << ParserX::AddComIfReq(this->defaultCrossingSMS) << " )" << "\n";
    out << "	DefaultSignalSMS ( " << ParserX::AddComIfReq(this->defaultSignalSMS) << " )" << "\n";
    out << "	DefaultWaterTowerSMS ( " << ParserX::AddComIfReq(this->defaultWaterTowerSMS) << " )" << "\n";
    out << "	DefaultCoalTowerSMS ( " << ParserX::AddComIfReq(this->defaultCoalTowerSMS) << " )" << "\n";
    out << "	DefaultDieselTowerSMS ( " << ParserX::AddComIfReq(this->defaultDieselTowerSMS) << " )" << "\n";
    out << "	TempRestrictedSpeed ( " << this->tempRestrictedSpeed << " )" << "\n";
    if(this->gravityScale >= 0)
        out << "	GravityScale ( " << this->gravityScale << " )" << "\n";
    if(this->derailScale >= 0)
    out << "	DerailScale ( " << this->derailScale << " )" << "\n";
    if(this->timetableTollerance >= 0)
    out << "	TimetableTollerance ( " << this->timetableTollerance << " )" << "\n";
    if(this->forestClearDistance >= 0)
    out << "	ORTSUserPreferenceForestClearDistance ( " << this->forestClearDistance << " )" << "\n";
    out << ")" << "\n";

    out.flush();
    file.close();
    modified = false;
}