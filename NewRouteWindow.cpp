#include "NewRouteWindow.h"

NewRouteWindow::NewRouteWindow() : QDialog(){
    this->setFixedSize(200, 100);

    QPushButton* ok = new QPushButton("OK");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));

    QFormLayout *vlist = new QFormLayout;
    vlist->setSpacing(2);
    vlist->setContentsMargins(3,0,3,0);
    vlist->addRow("Name:",&this->name);
    vlist->addRow("Lat:",&this->lat);
    vlist->addRow("Lon:",&this->lon);
    vlist->addRow(ok,cancel);
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
}

void NewRouteWindow::cancel(){
    this->changed = false;
    this->close();
}
void NewRouteWindow::ok(){
    this->changed = true;
    this->close();
}

NewRouteWindow::~NewRouteWindow() {
}

