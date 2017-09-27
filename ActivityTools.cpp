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
    QPushButton *actConsistFlip = new QPushButton("Flip");
    QPushButton *actConsistDelete = new QPushButton("Delete");
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
    //QObject::connect(&cHornAtCrossings, SIGNAL(editingFinished()), this, SLOT(eStartTimeEnabled()));
    
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
    eDescription.setMinimumWidth(350);
    vbox2->addWidget(new QLabel("Briefing:"));
    vbox2->addWidget(&eBriefing);
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
    settingsWidget.show();
}

void ActivityTools::actEventsOpenEnabled(){
    emit showActivityEventEditor();
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    emit showEvents(a);
}

void ActivityTools::actServiceOpenEnabled(){
    emit showActivityServiceEditor();
    emit showServices(route);
}

void ActivityTools::actTimetableOpenEnabled(){
    emit showActivityTimetableEditor();
    emit showTimetable(route);
}

void ActivityTools::actTrafficOpenEnabled(){
    emit showActivityTrafficEditor();
    emit showTraffic(route);
}

void ActivityTools::routeLoaded(Route* r){
    route = r;
    actShow.clear();
    consists.clear();
    foreach(int id, route->activityId){
        actShow.addItem(ActLib::act[id]->header->name, QVariant(id));
    }
    actShow.setCurrentIndex(-1);
    
    ConLib::loadSimpleList(Game::root);
    foreach(QString name, ConLib::conFileList){
        conFilesShow.addItem(name.section('/', -1), QVariant(name));
    }
    
    reloadServicesList();

    cTraffic.clear();
    cTraffic.addItem("UNDEFINED", QVariant(-1));
    for(int i = 0; i < route->traffic.size(); i++ )
        cTraffic.addItem(route->traffic[i]->displayName, QVariant(i));

    cPath.clear();
    for(int i = 0; i < route->path.size(); i++ )
        cPath.addItem(route->path[i]->displayName, QVariant(i));
}

void ActivityTools::reloadServicesList(){
    cService.clear();
    int idx = cService.currentIndex();

    cService.addItem("UNDEFINED", QVariant(-1));
    for(int i = 0; i < route->service.size(); i++ )
        cService.addItem(route->service[i]->displayName, QVariant(i));
    
    if(idx >= 0)
        cService.setCurrentIndex(idx);
}

void ActivityTools::conFilesShowEnabled(QString val){
    QString file = conFilesShow.currentData().toString();
    qDebug() << file;
    int id = actShow.currentData().toInt();
    if(ActLib::act[id] == NULL)
        return;
    ActLib::act[id]->editorConListSelected = file;
}

void ActivityTools::activitySelected(QString n){
    int id = actShow.currentData().toInt();
    Consist *e;
    Activity *a = ActLib::act[id];
    
    if(ActLib::act[id] == NULL)
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
    
   
    if(a->playerServiceDefinition == NULL){
        cService.setCurrentIndex(0);
    } else {
        cService.setCurrentIndex(0);
        QString cname = a->playerServiceDefinition->name.toLower();
        for(int i = 0; i < cService.count() ; i++ ){
            int id = cService.itemData(i).toInt();
            if(id < 0)
                continue;
            if(route->service[id] == NULL)
                continue;
            //qDebug() << cname << route->service[id]->name;
            if(cname == route->service[id]->nameId){
                cService.setCurrentIndex(i);
                break;
            }
        }
    }
    
    if(a->traffic == NULL){
        cTraffic.setCurrentIndex(0);
    } else {
        cTraffic.setCurrentIndex(0);
        QString cname = a->traffic->name.toLower();
        for(int i = 0; i < cTraffic.count() ; i++ ){
            int id = cTraffic.itemData(i).toInt();
            if(id < 0)
                continue;
            if(route->traffic[id] == NULL)
                continue;
            //qDebug() << cname << route->service[id]->name;
            if(cname == route->traffic[id]->nameId){
                cTraffic.setCurrentIndex(i);
                break;
            }
        }
    }
    
    consists.clear();
    for (int i = 0; i < ActLib::act[id]->activityObjects.size(); i++){
        e = ActLib::act[id]->activityObjects[i].con;
        if(e == NULL) continue;
        consists.addItem(e->showName, QVariant(i));
    }

    if(route != NULL){
        //if(!ActLib::act[id]->isInitActivityObjects)
        //    ActLib::act[id]->initActivityObjects();
        route->activitySelected(ActLib::act[id]);
    }
    
    QString txt = a->header->briefing;
    txt.replace("\\n","\n");
    eBriefing.setPlainText(txt);
    txt = a->header->description;
    txt.replace("\\n","\n");
    eDescription.setPlainText(txt);
}

