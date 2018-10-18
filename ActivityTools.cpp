/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ActivityTools.h"
#include "Route.h"
#include "Game.h"
#include "ActLib.h"
#include "EngLib.h"
#include "ConLib.h"
#include "Eng.h"
#include "Consist.h"
#include "Activity.h"
#include "GuiFunct.h"
#include "Service.h"
#include "Traffic.h"
#include "Path.h"
#include "ActivityServiceProperties.h"
#include "TextEditDialog.h"
#include "GeoCoordinates.h"
#include "UnsavedDialog.h"
#include "Trk.h"

ActivityTools::ActivityTools(QString name)
    : QWidget(){
    setFixedWidth(250);
    cSeason.setStyleSheet("combobox-popup: 0;");
    cSeason.addItem("Spring",0);
    cSeason.addItem("Summer",1);
    cSeason.addItem("Autumn",2);
    cSeason.addItem("Winter",3);
    cWeather.setStyleSheet("combobox-popup: 0;");
    cWeather.addItem("Clear",0);
    cWeather.addItem("Rain",1);
    cWeather.addItem("Snow",2);
    cDifficulty.setStyleSheet("combobox-popup: 0;");
    cDifficulty.addItem("Easy",0);
    cDifficulty.addItem("Medium",1);
    cDifficulty.addItem("Hard",2);
    
    buttonTools["actNewLooseConsistTool"] = new QPushButton("Place Consist", this);
    buttonTools["actNewSpeedZoneTool"] = new QPushButton("New", this);

    QMapIterator<QString, QPushButton*> i(buttonTools);
    while (i.hasNext()) {
        i.next();
        i.value()->setCheckable(true);
    }
    QObject::connect(buttonTools["actNewLooseConsistTool"], SIGNAL(toggled(bool)), this, SLOT(actNewLooseConsistToolEnabled(bool)));
    
    //QPushButton *loadActFilesButton = new QPushButton("Load Activities", this);
    QPushButton *newActButton = new QPushButton("New Activity", this);
    //advancedPlacenentButton->setCheckable(true);
    QObject::connect(newActButton, SIGNAL(released()), this, SLOT(newActButtonEnabled()));

    //radio1->setChecked(true);
    QGridLayout *vlist1 = NULL;
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QLabel *label = new QLabel("Activity List:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&actShow);
    actShow.setStyleSheet("combobox-popup: 0;");
    actShow.setMaxVisibleItems(35);
    actShow.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vbox->addWidget(newActButton);
    
    label = new QLabel("Player:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cService);
    cService.setStyleSheet("combobox-popup: 0;");
    cService.setMaxVisibleItems(35);
    cService.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QPushButton *actServiceOpen = new QPushButton("Open Service Editor");
    QObject::connect(actServiceOpen, SIGNAL(released()), this, SLOT(actServiceOpenEnabled()));
    vbox->addWidget(actServiceOpen);
    
    label = new QLabel("Traffic:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cTraffic);
    cTraffic.setStyleSheet("combobox-popup: 0;");
    cTraffic.setMaxVisibleItems(35);
    cTraffic.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QPushButton *actTrafficOpen = new QPushButton("Open Traffic Editor");
    QObject::connect(actTrafficOpen, SIGNAL(released()), this, SLOT(actTrafficOpenEnabled()));
    vbox->addWidget(actTrafficOpen);
    
    label = new QLabel("Timetable:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *actTimetableOpen = new QPushButton("Open Timetable Editor");
    QObject::connect(actTimetableOpen, SIGNAL(released()), this, SLOT(actTimetableOpenEnabled()));
    vbox->addWidget(actTimetableOpen);
    
    label = new QLabel("Paths:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cPath);
    cPath.setStyleSheet("combobox-popup: 0;");
    cPath.setMaxVisibleItems(35);
    cPath.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actPathsNew = new QPushButton("New");
    QPushButton *actPathsEdit = new QPushButton("Edit");
    QObject::connect(actPathsEdit, SIGNAL(released()), this, SLOT(actPathsEditToolEnabled()));
    QPushButton *actPathsClone = new QPushButton("Clone");
    QPushButton *actPathsDelete = new QPushButton("Delete");
    vlist1->addWidget(actPathsNew,0,0);
    vlist1->addWidget(actPathsEdit,0,1);
    vlist1->addWidget(actPathsClone,0,2);
    vlist1->addWidget(actPathsDelete,0,3);
    QPushButton *actPathsRefreshList = new QPushButton("Refresh List");
    QObject::connect(actPathsRefreshList, SIGNAL(released()), this, SLOT(actPathsRefreshListSelected()));
    vlist1->addWidget(actPathsRefreshList,1,1,1,3);
    vbox->addItem(vlist1);    
   
    label = new QLabel("Activity Objects List:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&consists);
    consists.setStyleSheet("combobox-popup: 0;");
    consists.setMaxVisibleItems(35);
    consists.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actConsistJump = new QPushButton("Jump To");
    QObject::connect(actConsistJump, SIGNAL(released()), this, SLOT(actConsistJumpEnabled()));
    QPushButton *actConsistDelete = new QPushButton("Delete");
    QObject::connect(actConsistDelete, SIGNAL(released()), this, SLOT(actConsistDeleteEnabled()));
    vlist1->addWidget(actConsistJump,0,0);
    vlist1->addWidget(actConsistDelete,0,1);
    vlist1->addWidget(buttonTools["actNewLooseConsistTool"],0,2,1,2);
    label = new QLabel("Consist:");
    //label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vlist1->addWidget(label,1,0);
    vlist1->addWidget(&conFilesShow,1,1,1,3);
    conFilesShow.setStyleSheet("combobox-popup: 0;");
    conFilesShow.setMaxVisibleItems(35);
    conFilesShow.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    QPushButton *conFilesRefresh = new QPushButton("Refresh List");
    QObject::connect(conFilesRefresh, SIGNAL(released()), this, SLOT(conFilesRefreshSelected()));
    vlist1->addWidget(conFilesRefresh,2,1,1,3);
    vbox->addItem(vlist1);
    
    label = new QLabel("Restricted Speed Zones List:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&speedZones);
    speedZones.setStyleSheet("combobox-popup: 0;");
    speedZones.setMaxVisibleItems(35);
    speedZones.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actZoneJump = new QPushButton("Jump To");
    QObject::connect(actZoneJump, SIGNAL(released()), this, SLOT(actReducedSpeedZonesEnabled()));
    QObject::connect(buttonTools["actNewSpeedZoneTool"], SIGNAL(toggled(bool)), this, SLOT(actZoneNewToolEnabled(bool)));
    QPushButton *actZoneDelete = new QPushButton("Delete");
    QObject::connect(actZoneDelete, SIGNAL(released()), this, SLOT(actZoneDeleteEnabled()));
    QPushButton *actZoneDeleteAll = new QPushButton("Delete All");
    QObject::connect(actZoneDeleteAll, SIGNAL(released()), this, SLOT(actZoneDeleteAllEnabled()));
    vlist1->addWidget(actZoneJump,0,0);
    vlist1->addWidget(buttonTools["actNewSpeedZoneTool"],0,1);
    vlist1->addWidget(actZoneDelete,0,2);
    vlist1->addWidget(actZoneDeleteAll,0,3);
    vbox->addItem(vlist1);
    
    label = new QLabel("Failed Signals List:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&failedSignals);
    failedSignals.setStyleSheet("combobox-popup: 0;");
    failedSignals.setMaxVisibleItems(35);
    failedSignals.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actFailedSignalJump = new QPushButton("Jump To");
    QObject::connect(actFailedSignalJump, SIGNAL(released()), this, SLOT(actFailedSignalsJumpEnabled()));
    QPushButton *actFailedSignalTool = new QPushButton("Disable");
    QObject::connect(actFailedSignalTool, SIGNAL(released()), this, SLOT(actFailedSignalNewToolEnabled()));
    QPushButton *actFailedSignalDelete = new QPushButton("Delete");
    QObject::connect(actFailedSignalDelete, SIGNAL(released()), this, SLOT(actFailedSignalDeleteEnabled()));
    QPushButton *actFailedSignalDeleteAll = new QPushButton("Delete All");
    QObject::connect(actFailedSignalDeleteAll, SIGNAL(released()), this, SLOT(actFailedSignalDeleteAllEnabled()));
    vlist1->addWidget(actFailedSignalJump,0,0);
    vlist1->addWidget(actFailedSignalTool,0,1);
    vlist1->addWidget(actFailedSignalDelete,0,2);
    vlist1->addWidget(actFailedSignalDeleteAll,0,3);
    vbox->addItem(vlist1);
    
    label = new QLabel("Events:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *actEventsOpen = new QPushButton("Open Event Editor");
    QObject::connect(actEventsOpen, SIGNAL(released()), this, SLOT(actEventsOpenEnabled()));
    vbox->addWidget(actEventsOpen);
    
    label = new QLabel("Activity Info:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *actSettingsOpen = new QPushButton("Open Settings ...");
    QObject::connect(actSettingsOpen, SIGNAL(released()), this, SLOT(actSettingsOpenEnabled()));
    vbox->addWidget(actSettingsOpen);
    
    label = new QLabel("Experimental:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *actPlayButton = new QPushButton("Play: Don't use!");
    QObject::connect(actPlayButton, SIGNAL(released()), this, SLOT(actPlayEnabled()));
    vbox->addWidget(actPlayButton);
    
    label = new QLabel("General:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *actSaveButton = new QPushButton("Save Activities");
    QObject::connect(actSaveButton, SIGNAL(released()), this, SLOT(actSaveEnabled()));
    vbox->addWidget(actSaveButton);
    
    QHBoxLayout *vbox1 = new QHBoxLayout;
    int row = 0;
    int labelWidth = 70;
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(0,0,1,0);
    vlist->addWidget(new QLabel("File Name:"), row, 0);
    vlist->addWidget(&eFileName, row++, 1, 1, 2);
    QObject::connect(&eFileName, SIGNAL(textEdited(QString)), this, SLOT(eFileNameEnabled(QString)));
    vlist->addWidget(new QLabel("Display Name:"), row, 0);
    vlist->addWidget(&eDisplayName, row++, 1, 1, 2);
    QObject::connect(&eDisplayName, SIGNAL(textEdited(QString)), this, SLOT(eDisplayNameEnabled(QString)));
    vlist->addWidget(new QLabel("Difficulty:"), row, 0);
    vlist->addWidget(&cDifficulty, row++, 1, 1, 2);
    QObject::connect(&cDifficulty, SIGNAL(activated(int)), this, SLOT(cDifficultyEnabled(int)));
    vlist->addWidget(new QLabel("Duration:"), row, 0);
    vlist->addWidget(&eDuration, row++, 1, 1, 2);
    QObject::connect(&eDuration, SIGNAL(editingFinished()), this, SLOT(eDurationEnabled()));
    eDuration.setDisplayFormat("HH:mm");
    vlist->addWidget(new QLabel("Start Time:"), row, 0);
    vlist->addWidget(&eStartTime, row++, 1, 1, 2);
    QObject::connect(&eStartTime, SIGNAL(editingFinished()), this, SLOT(eStartTimeEnabled()));
    eStartTime.setDisplayFormat("HH:mm:ss");
    vlist->addWidget(new QLabel("Season:"), row, 0);
    vlist->addWidget(&cSeason, row++, 1, 1, 2);
    QObject::connect(&cSeason, SIGNAL(activated(int)), this, SLOT(cSeasonEnabled(int)));
    vlist->addWidget(new QLabel("Weather:"), row, 0);
    vlist->addWidget(&cWeather, row++, 1, 1, 2);
    QObject::connect(&cWeather, SIGNAL(activated(int)), this, SLOT(cWeatherEnabled(int)));
    vlist->addWidget(new QLabel("Horn At Crossings: "), row, 0);
    cHornAtCrossings.setMinimumHeight(25);
    vlist->addWidget(&cHornAtCrossings, row++, 1, 1, 2);
    QObject::connect(&cHornAtCrossings, SIGNAL(stateChanged(int)), this, SLOT(cHornAtCrossingsEnabled(int)));
    
    eFuelCoal = GuiFunct::newQLineEdit(25,3);
    eFuelDiesel = GuiFunct::newQLineEdit(25,3);
    eFuelWater = GuiFunct::newQLineEdit(25,3);
    eHazardAnimal = GuiFunct::newQLineEdit(25,3);  
    eHazardPeople = GuiFunct::newQLineEdit(25,3);  
            
    vlist->addWidget(new QLabel("Fuel Coal:"), row, 0);
    vlist->addWidget(eFuelCoal, row, 1);
    vlist->addWidget(&sFuelCoal, row++, 2);
    QObject::connect(eFuelCoal, SIGNAL(textEdited(QString)), this, SLOT(eFuelCoalEnabled(QString)));
    QObject::connect(&sFuelCoal, SIGNAL(sliderReleased()), this, SLOT(sFuelCoalEnabled()));
    sFuelCoal.setRange(0, 100);
    sFuelCoal.setOrientation(Qt::Horizontal);
    vlist->addWidget(new QLabel("Fuel Diesel:"), row, 0);
    vlist->addWidget(eFuelDiesel, row, 1);
    vlist->addWidget(&sFuelDiesel, row++, 2);
    QObject::connect(eFuelDiesel, SIGNAL(textEdited(QString)), this, SLOT(eFuelDieselEnabled(QString)));
    QObject::connect(&sFuelDiesel, SIGNAL(sliderReleased()), this, SLOT(sFuelDieselEnabled()));
    sFuelDiesel.setRange(0, 100);
    sFuelDiesel.setOrientation(Qt::Horizontal);
    vlist->addWidget(new QLabel("Fuel Water:"), row, 0);
    vlist->addWidget(eFuelWater, row, 1);
    vlist->addWidget(&sFuelWater, row++, 2);
    QObject::connect(eFuelWater, SIGNAL(textEdited(QString)), this, SLOT(eFuelWaterEnabled(QString)));
    QObject::connect(&sFuelWater, SIGNAL(sliderReleased()), this, SLOT(sFuelWaterEnabled()));
    sFuelWater.setRange(0, 100);
    sFuelWater.setOrientation(Qt::Horizontal);
    vlist->addWidget(new QLabel("Hazard Animal:"), row, 0);
    vlist->addWidget(eHazardAnimal, row, 1);
    vlist->addWidget(&sHazardAnimal, row++, 2);
    QObject::connect(eHazardAnimal, SIGNAL(textEdited(QString)), this, SLOT(eHazardAnimalEnabled(QString)));
    QObject::connect(&sHazardAnimal, SIGNAL(sliderReleased()), this, SLOT(sHazardAnimalEnabled()));
    sHazardAnimal.setRange(0, 100);
    sHazardAnimal.setOrientation(Qt::Horizontal);
    vlist->addWidget(new QLabel("Hazard People:"), row, 0);
    vlist->addWidget(eHazardPeople, row, 1);
    vlist->addWidget(&sHazardPeople, row++, 2);
    QObject::connect(eHazardPeople, SIGNAL(textEdited(QString)), this, SLOT(eHazardPeopleEnabled(QString)));
    QObject::connect(&sHazardPeople, SIGNAL(sliderReleased()), this, SLOT(sHazardPeopleEnabled()));
    sHazardPeople.setRange(0, 100);
    sHazardPeople.setOrientation(Qt::Horizontal);
    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->setContentsMargins(0,0,0,0);
    vbox2->addItem(vlist);
    vbox2->addStretch(1);
    vbox1->addItem(vbox2);

    vbox2 = new QVBoxLayout;
    vbox2->setContentsMargins(0,0,0,0);
    vbox2->addWidget(new QLabel("Description:"));
    vbox2->addWidget(&eDescription);
    QObject::connect(&eDescription, SIGNAL(textChanged()), this, SLOT(eDescriptionEnabled()));
    eDescription.setMinimumWidth(350);
    vbox2->addWidget(new QLabel("Briefing:"));
    vbox2->addWidget(&eBriefing);
    QObject::connect(&eBriefing, SIGNAL(textChanged()), this, SLOT(eBriefingEnabled()));
    eBriefing.setMinimumWidth(350);
    vbox1->addItem(vbox2);
    settingsWidget.setParent(this);
    settingsWidget.setLayout(vbox1);
    settingsWidget.setWindowFlags(Qt::WindowType::Tool);
    settingsWidget.setWindowTitle("Activity Settings.");
    settingsWidget.setMinimumWidth(650);
    
    vbox->addStretch(1);
    this->setLayout(vbox);    
    
    //QObject::connect(&consists, SIGNAL(itemClicked(QListWidgetItem*)),
    //                  this, SLOT(itemsSelected(QListWidgetItem*)));
    QObject::connect(&actShow, SIGNAL(activated(QString)),
                      this, SLOT(activitySelected(QString)));
    QObject::connect(&conFilesShow, SIGNAL(activated(QString)),
                      this, SLOT(conFilesShowEnabled(QString)));
    QObject::connect(&cService, SIGNAL(activated(QString)),
                      this, SLOT(cServiceEnabled(QString)));
    QObject::connect(&cTraffic, SIGNAL(activated(QString)),
                      this, SLOT(cTrafficEnabled(QString)));
    //QObject::connect(loadActFilesButton, SIGNAL(released()),
    //                  this, SLOT(loadActFiles()));
}

void ActivityTools::actSettingsOpenEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    settingsWidget.show();
}

void ActivityTools::actEventsOpenEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    emit showActivityEventEditor();
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    emit showEvents(a);
}

void ActivityTools::actServiceOpenEnabled(){
    emit showActivityServiceEditor();
    emit showServices(route);
}

void ActivityTools::actTimetableOpenEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    emit showActivityTimetableEditor();
    emit showTimetable(ActLib::Act[actShow.currentData().toInt()]);
}

void ActivityTools::actTrafficOpenEnabled(){
    emit showActivityTrafficEditor();
    emit showTraffic(route);
}

void ActivityTools::routeLoaded(Route* r){
    route = r;

    consists.clear();
    ConLib::loadSimpleList(Game::root);
    foreach(QString name, ConLib::conFileList){
        conFilesShow.addItem(name.section('/', -1), QVariant(name));
    }
    
    reloadActivityList();
    reloadServicesList();
    reloadTrafficsList();
    reloadPathsList();
}

void ActivityTools::conFilesRefreshSelected(){
    conFilesShow.clear();
    ConLib::loadSimpleList(Game::root, true);
    foreach(QString name, ConLib::conFileList){
        conFilesShow.addItem(name.section('/', -1), QVariant(name));
    }
}

void ActivityTools::actPathsRefreshListSelected(){
    if(route == NULL)
        return;
    
    route->loadPaths();
    
    reloadPathsList();
}

void ActivityTools::reloadActivityList(){
    actShow.clear();
    foreach(int id, route->activityId){
        actShow.addItem(ActLib::Act[id]->header->name, QVariant(id));
    }
    actShow.setCurrentIndex(-1);
}

void ActivityTools::reloadServicesList(){
    int idx = cService.currentIndex();
    cService.clear();
    cService.addItem("UNDEFINED", QVariant(-1));
    for(int i = 0; i < ActLib::jestservice; i++ ){
        if(ActLib::Services[i] == NULL)
            continue;
        cService.addItem(ActLib::Services[i]->displayName, QVariant(i));
    }
    
    if(idx >= 0)
        cService.setCurrentIndex(idx);
}

void ActivityTools::reloadTrafficsList(){
    int idx = cTraffic.currentIndex();
    cTraffic.clear();
    cTraffic.addItem("UNDEFINED", QVariant(-1));
    for(int i = 0; i < ActLib::jesttraffic; i++ ){
        if(ActLib::Traffics[i] == NULL)
            continue;
        cTraffic.addItem(ActLib::Traffics[i]->nameId, QVariant(i));
    }
    
    if(idx >= 0)
        cTraffic.setCurrentIndex(idx);
}

void ActivityTools::reloadPathsList(){
    int idx = cPath.currentIndex();
    cPath.clear();
    
    for(int i = 0; i < route->path.size(); i++ )
        cPath.addItem(route->path[i]->displayName, QVariant(i));
    
    if(idx >= 0)
        cPath.setCurrentIndex(idx);
}
void ActivityTools::conFilesShowEnabled(QString val){
    if(actShow.currentIndex() < 0)
        return;
    QString file = conFilesShow.currentData().toString();
    qDebug() << file;
    int id = actShow.currentData().toInt();
    if(ActLib::Act[id] == NULL)
        return;
    ActLib::Act[id]->editorConListSelected = file;
}

void ActivityTools::actPlayEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int id = actShow.currentData().toInt();
    qDebug() << "id" << id;
    Activity *a = ActLib::Act[id];
    
    if(a == NULL)
        return;
    
    a->initToPlay();

}

void ActivityTools::actSaveEnabled(){
    QVector<QString> unsavedItems;
    ActLib::GetUnsavedInfo(unsavedItems);
    if(unsavedItems.size() == 0){
        qDebug() << "nic do zapisania";
        return;
    }
    
    UnsavedDialog unsavedDialog("SC");
    unsavedDialog.setWindowTitle("Save changes?");
    unsavedDialog.setMsg("Save changes in activities?");
    for(int i = 0; i < unsavedItems.size(); i++){
        unsavedDialog.items.addItem(unsavedItems[i]);
    }
    unsavedDialog.exec();
    if(unsavedDialog.changed == 1){
        ActLib::SaveAll();
    }
    return;
}

void ActivityTools::activitySelected(QString n){
    if(actShow.currentIndex() < 0)
        return;
    int id = actShow.currentData().toInt();
    qDebug() << "id" << id;
    Consist *e;
    Activity *a = ActLib::Act[id];
    
    if(ActLib::Act[id] == NULL)
        return;

    eFileName.setText(a->nameid);
    if(a->isNew())
        eFileName.setEnabled(true);
    else
        eFileName.setEnabled(false);
    eDisplayName.setText(a->header->name);
    eDuration.setTime(QTime(a->header->duration[0], a->header->duration[1]));
    eStartTime.setTime(QTime(a->header->startTime[0], a->header->startTime[1], a->header->startTime[2]));

    eHazardAnimal->setText(QString::number(a->header->animals));
    eHazardPeople->setText(QString::number(a->header->workers));
    eFuelCoal->setText(QString::number(a->header->fuelCoal));
    eFuelWater->setText(QString::number(a->header->fuelWater));
    eFuelDiesel->setText(QString::number(a->header->fuelDiesel));
    sHazardAnimal.setValue(a->header->animals);
    sHazardPeople.setValue(a->header->workers);
    sFuelCoal.setValue(a->header->fuelCoal);
    sFuelWater.setValue(a->header->fuelWater);
    sFuelDiesel.setValue(a->header->fuelDiesel);
    
    cDifficulty.setCurrentIndex(a->header->difficulty);
    cSeason.setCurrentIndex(a->header->season);
    cWeather.setCurrentIndex(a->header->weather);
    
    cHornAtCrossings.blockSignals(true);
    if(a->isOrtsHornAtCrossigns())
        cHornAtCrossings.setChecked(true);
    else
        cHornAtCrossings.setChecked(false);
    cHornAtCrossings.blockSignals(false);
   
    if(a->playerServiceDefinition == NULL){
        cService.setCurrentIndex(0);
    } else {
        cService.setCurrentIndex(0);
        QString cname = a->playerServiceDefinition->name;
        for(int i = 0; i < cService.count() ; i++ ){
            int id = cService.itemData(i).toInt();
            if(id < 0)
                continue;
            if(ActLib::Services[id] == NULL)
                continue;
            if(cname == ActLib::Services[id]->nameId){
                cService.setCurrentIndex(i);
                break;
            }
        }
    }
    
    if(a->traffic == NULL){
        cTraffic.setCurrentIndex(0);
    } else {
        cTraffic.setCurrentIndex(0);
        QString cname = a->traffic->name;
        for(int i = 0; i < cTraffic.count() ; i++ ){
            int id = cTraffic.itemData(i).toInt();
            if(id < 0)
                continue;
            if(ActLib::Traffics[id] == NULL)
                continue;
            //qDebug() << cname << route->service[id]->name;
            if(cname == ActLib::Traffics[id]->nameId){
                cTraffic.setCurrentIndex(i);
                break;
            }
        }
    }

    reloadActivityObjectLists();
    
    if(route != NULL){
        //if(!ActLib::act[id]->isInitActivityObjects)
        //    ActLib::act[id]->initActivityObjects();
        route->activitySelected(ActLib::Act[id]);
    }
    
    eBriefing.blockSignals(true);
    eDescription.blockSignals(true);
    QString txt = a->header->briefing;
    txt.replace("\\n","\n");
    eBriefing.setPlainText(txt);
    txt = a->header->description;
    txt.replace("\\n","\n");
    eDescription.setPlainText(txt);
    eBriefing.blockSignals(false);
    eDescription.blockSignals(false);
}

void ActivityTools::reloadActivityObjectLists(){
    if(actShow.currentIndex() < 0)
        return;
    int id = actShow.currentData().toInt();
    if(ActLib::Act[id] == NULL)
        return;
    
    Consist *e;
    consists.clear();
    for (int i = 0; i < ActLib::Act[id]->activityObjects.size(); i++){
        e = ActLib::Act[id]->activityObjects[i]->con;
        if(e == NULL) continue;
        consists.addItem(e->showName, QVariant(i));
    }
        
    speedZones.clear();
    ActivityObject *rz;
    for (int i = 0; i < ActLib::Act[id]->restrictedSpeedZone.size(); i++){
        rz = ActLib::Act[id]->restrictedSpeedZone[i];
        if(rz == NULL) continue;
        speedZones.addItem(rz->getSpeedZoneName(), QVariant(i));
    }

    failedSignals.clear();
    for (int i = 0; i < ActLib::Act[id]->activityFailedSignal.size(); i++){
        rz = ActLib::Act[id]->activityFailedSignal[i];
        if(rz == NULL) continue;
        failedSignals.addItem(QString("Signal ID: ")+QString::number(rz->getFailedSignalId()), QVariant(i));
    }
}

void ActivityTools::loadActFiles(){
    if(route == NULL)
        return;
    
}

void ActivityTools::actNewLooseConsistToolEnabled(bool val){
    conFilesShowEnabled("");
    if(val){
        emit enableTool("actNewLooseConsistTool");
    } else {
        emit enableTool("");
    }
}

void ActivityTools::actPathsEditToolEnabled(){
    int currentPathId = cPath.currentData().toInt();
    for(int i = 0; i < route->path.size(); i++){
        route->path[i]->unselect();
    }
    qDebug() << "aa";
    //route->path[currentPathId]->select();
    emit objectSelected(route->path[currentPathId]);
}


void ActivityTools::msg(QString text){
    if(text == "refreshActivityTools"){
        reloadActivityObjectLists();
    }
}
    
void ActivityTools::msg(QString text, QString val){
    if(text == "toolEnabled"){
        QMapIterator<QString, QPushButton*> i(buttonTools);
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(true);
            i.value()->setChecked(false);
        }
        if(buttonTools[val] != NULL)
            buttonTools[val]->setChecked(true);
        i.toFront();
        while (i.hasNext()) {
            i.next();
            if(i.value() == NULL)
                continue;
            i.value()->blockSignals(false);
        }
    }
}

void ActivityTools::cServiceEnabled(QString val){
    if(actShow.currentIndex() < 0)
        return;
    int id = cService.currentData().toInt();
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    
    if(id == -1){
        a->playerServiceDefinition = NULL;
    } else {
        int startTime = a->header->startTime[0]*60*60 + a->header->startTime[1]*60;
        a->createNewPlayerService(ActLib::Services[id]->nameId, startTime);
    }
}

void ActivityTools::cTrafficEnabled(QString val){
    if(actShow.currentIndex() < 0)
        return;
    int id = cTraffic.currentData().toInt();
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    
    if(id == -1){
        a->traffic = NULL;
    } else {
        a->createNewTrafficService(ActLib::Traffics[id]);
    }
}

void ActivityTools::newActButtonEnabled(){
    QString pathid = Game::root + "/routes/" + Game::route + "/activities/";
    //ActLib::Act[ActLib::jestact] = new Activity(pathid, "file.act", true);
    for(int i = 0; i < 1000; i++){
        QString name = QString("file")+QString::number(i)+QString(".act");
        if(ActLib::GetAct(pathid, name) > 0)
            continue;
        int id = ActLib::AddAct(pathid, name, true);
        ActLib::Act[id]->init(route->getTrk()->idName, "New Activity"+QString::number(i));
        qDebug()<< ActLib::Act[id]->header->name;
        route->activityId.push_back(id);
        //actShow.addItem(ActLib::Act[id]->header->name, id);
        //actShow.setCurrentIndex(actShow.count()-1); 
        reloadActivityList();
        actShow.setCurrentIndex(actShow.findData(id));
        activitySelected("");
        return;
    }
}
void ActivityTools::actConsistDeleteEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    int i = consists.currentIndex();
    if(i >= a->activityObjects.size())
        return;
    if(i < 0)
        return;
    
    a->activityObjects[i]->remove();
    reloadActivityObjectLists();
    emit sendMsg("unselect");
}

void ActivityTools::actConsistJumpEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    int i = consists.currentIndex();
    if(i >= a->activityObjects.size())
        return;
    Consist *e = a->activityObjects[i]->con;
    if(e == NULL) 
        return;
    
    float posTW[5];
    bool ok = a->activityObjects[i]->getElementPosition(0, posTW);
    if(!ok)
        return;
    
    if(coordinate == NULL)
        coordinate = new PreciseTileCoordinate();
    
    coordinate->TileX = posTW[0];
    coordinate->TileZ = posTW[1];
    coordinate->setWxyz(posTW[2], 0, posTW[4]);
    
    emit jumpTo(coordinate);
}

void ActivityTools::actFailedSignalsJumpEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    int i = failedSignals.currentIndex();
    if(i >= a->activityFailedSignal.size())
        return;
    float posTW[5];
    bool ok = a->activityFailedSignal[i]->getElementPosition(0, posTW);
    if(!ok)
        return;
    if(coordinate == NULL)
        coordinate = new PreciseTileCoordinate();
    
    coordinate->TileX = posTW[0];
    coordinate->TileZ = posTW[1];
    coordinate->setWxyz(posTW[2], 0, posTW[4]);
    
    emit jumpTo(coordinate);
}

void ActivityTools::actFailedSignalNewToolEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    bool ok = a->newFailedSignalFromSelected();
    if(!ok){
        QMessageBox msgBox;
        msgBox.setWindowTitle("Signal not disabled!");
        msgBox.setText("Select signal before using this button.");
        msgBox.exec();
        return;
    }
    reloadActivityObjectLists();
}

void ActivityTools::actFailedSignalDeleteEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;

    int i = failedSignals.currentIndex();
        qDebug() << i << a->activityFailedSignal.size();
    if(i >= a->activityFailedSignal.size())
        return;
    if(i < 0)
        return;
    a->activityFailedSignal[i]->remove();
    reloadActivityObjectLists();
    emit sendMsg("unselect");
}

void ActivityTools::actFailedSignalDeleteAllEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    
    a->removeAllFailedSignals();
    reloadActivityObjectLists();
    emit sendMsg("unselect");
}

void ActivityTools::actReducedSpeedZonesEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    int i = speedZones.currentIndex();
    if(i >= a->restrictedSpeedZone.size())
        return;
    float posTW[5];
    bool ok = a->restrictedSpeedZone[i]->getElementPosition(0, posTW);
    if(!ok)
        return;
    if(coordinate == NULL)
        coordinate = new PreciseTileCoordinate();
    
    coordinate->TileX = posTW[0];
    coordinate->TileZ = posTW[1];
    coordinate->setWxyz(posTW[2], 0, posTW[4]);
    
    emit jumpTo(coordinate);
}

