#include "ToolBox.h"


ToolBox::ToolBox(QString name)
    : QWidget(){
    //QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
    //QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
    //QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));

    QPushButton *selectTool = new QPushButton("Select", this);
    QPushButton *placeTool = new QPushButton("Place new", this);
    //radio1->setChecked(true);
    
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(&refClass);
    vbox->addWidget(&refList);
    vbox->addWidget(selectTool);
    vbox->addWidget(placeTool);
    //vbox->addWidget(radio2);
    //vbox->addWidget(radio3);
    vbox->addStretch(1);
    this->setLayout(vbox);
    
    QObject::connect(&refClass, SIGNAL(activated(QString)),
                      this, SLOT(refClassSelected(QString)));
    
    QObject::connect(&refList, SIGNAL(itemClicked(QListWidgetItem*)),
                      this, SLOT(refListSelected(QListWidgetItem*)));
    
    QObject::connect(selectTool, SIGNAL(released()),
                      this, SLOT(selectToolEnabled()));
    
    QObject::connect(placeTool, SIGNAL(released()),
                      this, SLOT(placeToolEnabled()));
}

ToolBox::~ToolBox() {
}

void ToolBox::routeLoaded(Ref* ref){
    this->ref = ref;

    for ( auto it = ref->refItems.begin(); it != ref->refItems.end(); ++it ){
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        refClass.addItem(QString::fromStdString(it->first), QVariant(QString::fromStdString(it->first)));
      //std::cout << " " << it->first << ":" << it->second;
    }
}

void ToolBox::refClassSelected(const QString & text){
    //qDebug() << "Bbbb " << text;
    refList.clear();
    for (int it = 0; it < ref->refItems[text.toStdString()].size(); ++it ){
        refList.addItem(ref->refItems[text.toStdString()][it].description);
        //qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
        //refClass.addItem(QString::fromStdString(it->first), QVariant(QString::fromStdString(it->first)));
      //std::cout << " " << it->first << ":" << it->second;
    }
}

void ToolBox::refListSelected(QListWidgetItem * item){
    //refList.addItem(
    try{
        ref->selected = &ref->refItems[refClass.currentText().toStdString()][refList.currentRow()];

    } catch(const std::out_of_range& oor){
        ref->selected = NULL;
    }
}

void ToolBox::selectToolEnabled(){
    emit enableTool("selectTool");
}

void ToolBox::placeToolEnabled(){
    emit enableTool("placeTool");
}