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
#include "ActivityServiceTools.h"
#include "TextEditDialog.h"

ActivityTools::ActivityTools(QString name)
    : QWidget(){

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
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QLabel *label = new QLabel("Activity List:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&actShow);
    actShow.setStyleSheet("combobox-popup: 0;");
    vbox->addWidget(newActButton);
    
    label = new QLabel("Player:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cService);
    cService.setStyleSheet("combobox-popup: 0;");
    QGridLayout *vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actServiceNew = new QPushButton("New");
    QObject::connect(actServiceNew, SIGNAL(released()), this, SLOT(actServiceNewEnabled()));
    QPushButton *actServiceEdit = new QPushButton("Edit");
    QPushButton *actServiceClone = new QPushButton("Clone");
    QPushButton *actServiceDelete = new QPushButton("Delete");
    vlist1->addWidget(actServiceNew,0,0);
    vlist1->addWidget(actServiceEdit,0,1);
    vlist1->addWidget(actServiceClone,0,2);
    vlist1->addWidget(actServiceDelete,0,3);
    vbox->addItem(vlist1);
    
    label = new QLabel("Traffic:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cTraffic);
    cTraffic.setStyleSheet("combobox-popup: 0;");
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actTrafficNew = new QPushButton("New");
    QPushButton *actTrafficEdit = new QPushButton("Edit");
    QPushButton *actTrafficClone = new QPushButton("Clone");
    QPushButton *actTrafficDelete = new QPushButton("Delete");
    vlist1->addWidget(actTrafficNew,0,0);
    vlist1->addWidget(actTrafficEdit,0,1);
    vlist1->addWidget(actTrafficClone,0,2);
    vlist1->addWidget(actTrafficDelete,0,3);
    vbox->addItem(vlist1);
    
    label = new QLabel("Paths:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&cPath);
    cPath.setStyleSheet("combobox-popup: 0;");
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actPathsNew = new QPushButton("New");
    QPushButton *actPathsEdit = new QPushButton("Edit");
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
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);
    QPushButton *actConsistJump = new QPushButton("Jump To");
    QPushButton *actConsistFlip = new QPushButton("Flip");
    QPushButton *actConsistDelete = new QPushButton("Delete");
    vlist1->addWidget(actConsistJump,0,0);
    vlist1->addWidget(actConsistFlip,0,1);
    vlist1->addWidget(actConsistDelete,0,2);
    vlist1->addWidget(buttonTools["actNewLooseConsistTool"],0,3,1,2);
    vbox->addItem(vlist1);
    label = new QLabel("Consist List:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    vbox->addWidget(&conFilesShow);
    conFilesShow.setStyleSheet("combobox-popup: 0;");
    conFilesShow.setMaxVisibleItems(35);
    conFilesShow.view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    label = new QLabel("Events:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QPushButton *actEventsOpen = new QPushButton("Open Event Editor");
    vbox->addWidget(actEventsOpen);
    
    label = new QLabel("Activity Info:");
    label->setStyleSheet(QString("QLabel { color : ")+Game::StyleMainLabel+"; }");
    label->setContentsMargins(3,0,0,0);
    vbox->addWidget(label);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("File Name:",&eFileName);
    QObject::connect(&eFileName, SIGNAL(textEdited(QString)), this, SLOT(eFileNameEnabled(QString)));
    vlist->addRow("Display Name:",&eDisplayName);
    QObject::connect(&eDisplayName, SIGNAL(textEdited(QString)), this, SLOT(eDisplayNameEnabled(QString)));
    vlist->addRow("Difficulty:",&cDifficulty);
    QObject::connect(&cDifficulty, SIGNAL(activated(int)), this, SLOT(cDifficultyEnabled(int)));
    vlist->addRow("Duration:",&eDuration);
    QObject::connect(&eDuration, SIGNAL(editingFinished()), this, SLOT(eDurationEnabled()));
    eDuration.setDisplayFormat("HH:mm");
    vlist->addRow("Start Time:",&eStartTime);
    QObject::connect(&eStartTime, SIGNAL(editingFinished()), this, SLOT(eStartTimeEnabled()));
    eStartTime.setDisplayFormat("HH:mm:ss");
    vlist->addRow("Season:",&cSeason);
    QObject::connect(&cSeason, SIGNAL(activated(int)), this, SLOT(cSeasonEnabled(int)));
    vlist->addRow("Weather:",&cWeather);
    QObject::connect(&cWeather, SIGNAL(activated(int)), this, SLOT(cWeatherEnabled(int)));
    vbox->addItem(vlist);
    int row = 0;
    int labelWidth = 70;
    vlist1 = new QGridLayout;
    vlist1->setSpacing(2);
    vlist1->setContentsMargins(0,0,1,0);

    eFuelCoal = GuiFunct::newQLineEdit(25,3);
    eFuelDiesel = GuiFunct::newQLineEdit(25,3);
    eFuelWater = GuiFunct::newQLineEdit(25,3);
    eHazardAnimal = GuiFunct::newQLineEdit(25,3);  
    eHazardPeople = GuiFunct::newQLineEdit(25,3);  
            
    vlist1->addWidget(GuiFunct::newQLabel("Fuel Coal:", labelWidth), row, 0);
    vlist1->addWidget(eFuelCoal, row, 1);
    vlist1->addWidget(&sFuelCoal, row++, 2);
    QObject::connect(eFuelCoal, SIGNAL(textEdited(QString)), this, SLOT(eFuelCoalEnabled(QString)));
    QObject::connect(&sFuelCoal, SIGNAL(sliderReleased()), this, SLOT(sFuelCoalEnabled()));
    sFuelCoal.setRange(0, 100);
    sFuelCoal.setOrientation(Qt::Horizontal);
    vlist1->addWidget(GuiFunct::newQLabel("Fuel Diesel:", labelWidth), row, 0);
    vlist1->addWidget(eFuelDiesel, row, 1);
    vlist1->addWidget(&sFuelDiesel, row++, 2);
    QObject::connect(eFuelDiesel, SIGNAL(textEdited(QString)), this, SLOT(eFuelDieselEnabled(QString)));
    QObject::connect(&sFuelDiesel, SIGNAL(sliderReleased()), this, SLOT(sFuelDieselEnabled()));
    sFuelDiesel.setRange(0, 100);
    sFuelDiesel.setOrientation(Qt::Horizontal);
    vlist1->addWidget(GuiFunct::newQLabel("Fuel Water:", labelWidth), row, 0);
    vlist1->addWidget(eFuelWater, row, 1);
    vlist1->addWidget(&sFuelWater, row++, 2);
    QObject::connect(eFuelWater, SIGNAL(textEdited(QString)), this, SLOT(eFuelWaterEnabled(QString)));
    QObject::connect(&sFuelWater, SIGNAL(sliderReleased()), this, SLOT(sFuelWaterEnabled()));
    sFuelWater.setRange(0, 100);
    sFuelWater.setOrientation(Qt::Horizontal);
    vlist1->addWidget(GuiFunct::newQLabel("Hazard Animal:", labelWidth), row, 0);
    vlist1->addWidget(eHazardAnimal, row, 1);
    vlist1->addWidget(&sHazardAnimal, row++, 2);
    QObject::connect(eHazardAnimal, SIGNAL(textEdited(QString)), this, SLOT(eHazardAnimalEnabled(QString)));
    QObject::connect(&sHazardAnimal, SIGNAL(sliderReleased()), this, SLOT(sHazardAnimalEnabled()));
    sHazardAnimal.setRange(0, 100);
    sHazardAnimal.setOrientation(Qt::Horizontal);
    vlist1->addWidget(GuiFunct::newQLabel("Hazard People:", labelWidth), row, 0);
    vlist1->addWidget(eHazardPeople, row, 1);
    vlist1->addWidget(&sHazardPeople, row++, 2);
    QObject::connect(eHazardPeople, SIGNAL(textEdited(QString)), this, SLOT(eHazardPeopleEnabled(QString)));
    QObject::connect(&sHazardPeople, SIGNAL(sliderReleased()), this, SLOT(sHazardPeopleEnabled()));
    sHazardPeople.setRange(0, 100);
    sHazardPeople.setOrientation(Qt::Horizontal);
    vbox->addItem(vlist1);
    QPushButton *descriptionOpen = new QPushButton("Edit Description");
    QObject::connect(descriptionOpen, SIGNAL(released()), this, SLOT(descriptionOpenEnabled()));
    vbox->addWidget(descriptionOpen);
    QPushButton *briefingOpen = new QPushButton("Edit Briefing");
    vbox->addWidget(briefingOpen);
    QObject::connect(briefingOpen, SIGNAL(released()), this, SLOT(briefingOpenEnabled()));
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&consists, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(itemsSelected(QListWidgetItem*)));
    QObject::connect(&actShow, SIGNAL(activated(QString)),
                      this, SLOT(activitySelected(QString)));
    QObject::connect(&conFilesShow, SIGNAL(activated(QString)),
                      this, SLOT(conFilesShowEnabled(QString)));
    QObject::connect(&cService, SIGNAL(activated(QString)),
                      this, SLOT(cServiceEnabled(QString)));
    //QObject::connect(loadActFilesButton, SIGNAL(released()),
    //                  this, SLOT(loadActFiles()));
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
    consists.clear();
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