void ActivityTools::actZoneNewToolEnabled(bool val){
    if(val){
        emit enableTool("actNewSpeedZoneTool");
    } else {
        emit enableTool("");
    }
}

void ActivityTools::actZoneDeleteEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    
    int i = speedZones.currentIndex();
    if(i >= a->restrictedSpeedZone.size())
        return;
    if(i < 0)
        return;
    a->restrictedSpeedZone[i]->remove();
    
    reloadActivityObjectLists();
    emit sendMsg("unselect");
}

void ActivityTools::actZoneDeleteAllEnabled(){
    if(actShow.currentIndex() < 0)
        return;
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::Act[aid];
    if(a == NULL)
        return;
    
    a->removeAllSpeedZones();
    reloadActivityObjectLists();
    emit sendMsg("unselect");
}

/*void ActivityTools::actServiceNewEnabled(){
    ActivityServiceTools sTools;
    QString pathid = Game::root + "/routes/" + Game::route + "/services/";
    QString name = "aaaaa1";
    Service *s = new Service(pathid, name+".srv", true);
    sTools.setData(s, route->path);
    sTools.exec();
    if(sTools.changed) {
        route->service.push_back(s);
        reloadServicesList();
    }
}*/

void ActivityTools::eFileNameEnabled(QString val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setFileName(val);
}

