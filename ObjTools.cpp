#include "TSectionDAT.h"
#include "ObjTools.h"
#include "Route.h"
#include "Game.h"
#include "SigCfg.h"
#include "SignalShape.h"
#include "ForestObj.h"
#include "SpeedPost.h"
#include "SpeedPostDAT.h"
#include "SoundList.h"

ObjTools::ObjTools(QString name)
    : QWidget(){
    //QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    //QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    //QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));

    QPushButton *selectTool = new QPushButton("Select", this);
    QPushButton *placeTool = new QPushButton("Place New", this);
    QPushButton *resetRotationButton = new QPushButton("Reset Place Rot", this);
    //radio1->setChecked(true);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QLabel *label1 = new QLabel("Objects:");
    label1->setStyleSheet("QLabel { color : #999999; }");
    label1->setContentsMargins(3,0,0,0);
    vbox->addWidget(label1);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Ref file:",&refClass);
    vlist->addRow("Tracks:",&refTrack);
    vlist->addRow("Roads:",&refRoad);
    vlist->addRow("Other:",&refOther);
    vbox->addItem(vlist);
    vbox->addWidget(&refList);
    refList.hide();
    vbox->addWidget(&trackList);
    vbox->addWidget(&otherList);
    otherList.hide();
    vbox->addWidget(selectTool);
    vbox->addWidget(placeTool);
    vbox->addWidget(resetRotationButton);
    stickToTDB.setText("Stick To TrackDB");
    stickToTDB.setChecked(false);
    vbox->addWidget(&stickToTDB);
    QLabel *label2 = new QLabel("Recent items:");
    label2->setStyleSheet("QLabel { color : #999999; }");
    label2->setContentsMargins(3,0,0,0);
    //refClasssetMargin(0);
    //refTrack->sets >setMargin(0);
    //refRoad->setMargin(0);
    //label1->setMargin(0);
    vbox->addWidget(label2);
    vbox->addWidget(&lastItems);

    lastItems.setMinimumHeight(170);
    lastItems.setMaximumHeight(170);
    //QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    //astItems.setSizePolicy(*sizePolicy);
    //) QSizePolicy::MinimumExpanding);
    //lastItems.setMaximumHeight(999);
    trackList.setMinimumHeight(250);
    refList.setMinimumHeight(250);
    otherList.setMinimumHeight(250);
    refClass.setStyleSheet("combobox-popup: 0;");
    refTrack.setStyleSheet("combobox-popup: 0;");
    refRoad.setStyleSheet("combobox-popup: 0;");
    refOther.setStyleSheet("combobox-popup: 0;");
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&refClass, SIGNAL(activated(QString)),
                      this, SLOT(refClassSelected(QString)));
    
    QObject::connect(&refTrack, SIGNAL(activated(QString)),
                      this, SLOT(refTrackSelected(QString)));
    
    QObject::connect(&refRoad, SIGNAL(activated(QString)),
                      this, SLOT(refTrackSelected(QString)));
    
    QObject::connect(&refOther, SIGNAL(activated(QString)),
                      this, SLOT(refOtherSelected(QString)));
    
    QObject::connect(&refList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(refListSelected(QListWidgetItem*)));
    
    QObject::connect(&trackList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(trackListSelected(QListWidgetItem*)));
    
    QObject::connect(&otherList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(otherListSelected(QListWidgetItem*)));
    
    QObject::connect(&lastItems, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(lastItemsListSelected(QListWidgetItem*)));
    
    QObject::connect(selectTool, SIGNAL(released()),
                      this, SLOT(selectToolEnabled()));
    
    QObject::connect(placeTool, SIGNAL(released()),
                      this, SLOT(placeToolEnabled()));
    
    QObject::connect(resetRotationButton, SIGNAL(released()),
                      this, SLOT(resetRotationButtonEnabled()));
    
    QObject::connect(&stickToTDB, SIGNAL(stateChanged(int)),
                      this, SLOT(stickToTDBEnabled(int)));
    
    
}

ObjTools::~ObjTools() {
}