void ActivityTools::loadActFiles(){
    if(route == NULL)
        return;
    
}

void ActivityTools::actNewLooseConsistToolEnabled(bool val){
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
    int id = cService.currentData().toInt();
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::act[aid];
    
    if(id == -1){
        a->playerServiceDefinition = NULL;
    } else {
        int startTime = a->header->startTime[0]*60*60 + a->header->startTime[0]*60;
        a->createNewPlayerService(route->service[id]->nameId, startTime);
    }
}

void ActivityTools::cTrafficEnabled(QString val){
    int id = cTraffic.currentData().toInt();
    int aid = actShow.currentData().toInt();
    Activity *a = ActLib::act[aid];
    
    if(id == -1){
        a->traffic = NULL;
    } else {
        a->createNewTrafficService(route->traffic[id]);
    }
}

void ActivityTools::newActButtonEnabled(){
    QString pathid = Game::root + "/routes/" + Game::route + "/activities/";
    QString name = "New Activity";
    ActLib::act[ActLib::jestact] = new Activity(pathid, "file.act", true);
    ActLib::act[ActLib::jestact]->init(Game::route, name);
    route->activityId.push_back(ActLib::jestact);
    
    qDebug()<< ActLib::act[ActLib::jestact]->header->name;
    actShow.addItem(ActLib::act[ActLib::jestact]->header->name, QVariant(ActLib::jestact));
    actShow.setCurrentIndex(actShow.count()-1); 
    activitySelected("");
    
    ActLib::jestact++;
}
/*
void ActivityTools::actServiceNewEnabled(){
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
}

void ActivityTools::actServiceEditEnabled(){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    ActivityServiceTools sTools;
    //QString pathid = Game::root + "/routes/" + Game::route + "/services/";
    //QString name = "aaaaa1";
    Service *s = NULL;
    for(int i = 0; i < route->service.size(); i++){
        qDebug() << route->service[i]->pathId << a->playerServiceDefinition->name;
        if(route->service[i]->pathId.toLower() == a->playerServiceDefinition->name.toLower()){
            s = route->service[i];
            break;
        }
    }
    if(s == NULL)
        return;
    sTools.setData(s, route->path);
    sTools.exec();
    if(sTools.changed) {
        //route->service.push_back(s);
        //reloadServicesList();
    }
}*/

void ActivityTools::eFileNameEnabled(QString val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setFileName(val);
}

void ActivityTools::eDisplayNameEnabled(QString val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setDisplayName(val);
}

void ActivityTools::cDifficultyEnabled(int val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setDifficulty(val);
}

void ActivityTools::eDurationEnabled(){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setDuration(eDuration.time().hour(),  eDuration.time().minute());
}

void ActivityTools::eStartTimeEnabled(){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setStartTime(eStartTime.time().hour(), eStartTime.time().minute(), eStartTime.time().second());
}

void ActivityTools::cSeasonEnabled(int val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setSeason(val);
}

void ActivityTools::cWeatherEnabled(int val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    a->setWeather(val);
}

void ActivityTools::eFuelCoalEnabled(QString val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
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
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eFuelCoal->setText(QString::number(sFuelCoal.value()));
    a->setFuelCoal(sFuelCoal.value());
}

void ActivityTools::eFuelDieselEnabled(QString val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
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
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eFuelDiesel->setText(QString::number(sFuelDiesel.value()));
    a->setFuelDiesel(sFuelDiesel.value());
}

void ActivityTools::eFuelWaterEnabled(QString val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
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
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eFuelWater->setText(QString::number(sFuelWater.value()));
    a->setFuelWater(sFuelWater.value());
}

void ActivityTools::eHazardAnimalEnabled(QString val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
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
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eHazardAnimal->setText(QString::number(sHazardAnimal.value()));
    a->setHazardAnimals(sHazardAnimal.value());
}

void ActivityTools::eHazardPeopleEnabled(QString val){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
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
    Activity *a = ActLib::act[actShow.currentData().toInt()];
    if(a == NULL)
        return;
    eHazardPeople->setText(QString::number(sHazardPeople.value()));
    a->setHazardWorkers(sHazardPeople.value());
}

void ActivityTools::descriptionOpenEnabled(){
    Activity *a = ActLib::act[actShow.currentData().toInt()];
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
    Activity *a = ActLib::act[actShow.currentData().toInt()];
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

