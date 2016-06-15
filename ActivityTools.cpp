#include "ActivityTools.h"
#include "Route.h"
#include "Game.h"
#include "ActLib.h"
#include "EngLib.h"
#include "ConLib.h"
#include "Eng.h"
#include "Consist.h"
#include "Activity.h"

ActivityTools::ActivityTools(QString name)
    : QWidget(){
    QPushButton *loadActFilesButton = new QPushButton("Load Activities", this);
    QPushButton *placeTool = new QPushButton("Place New", this);
    QPushButton *resetRotationButton = new QPushButton("Reset Place Rot", this);
    //radio1->setChecked(true);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QLabel *label1 = new QLabel("Objects:");
    label1->setStyleSheet("QLabel { color : #999999; }");
    label1->setContentsMargins(3,0,0,0);
    vbox->addWidget(&actShow);
    actShow.setStyleSheet("combobox-popup: 0;");
    vbox->addWidget(&consists);
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&consists, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(itemsSelected(QListWidgetItem*)));
    QObject::connect(&actShow, SIGNAL(activated(QString)),
                      this, SLOT(fillConListAct(QString)));
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
}

void ActivityTools::fillConListAct(QString n){
    int id = actShow.currentData().toInt();
    consists.clear();
    Consist * e;
    
    for (int i = 0; i < ActLib::act[id]->activityObjects.size(); i++){
        e = ActLib::act[id]->activityObjects[i].con;
        if(e == NULL) continue;
        new QListWidgetItem ( e->showName, &consists, i);
    }
}

void ActivityTools::loadActFiles(){
    if(route == NULL)
        return;
    
}

ActivityTools::~ActivityTools() {
}

