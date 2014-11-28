#include <QtWidgets>

#include "glwidget.h"
#include "window.h"
#include "ToolBox.h"

Window::Window() {
    groupBox = new ToolBox("ToolBox");
    glWidget = new GLWidget;

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);

    setWindowTitle(tr("TSRE5"));
    
    QObject::connect(glWidget, SIGNAL(routeLoaded(Ref*)),
                      groupBox, SLOT(routeLoaded(Ref*)));
    
    QObject::connect(groupBox, SIGNAL(enableTool(QString)),
                      glWidget, SLOT(enableTool(QString)));
}

void Window::keyPressEvent(QKeyEvent *e) {
    qDebug() << "rr";
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
