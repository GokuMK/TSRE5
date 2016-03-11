#include "EngListWidget.h"
#include "EngLib.h"
#include "Eng.h"
#include "Game.h"

EngListWidget::EngListWidget() : QWidget(){
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Type:",&engType);
    vlist->addRow("Coupling:",&couplingType);
    vbox->addItem(vlist);
    vbox->addWidget(&items);
    //vbox->addStretch(1);
    this->setLayout(vbox);
    engType.setStyleSheet("combobox-popup: 0;");
    engType.addItem("ALL");
    this->setMinimumWidth(250);
    couplingType.setStyleSheet("combobox-popup: 0;");
    couplingType.addItem("ALL");
    
    QObject::connect(&items, SIGNAL(itemSelectionChanged()),
                      this, SLOT(itemsSelected()));
}

EngListWidget::~EngListWidget() {
}

void EngListWidget::fillEngList(){
    items.clear();

    Eng * e;
    for (int i = 0; i < englib->jesteng; i++){
        e = englib->eng[i];
        if(e == NULL) continue;
        if(e->loaded !=1) continue;
        new QListWidgetItem ( e->displayName, &items, i);
    }
    items.sortItems(Qt::AscendingOrder);
}

void EngListWidget::itemsSelected(){
    QListWidgetItem * item = items.currentItem();
    //qDebug() << item->type() << " " << item->text();
    emit engListSelected(item->type());
}