#include "ConUnitsWidget.h"
#include "Consist.h"
#include "Eng.h"
#include "EngLib.h"

ConUnitsWidget::ConUnitsWidget() {
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Units:",&count);
    vbox->addItem(vlist);
    vbox->addWidget(&items);
    //vbox->addStretch(1);
    this->setLayout(vbox);
    this->setMinimumWidth(250);
    QObject::connect(&items, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(itemsSelected(QListWidgetItem*)));
}

ConUnitsWidget::~ConUnitsWidget() {
}

void ConUnitsWidget::setCon(Consist* c){
    con = c;
    if(con == NULL) return;
    items.clear();
    count.setText(QString::number(con->engItems.size()));
    Eng * e;
    for (int i = 0; i < con->engItems.size(); i++){
        e = englib->eng[con->engItems[i].eng];
        if(e == NULL) continue;
        if(e->loaded !=1) continue;
        new QListWidgetItem ( e->engName, &items, i);
    }
}

void ConUnitsWidget::itemsSelected(QListWidgetItem * item){
    emit selected(item->type());
}