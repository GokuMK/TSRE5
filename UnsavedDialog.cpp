#include "UnsavedDialog.h"

UnsavedDialog::UnsavedDialog() : QDialog(){
    //this->setFixedWidth(300);
    items.setFixedWidth(300);
    //this->setFixedSize(300,300);

    //QLabel *label = new QLabel("Save changes in consists?");
    QPushButton* ok = new QPushButton("Save and Quit");
    QPushButton* exit = new QPushButton("Discard and Quit");
    QPushButton* cancel = new QPushButton("Cancel");
    connect(ok, SIGNAL (released()), this, SLOT (ok()));
    connect(cancel, SIGNAL (released()), this, SLOT (cancel()));
    connect(exit, SIGNAL (released()), this, SLOT (exit()));
    
    QGridLayout *vlist = new QGridLayout;
    vlist->setSpacing(2);
    vlist->addWidget(&infoLabel, 0, 0, 1, 3, Qt::AlignCenter);
    //vlist->addWidget(new QLabel("New FileName:"), 1, 0, Qt::AlignLeft);
    //vlist->addWidget(&name, 1, 1, 1, 1, Qt::AlignLeft);
    //QHBoxLayout *vlist1 = new QHBoxLayout;
    vlist->addWidget(&items, 1, 0, 1, 3, Qt::AlignCenter);
    vlist->addWidget(ok, 2, 0);
    vlist->addWidget(exit, 2, 1);
    vlist->addWidget(cancel, 2, 2);
    
//    mainLayout->setAlignment(browse, Qt::AlignBottom);
    vlist->setContentsMargins(1,1,1,1);
    this->setLayout(vlist);
    this->layout()->setSizeConstraint( QLayout::SetFixedSize );
    //this->setFixedSize(this->width(),this->height());
}

void UnsavedDialog::setMsg(QString msg){
    infoLabel.setText(msg);
}

void UnsavedDialog::cancel(){
    this->changed = 0;
    this->close();
}
void UnsavedDialog::ok(){
    this->changed = 1;
    this->close();
}

void UnsavedDialog::exit(){
    this->changed = 2;
    this->close();
}

UnsavedDialog::~UnsavedDialog() {
}

