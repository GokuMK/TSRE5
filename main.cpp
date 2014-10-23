#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QtCore>
#include <iostream>

#include "window.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    const char symbols[] = { 'I', 'E', '!', 'X' };
    QString output = QString("[%1] %2").arg( symbols[type] ).arg( msg );
    std::cout << output.toStdString() << std::endl;
    if( type == QtFatalMsg ) abort();
}

int main(int argc, char *argv[]){
     qInstallMessageHandler( myMessageOutput );
     QApplication app(argc, argv);
     Window window;
     window.resize(window.sizeHint());
     
     int desktopArea = QApplication::desktop()->width() *
                      QApplication::desktop()->height();
     int widgetArea = window.width() * window.height();
     if (((float)widgetArea / (float)desktopArea) < 0.75f)
         window.show();
     else
         window.showMaximized();

     return app.exec();
 }
