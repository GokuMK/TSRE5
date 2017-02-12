/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYTOOLS_H
#define	ACTIVITYTOOLS_H

#include <QtWidgets>
#include <QString>

class Route;

class ActivityTools : public QWidget{
    Q_OBJECT
public:
    ActivityTools(QString name);
    virtual ~ActivityTools();

public slots:
    void loadActFiles();
    void activitySelected(QString n);
    void conFilesShowEnabled(QString);
    void routeLoaded(Route* r);
    void actNewLooseConsistToolEnabled(bool val);
    void newActButtonEnabled();
    void cServiceEnabled(QString val);
    void actServiceNewEnabled();
    void msg(QString text, QString val);
    void eFileNameEnabled(QString val);
    void eDisplayNameEnabled(QString val);
    void cDifficultyEnabled(int val);
    void eDurationEnabled();
    void eStartTimeEnabled();
    void cSeasonEnabled(int val);
    void cWeatherEnabled(int val);
    void eFuelCoalEnabled(QString val);
    void sFuelCoalEnabled();
    void eFuelDieselEnabled(QString val);
    void sFuelDieselEnabled();
    void eFuelWaterEnabled(QString val);
    void sFuelWaterEnabled();
    void eHazardAnimalEnabled(QString val);
    void sHazardAnimalEnabled();
    void eHazardPeopleEnabled(QString val);
    void sHazardPeopleEnabled();
    void descriptionOpenEnabled();
    void briefingOpenEnabled();
signals:
    void enableTool(QString name);

private:
    Route *route = NULL;
    QComboBox consists;
    QComboBox actShow;
    QComboBox conFilesShow;
    QMap<QString, QPushButton*> buttonTools;
    
    QLineEdit eDisplayName;
    QLineEdit eFileName;

    QTimeEdit eDuration;
    QTimeEdit eStartTime;
    
    QLineEdit* eHazardAnimal;
    QLineEdit* eHazardPeople;
    QLineEdit* eFuelCoal;
    QLineEdit* eFuelWater;
    QLineEdit* eFuelDiesel;
    QSlider sHazardAnimal;
    QSlider sHazardPeople;
    QSlider sFuelCoal;
    QSlider sFuelWater;
    QSlider sFuelDiesel;
    
    QComboBox cDifficulty;
    QComboBox cSeason;
    QComboBox cWeather;

    QComboBox cService;
    QComboBox cTraffic;
    QComboBox cPath;
    
    void reloadServicesList();
};

#endif	/* ACTIVITYTOOLS_H */