void ActivityTools::eDisplayNameEnabled(QString val){
    int id = actShow.currentData().toInt();
    Activity *a = ActLib::Act[id];
    if(a == NULL)
        return;
    a->setDisplayName(val);
    
    reloadActivityList();
    actShow.setCurrentIndex(actShow.findData(id));
}

void ActivityTools::cDifficultyEnabled(int val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setDifficulty(val);
}

void ActivityTools::eDurationEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setDuration(eDuration.time().hour(),  eDuration.time().minute());
}

void ActivityTools::eStartTimeEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setStartTime(eStartTime.time().hour(), eStartTime.time().minute(), eStartTime.time().second());
}

void ActivityTools::cSeasonEnabled(int val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setSeason(val);
}

void ActivityTools::cWeatherEnabled(int val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setWeather(val);
}

void ActivityTools::cHornAtCrossingsEnabled(int val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    if(val == Qt::Checked)
        a->setOrtsHornAtCrossigns(true);
    else
        a->setOrtsHornAtCrossigns(false);
}

void ActivityTools::eFuelCoalEnabled(QString val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    bool ok = false;
    int ival = val.toInt(&ok);
    if(!ok)
        return;
    sFuelCoal.setValue(ival);
    a->setFuelCoal(ival);
}

void ActivityTools::sFuelCoalEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eFuelCoal->setText(QString::number(sFuelCoal.value()));
    a->setFuelCoal(sFuelCoal.value());
}

