#include "EngListWidget.h"
#include "EngLib.h"
#include "Eng.h"
#include "Game.h"

EngListWidget::EngListWidget() : QWidget(){
    addBegButton.setText("Add Beg");
    addCurButton.setText("Add Cur");
    addEndButton.setText("Add End");
    addRandButton.setText("Add Rand");
    addNum.setText("1");
    
    engType.addItem("ALL");
    engType.addItem("electric");
    engType.addItem("diesel");
    engType.addItem("steam");
    engType.addItem("carriage");
    engType.addItem("freight");
    engType.addItem("tender");

    couplingType.addItem("ALL");
    couplingType.addItem("Chain");
    couplingType.addItem("Automatic");
    couplingType.addItem("Bar");
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(2);
    vbox->setContentsMargins(0,1,1,1);
    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Total:", &totalVal);
    vlist->addRow("Type:", &engType);
    vlist->addRow("Coupling:", &couplingType);
    vlist->addRow("Search", &searchBox);
    vlist->addRow("Num to add", &addNum);
    vbox->addItem(vlist);
    QHBoxLayout *addbuttons = new QHBoxLayout;
    addbuttons->addWidget(&addBegButton);
    addbuttons->addWidget(&addCurButton);
    addbuttons->addWidget(&addEndButton);
    addbuttons->addWidget(&addRandButton);
    //vbox->addWidget();
    vbox->addItem(addbuttons);
    vbox->addWidget(&items);
    //vbox->addStretch(1);
    this->setLayout(vbox);
    engType.setStyleSheet("combobox-popup: 0;");
    this->setMinimumWidth(250);
    couplingType.setStyleSheet("combobox-popup: 0;");
    
    QObject::connect(&engType, SIGNAL(activated(QString)),
                      this, SLOT(fs(QString)));
    QObject::connect(&couplingType, SIGNAL(activated(QString)),
                      this, SLOT(fs(QString)));
    QObject::connect(&searchBox, SIGNAL(textEdited(QString)),
                      this, SLOT(fs(QString)));
    
    QObject::connect(&items, SIGNAL(itemSelectionChanged()),
                      this, SLOT(itemsSelected()));
    
    QObject::connect(&addBegButton, SIGNAL(released()),
                      this, SLOT(addBegButtonSelected()));
    QObject::connect(&addCurButton, SIGNAL(released()),
                      this, SLOT(addCurButtonSelected()));
    QObject::connect(&addEndButton, SIGNAL(released()),
                      this, SLOT(addEndButtonSelected()));
    QObject::connect(&addRandButton, SIGNAL(released()),
                      this, SLOT(addRndButtonSelected()));
    
    items.viewport()->installEventFilter(this);
    items.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    totalVal.setEnabled(false);
}

EngListWidget::~EngListWidget() {
}

void EngListWidget::fs(QString n){
    QString ef = engType.currentText();
    if(engType.currentIndex() == 0)
        ef = "";
    QString cf = couplingType.currentText();
    if(couplingType.currentIndex() == 0)
        cf = "";
    QString sf = searchBox.text();
    qDebug() << ef <<" "<< cf <<" "<< sf;
    fillEngList(ef, cf, sf);
}

void EngListWidget::fillEngList(){
    fillEngList("","","");
}

void EngListWidget::fillEngList(QString engFilter, QString couplingFilter, QString searchFilter){
    items.clear();

    Eng * e;
    totalVal.setText(QString::number(englib->jesteng));
    
    for (int i = 0; i < englib->jesteng; i++){
        e = englib->eng[i];
        if(e == NULL) continue;
        if(e->loaded !=1) continue;
        if(!e->engFilter(engFilter)) continue;
        if(!e->couplingFilter(couplingFilter)) continue;
        if(!e->searchFilter(searchFilter)) continue;
        
        new QListWidgetItem ( e->displayName, &items, i);
    }
    items.sortItems(Qt::AscendingOrder);
}

void EngListWidget::itemsSelected(){
    QListWidgetItem * item = items.currentItem();
    //qDebug() << item->type() << " " << item->text();
    emit engListSelected(item->type());
}

void EngListWidget::addBegButtonSelected(){
    bool ok = false;
    int count = addNum.text().toInt(&ok);
    if(!ok) count = 1;
    addBegButtonSelected(count);
}

void EngListWidget::addBegButtonSelected(int count){
    QListWidgetItem * item = items.currentItem();
    if(item == NULL) return;
    emit addToConSelected(item->type(), 0, count);
}

void EngListWidget::addCurButtonSelected(){
    bool ok = false;
    int count = addNum.text().toInt(&ok);
    if(!ok) count = 1;
    addCurButtonSelected(count);
}

void EngListWidget::addCurButtonSelected(int count){
    QListWidgetItem * item = items.currentItem();
    if(item == NULL) return;
    emit addToConSelected(item->type(), 1, count);
}

void EngListWidget::addEndButtonSelected(){
    bool ok = false;
    int count = addNum.text().toInt(&ok);
    if(!ok) count = 1;
    addEndButtonSelected(count);
}

void EngListWidget::addEndButtonSelected(int count){
    QListWidgetItem * item = items.currentItem();
    if(item == NULL) return;
    emit addToConSelected(item->type(), 2, count);
}

void EngListWidget::addRndButtonSelected(){
    QListWidgetItem * item = items.currentItem();
    if(item == NULL) return;
    emit f0(item->type());
}

bool EngListWidget::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonDblClick) {
        QMouseEvent * mouseEvent = static_cast <QMouseEvent *> (event);
        if (mouseEvent->button() == Qt::LeftButton) {
            addEndButtonSelected(1);
        }
    }
    return QWidget::eventFilter(obj, event);
}