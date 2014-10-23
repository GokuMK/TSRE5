#include <QtWidgets>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    glWidget = new GLWidget;

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);

    setLayout(mainLayout);

    setWindowTitle(tr("TSRE5"));
}

void Window::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "rr";
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}