void ActivityTools::eFuelDieselEnabled(QString val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    bool ok = false;
    int ival = val.toInt(&ok);
    if(!ok)
        return;
    sFuelDiesel.setValue(ival);
    a->setFuelDiesel(ival);
}

void ActivityTools::sFuelDieselEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eFuelDiesel->setText(QString::number(sFuelDiesel.value()));
    a->setFuelDiesel(sFuelDiesel.value());
}

void ActivityTools::eFuelWaterEnabled(QString val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    bool ok = false;
    int ival = val.toInt(&ok);
    if(!ok)
        return;
    sFuelWater.setValue(ival);
    a->setFuelWater(ival);
}

void ActivityTools::sFuelWaterEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eFuelWater->setText(QString::number(sFuelWater.value()));
    a->setFuelWater(sFuelWater.value());
}

void ActivityTools::eHazardAnimalEnabled(QString val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    bool ok = false;
    int ival = val.toInt(&ok);
    if(!ok)
        return;
    sHazardAnimal.setValue(ival);
    a->setHazardAnimals(ival);
}

void ActivityTools::sHazardAnimalEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eHazardAnimal->setText(QString::number(sHazardAnimal.value()));
    a->setHazardAnimals(sHazardAnimal.value());
}

void ActivityTools::eHazardPeopleEnabled(QString val){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    bool ok = false;
    int ival = val.toInt(&ok);
    if(!ok)
        return;
    sHazardPeople.setValue(ival);
    a->setHazardWorkers(ival);
}

