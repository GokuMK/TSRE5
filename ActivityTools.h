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
    
signals:
    void enableTool(QString name);

private:
    Route *route = NULL;
    QComboBox consists;
    QComboBox actShow;
    QComboBox conFilesShow;
    QMap<QString, QPushButton*> buttonTools;
    
    QLineEdit eDisplayName;

    QLineEdit eDuration;
    QLineEdit eStartTime;
    
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
};

#endif	/* ACTIVITYTOOLS_H */

