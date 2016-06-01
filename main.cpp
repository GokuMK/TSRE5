#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QtCore>
#include <iostream>
#include "Game.h"
#include "window.h"
#include "LoadWindow.h"
#include "MapWindow.h"
#include <QFile>
#include <QTextStream>
QFile logFile;
QTextStream logFileOut;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){
    const char symbols[] = { 'I', 'E', '!', 'X' };
    QString output = QString("[%1] %2").arg( symbols[type] ).arg( msg );
    if(Game::consoleOutput)
        std::cout << output.toStdString() << std::endl;
    logFileOut << output << "\n";
    logFileOut.flush();
    logFile.flush();
    
    if( type == QtFatalMsg ) abort(); 
}

int main(int argc, char *argv[]){
    logFile.setFileName("log.txt");
    logFile.open(QIODevice::WriteOnly);
    logFileOut.setDevice(&logFile);

    qInstallMessageHandler( myMessageOutput );
    QLocale lepsze(QLocale::English);
    //loc.setNumberOptions(lepsze.numberOptions());
    QLocale::setDefault(lepsze);
    
    QApplication app(argc, argv);
    QSurfaceFormat format;
    //format.setVersion(3, 2);
    //format.setProfile(QSurfaceFormat::CoreProfile);
    //format.setDepthBufferSize(32);
    //format.setStencilBufferSize(8);
    //format.setSamples(2);
    //format.set
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
    Game::load();
    
    if(!Game::systemTheme){
        app.setStyle(QStyleFactory::create("Fusion"));
        QPalette darkPalette;
        darkPalette.setColor(QPalette::Window, QColor(53,53,53));
        darkPalette.setColor(QPalette::WindowText, Qt::white);
        darkPalette.setColor(QPalette::Base, QColor(25,25,25));
        darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
        darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
        darkPalette.setColor(QPalette::ToolTipText, Qt::white);
        darkPalette.setColor(QPalette::Text, Qt::white);
        darkPalette.setColor(QPalette::Button, QColor(53,53,53));
        darkPalette.setColor(QPalette::ButtonText, Qt::white);
        darkPalette.setColor(QPalette::BrightText, Qt::red);
        darkPalette.setColor(QPalette::Link, QColor(240, 130, 0));
        darkPalette.setColor(QPalette::Highlight, QColor(240, 130, 0));
        darkPalette.setColor(QPalette::HighlightedText, Qt::black);
        app.setPalette(darkPalette);
        app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }
    
    //Game::window.resize(1280, 720);
    //window.resize(window.sizeHint());
     
    //int desktopArea = QApplication::desktop()->width() *
    //                  QApplication::desktop()->height();
    //int widgetArea = window.width() * window.height();
    //if (((float)widgetArea / (float)desktopArea) < 1.0f)
    //    window.show();
    //else
    //    window.showMaximized();

    //Game::loadWindow.show();
    
    if(app.arguments().count() > 1){
        qDebug() << "arg1 " << app.arguments().at(1);    
        if(app.arguments().at(1) == "--aceconv"){
            // Run ace converter
            qDebug() << "Run ace converter";
            return app.exec();
        }
        if(app.arguments().at(1) == "--conedit"){
            // Run ace converter
            qDebug() << "Run con editor";
            Game::loadConEditor();
            return app.exec();
        }
    }
    // Run route editor
    Game::loadRouteEditor();
    //MapWindow aaa;
    //aaa.show();
    return app.exec();
 }