void ActivityTools::sHazardPeopleEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eHazardPeople->setText(QString::number(sHazardPeople.value()));
    a->setHazardWorkers(sHazardPeople.value());
}

void ActivityTools::eDescriptionEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    QString txt = eDescription.toPlainText();
    txt.replace("\n","\\n");
    a->setDescription(txt);
}

void ActivityTools::eBriefingEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    QString txt = eBriefing.toPlainText();
    txt.replace("\n","\\n");
    a->setBriefing(txt);
}

void ActivityTools::descriptionOpenEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    TextEditDialog dialog;
    QString txt = a->header->description;
    txt.replace("\\n","\n");
    dialog.textBox.setPlainText(txt);
    dialog.setWindowTitle("Activity Description");
    dialog.exec();
    if(dialog.changed == 1){
        txt = dialog.textBox.toPlainText();
        txt.replace("\n","\\n");
        a->setDescription(txt);
    }
}

void ActivityTools::briefingOpenEnabled(){
    Activity *a = ActLib::Act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    TextEditDialog dialog;
    QString txt = a->header->briefing;
    txt.replace("\\n","\n");
    dialog.textBox.setPlainText(txt);
    dialog.setWindowTitle("Activity Description");
    dialog.exec();
    if(dialog.changed == 1){
        txt = dialog.textBox.toPlainText();
        txt.replace("\n","\\n");
        a->setBriefing(txt);
    }
}
    
ActivityTools::~ActivityTools() {
}

