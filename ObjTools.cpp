#include "ObjTools.h"
#include "Route.h"


ObjTools::ObjTools(QString name)
    : QWidget(){
    //QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    //QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    //QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));

    QPushButton *selectTool = new QPushButton("Select", this);
    QPushButton *placeTool = new QPushButton("Place new", this);
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
    vbox->addItem(vlist);
    vbox->addWidget(&refList);
    refList.hide();
    vbox->addWidget(&trackList);
    vbox->addWidget(selectTool);
    vbox->addWidget(placeTool);
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
   
    refClass.setStyleSheet("combobox-popup: 0;");
    refTrack.setStyleSheet("combobox-popup: 0;");
    refRoad.setStyleSheet("combobox-popup: 0;");
    
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&refClass, SIGNAL(activated(QString)),
                      this, SLOT(refClassSelected(QString)));
    
    QObject::connect(&refTrack, SIGNAL(activated(QString)),
                      this, SLOT(refTrackSelected(QString)));
    
    QObject::connect(&refRoad, SIGNAL(activated(QString)),
                      this, SLOT(refTrackSelected(QString)));
    
    QObject::connect(&refList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(refListSelected(QListWidgetItem*)));
    
    QObject::connect(&trackList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(trackListSelected(QListWidgetItem*)));
    
    QObject::connect(&lastItems, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(lastItemsListSelected(QListWidgetItem*)));
    
    QObject::connect(selectTool, SIGNAL(released()),
                      this, SLOT(selectToolEnabled()));
    
    QObject::connect(placeTool, SIGNAL(released()),
                      this, SLOT(placeToolEnabled()));
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
}

void ObjTools::refClassSelected(const QString & text){
    //qDebug() << "Bbbb " << text;
    refList.clear();
    trackList.hide();
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
    refList.hide();
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

void ObjTools::refListSelected(QListWidgetItem * item){
    //refList.addItem(
    try{
        route->ref->selected = &route->ref->refItems[refClass.currentText().toStdString()][refList.currentRow()];
        itemSelected((int)route->ref->selected);
    } catch(const std::out_of_range& oor){
        route->ref->selected = NULL;
    }
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
        itemSelected((int)route->ref->selected);
    } catch(const std::out_of_range& oor){
        route->ref->selected = NULL;
    }
}

void ObjTools::lastItemsListSelected(QListWidgetItem * item){
    
    qDebug() << item->type() << " " << item->text();
    route->ref->selected = (Ref::RefItem*)item->type();
}

void ObjTools::selectToolEnabled(){
    emit enableTool("selectTool");
}

void ObjTools::placeToolEnabled(){
    emit enableTool("placeTool");
}

void ObjTools::itemSelected(int pointer){
    Ref::RefItem* item = (Ref::RefItem*) pointer;
    QString text;
    if(item->description.length() > 1) 
        text = item->description;
    else
        text = item->filename;
    new QListWidgetItem ( text, &lastItems, pointer );
        if(lastItems.count() > 11)
            delete lastItems.takeItem(0);
}