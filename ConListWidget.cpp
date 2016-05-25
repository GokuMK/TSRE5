#include "ConListWidget.h"
#include "ConLib.h"
#include "Consist.h"
#include "Game.h"

ConListWidget::ConListWidget() : QWidget(){
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Show:",&conType);
    vbox->addItem(vlist);
    vbox->addWidget(&items);
    //vbox->addStretch(1);
    this->setLayout(vbox);
    conType.setStyleSheet("combobox-popup: 0;");
    conType.addItem("ALL");
    conType.addItem("Broken");
    conType.addItem("Unsaved");
    this->setMinimumWidth(250);
    QObject::connect(&items, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(itemsSelected(QListWidgetItem*)));
    
    QObject::connect(&conType, SIGNAL(activated(QString)),
                      this, SLOT(filterSelected(QString)));
}

ConListWidget::~ConListWidget() {
}

void ConListWidget::newConsist(){
    ConLib::con[ConLib::jestcon] = new Consist();
    ConLib::con[ConLib::jestcon]->setNewConsistFlag();
    ConLib::con[ConLib::jestcon]->showName = "new";
    new QListWidgetItem ( ConLib::con[ConLib::jestcon]->showName, &items, ConLib::jestcon);
    emit conListSelected(ConLib::jestcon);
    ConLib::jestcon++;
}

void ConListWidget::filterSelected(QString n){
    if(conType.currentIndex() == 0)
        n = "";
    fillConList(n);
}
void ConListWidget::fillConList(){
    fillConList("");
}

void ConListWidget::fillConList(QString n){
    items.clear();
    Game::currentEngLib = englib;
    Consist * e;
    bool brokenf = false;
    bool unsavedf = false;
    if(n.toLower() == "broken")
        brokenf = true;
    if(n.toLower() == "unsaved")
        unsavedf = true;      
    
    for (int i = 0; i < ConLib::jestcon; i++){
        e = ConLib::con[i];
        if(e == NULL) continue;
        if(e->loaded !=1) continue;
        if(brokenf)
            if(!e->isBroken()) continue;
        if(unsavedf)
            if(!e->isUnSaved()) continue;
        new QListWidgetItem ( e->showName, &items, i);
    }
    items.sortItems(Qt::AscendingOrder);
}

void ConListWidget::itemsSelected(QListWidgetItem * item){
    
    //qDebug() << item->type() << " " << item->text();
    emit conListSelected(item->type());
}