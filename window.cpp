#include <QtWidgets>

#include "glwidget.h"
#include "window.h"

Window::Window() {
    groupBox = new ToolBox("ToolBox");
    naviBox = new NaviBox();
    glWidget = new GLWidget;
    
    QWidget* box = new QWidget();
    box->setMaximumWidth(250);
    box->setMinimumWidth(250);
    QVBoxLayout *mainLayout2 = new QVBoxLayout;
    mainLayout2->addWidget(groupBox);
    mainLayout2->addWidget(naviBox);
    mainLayout2->setAlignment(naviBox, Qt::AlignBottom);
    mainLayout2->setContentsMargins(0,0,0,0);
    box->setLayout(mainLayout2);
    
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(box);
    //mainLayout->addWidget(groupBox);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(1,1,1,1);
    
    setWindowTitle(tr("TSRE5"));
    
    QObject::connect(glWidget, SIGNAL(routeLoaded(Route*)),
                      groupBox, SLOT(routeLoaded(Route*)));
    
    QObject::connect(groupBox, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
    
    QObject::connect(naviBox, SIGNAL(jumpTo(int, int)),
                      glWidget, SLOT(jumpTo(int, int)));
    
    QObject::connect(glWidget, SIGNAL(itemSelected(int)),
                      groupBox, SLOT(itemSelected(int)));
}

void Window::keyPressEvent(QKeyEvent *e) {

    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