void ObjTools::routeLoaded(Route* a){
    this->route = a;

    QStringList hash;
    QStringList hash2;
    for ( auto it = route->ref->refItems.begin(); it != route->ref->refItems.end(); ++it ){
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        hash.append(QString::fromStdString(it->first));
        //refClass.addItem(QString::fromStdString(it->first), QVariant(QString::fromStdString(it->first)));
      //std::cout << " " << it->first << ":" << it->second;
    }
    hash.sort(Qt::CaseInsensitive);
    hash.removeDuplicates();
    refClass.addItems(hash);
    refClass.setMaxVisibleItems(25);
    //trackList.clear();
    /*TrackShape * track;
    for (auto it = route->trackDB->tsection->shape.begin(); it != route->trackDB->tsection->shape.end(); ++it ){
        track = it->second;
        //qDebug() << track->filename;
        if(track->filename.startsWith("a1t", Qt::CaseInsensitive) || track->filename.startsWith("a2t", Qt::CaseInsensitive) )
            new QListWidgetItem ( track->filename, &trackList, track->id );
        //refList.addItem(route->ref->refItems[text.toStdString()][it].description);
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        //refClass.addItem(QString::fromStdString(it->first), QVariant(QString::fromStdString(it->first)));
      //std::cout << " " << it->first << ":" << it->second;
    }*/
    TrackShape * track;
    //std::string hash;
    //std::unordered_map<std::string, int> types;
    //refTrack.setInsertPolicy(refTrack.InsertAlphabetically);
    hash.clear();
    hash2.clear();
    int i = 0;
    int a1ti = 0;
    for (auto it = route->tsection->shape.begin(); it != route->tsection->shape.end(); ++it ){
        track = it->second;
        //hash = track->filename.left(3).toStdString();
        if(track == NULL) continue;
        if(track->dyntrack) continue;
        if(track->roadshape)
            hash2.append(track->filename.left(3).toLower());
        else {
            i++;
            if(track->filename.left(3).toLower() == "a1t")
                a1ti = i;
            hash.append(track->filename.left(3).toLower());
        }
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        //if(types[hash] != 1){
        //    refTrack.addItem(QString::fromStdString(hash));//, QVariant(QString::fromStdString(hash)));
            //types[hash] = 1;
        //}
      //std::cout << " " << it->first << ":" << it->second;
    }
    hash.sort(Qt::CaseInsensitive);
    hash.removeDuplicates();
    refTrack.addItems(hash);
    refTrack.setMaxVisibleItems(25);
    hash2.sort(Qt::CaseInsensitive);
    hash2.removeDuplicates();
    refRoad.addItems(hash2);
    refRoad.setMaxVisibleItems(25);
    //refTrack.s .sortItems(Qt::AscendingOrder);
    
    refTrack.setCurrentText("a1t");
    refTrackSelected("a1t");
    
    refOther.addItem("Signals");
    refOther.addItem("Forests");
    refOther.addItem("Sound Sources");
    refOther.addItem("Sound Regions");    
    refOther.addItem("SpeedSign");
    refOther.addItem("SpeedResume");
    refOther.addItem("SpeedWarning");
    refOther.addItem("Milepost");
    refOther.setMaxVisibleItems(25);
}

void ObjTools::refClassSelected(const QString & text){
    //qDebug() << "Bbbb " << text;
    refList.clear();
    hideAllLists();
    refList.show();
    for (int it = 0; it < route->ref->refItems[text.toStdString()].size(); ++it ){
        refList.addItem(route->ref->refItems[text.toStdString()][it].description);
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        //refClass.addItem(QString::fromStdString(it->first), QVariant(QString::fromStdString(it->first)));
      //std::cout << " " << it->first << ":" << it->second;
    }
}

void ObjTools::refTrackSelected(const QString & text){
    trackList.clear();
    hideAllLists();
    trackList.show();
    TrackShape * track;
    for (auto it = route->tsection->shape.begin(); it != route->tsection->shape.end(); ++it ){
        track = it->second;
        //qDebug() << track->filename;
        if(track == NULL) continue;
        if(track->filename.startsWith(text, Qt::CaseInsensitive) )
            new QListWidgetItem ( track->filename, &trackList, track->id );
        //refList.addItem(route->ref->refItems[text.toStdString()][it].description);
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        //refClass.addItem(QString::fromStdString(it->first), QVariant(QString::fromStdString(it->first)));
      //std::cout << " " << it->first << ":" << it->second;
    }
    trackList.sortItems(Qt::AscendingOrder);
}

void ObjTools::refOtherSelected(const QString & text){
    otherList.clear();
    hideAllLists();
    otherList.show();
    if(text.toLower() == "signals"){
        SignalShape * signal;
        for (auto it = Game::trackDB->sigCfg->signalShape.begin(); it != Game::trackDB->sigCfg->signalShape.end(); ++it ){
            signal = it->second;
            if(signal == NULL) continue;
            new QListWidgetItem ( signal->desc, &otherList, signal->listId );
        }
    }
    if(text.toLower() == "forests"){
        for (int i = 0; i < ForestObj::forestList.size(); i++){
            new QListWidgetItem ( ForestObj::forestList[i].name, &otherList, i );
        }
    }
    if(text.toLower() == "speedsign"){
        for (int i = 0; i < Game::trackDB->speedPostDAT->speedPost.size(); i++){
            if(Game::trackDB->speedPostDAT->speedPost[i]->speedSignShapeCount > 0)
                new QListWidgetItem ( Game::trackDB->speedPostDAT->speedPost[i]->name, &otherList, i*1000+0 );
        }
    }
    if(text.toLower() == "speedresume"){
        for (int i = 0; i < Game::trackDB->speedPostDAT->speedPost.size(); i++){
            if(Game::trackDB->speedPostDAT->speedPost[i]->speedResumeSignShapeCount > 0)
                new QListWidgetItem ( Game::trackDB->speedPostDAT->speedPost[i]->name, &otherList, i*1000+1 );
        }
    }
    if(text.toLower() == "speedwarning"){
        for (int i = 0; i < Game::trackDB->speedPostDAT->speedPost.size(); i++){
            if(Game::trackDB->speedPostDAT->speedPost[i]->speedWarningSignShapeCount > 0)
                new QListWidgetItem ( Game::trackDB->speedPostDAT->speedPost[i]->name, &otherList, i*1000+2 );
        }
    }
    if(text.toLower() == "milepost"){
        for (int i = 0; i < Game::trackDB->speedPostDAT->speedPost.size(); i++){
            if(Game::trackDB->speedPostDAT->speedPost[i]->milepostShapeCount > 0)
                new QListWidgetItem ( Game::trackDB->speedPostDAT->speedPost[i]->name, &otherList, i*1000+3 );
        }
    }
    if(text.toLower() == "sound sources"){
        for (auto it = route->soundList->sources.begin(); it != route->soundList->sources.end(); ++it ){
            new QListWidgetItem ( it->second->name, &otherList, it->second->id );
        }
    }
    if(text.toLower() == "sound regions"){
        for (auto it = route->soundList->regions.begin(); it != route->soundList->regions.end(); ++it ){
            new QListWidgetItem ( it->second->name, &otherList, it->second->id );
        }
    }
    otherList.sortItems(Qt::AscendingOrder);
}

void ObjTools::refListSelected(QListWidgetItem * item){
    //refList.addItem(
    try{
        route->ref->selected = &route->ref->refItems[refClass.currentText().toStdString()][refList.currentRow()];
        itemSelected((Ref::RefItem*)route->ref->selected);
    } catch(const std::out_of_range& oor){
        route->ref->selected = NULL;
    }
}

void ObjTools::hideAllLists(){
    otherList.hide();
    refList.hide();
    trackList.hide();
}

void ObjTools::trackListSelected(QListWidgetItem * item){
    qDebug() << item->type() << " " << item->text();
    Ref::RefItem* itemRef = new Ref::RefItem(); 
    itemRef->filename = item->text();
    itemRef->clas = "";
    itemRef->type = "trackobj";
    itemRef->value = item->type();
    try{
        route->ref->selected = itemRef;
        itemSelected((Ref::RefItem*)route->ref->selected);
    } catch(const std::out_of_range& oor){
        route->ref->selected = NULL;
    }
}

void ObjTools::otherListSelected(QListWidgetItem * item){
    Ref::RefItem* itemRef = new Ref::RefItem(); 
    if(refOther.currentText().toLower() == "signals") itemRef->type = "signal";
    if(refOther.currentText().toLower() == "forests") itemRef->type = "forest";
    if(refOther.currentText().toLower() == "speedsign") itemRef->type = "speedpost";
    if(refOther.currentText().toLower() == "speedresume") itemRef->type = "speedpost";
    if(refOther.currentText().toLower() == "speedwarning") itemRef->type = "speedpost";
    if(refOther.currentText().toLower() == "milepost") itemRef->type = "speedpost";
    if(refOther.currentText().toLower() == "sound sources") itemRef->type = "soundsource";
    if(refOther.currentText().toLower() == "sound regions") itemRef->type = "soundregion";
    qDebug() << item->type() << " " << item->text();
    itemRef->clas = refOther.currentText().toLower();
    itemRef->filename = item->text();
    itemRef->value = item->type();
    try{
        route->ref->selected = itemRef;
        itemSelected((Ref::RefItem*)route->ref->selected);
    } catch(const std::out_of_range& oor){
        route->ref->selected = NULL;
    }
}

void ObjTools::lastItemsListSelected(QListWidgetItem * item){
    
    qDebug() << item->type() << " " << item->text();
    route->ref->selected = lastItemsPtr[item->type()];
}

void ObjTools::selectToolEnabled(){
    emit enableTool("selectTool");
}

void ObjTools::placeToolEnabled(){
    emit enableTool("placeTool");
}

void ObjTools::resetRotationButtonEnabled(){
    emit sendMsg("resetPlaceRotation");
}

void ObjTools::itemSelected(Ref::RefItem* item){
    QString text;
    if(item->description.length() > 1) 
        text = item->description;
    else
        text = item->filename;
    lastItemsPtr.push_back(item);
    
    new QListWidgetItem ( text, &lastItems, lastItemsPtr.size() - 1 );
        if(lastItems.count() > 11)
            delete lastItems.takeItem(0);
}

void ObjTools::stickToTDBEnabled(int state){
    if(state == Qt::Checked)
        this->sendMsg("stickToTDB", true);
    else
        this->sendMsg("stickToTDB", false);
}

void ObjTools::msg(QString text){
}

void ObjTools::msg(QString text, bool val){
}

void ObjTools::msg(QString text, int val){
}

void ObjTools::msg(QString text, float val){
}

void ObjTools::msg(QString text, QString val){
}