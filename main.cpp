/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QtCore>
#include <QFile>
#include <QTextStream>
#include <QPalette>
#include <QColor>
#include <QStringList>
#include <iostream>
#include "Game.h"
#include "RouteEditorWindow.h"
#include "LoadWindow.h"
#include "CELoadWindow.h"
#include "ShapeViewerWindow.h"
#include "MapWindow.h"
#include "RouteEditorServer.h"
#include "RouteEditorClient.h"
#include "Undo.h"

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

void LoadConEditor(){
    CELoadWindow* ceLoadWindow = new CELoadWindow();
    ceLoadWindow->show();
}

void LoadShapeViewer(QString arg){
    ShapeViewerWindow* shapeWindow = new ShapeViewerWindow();
    if(arg.length() > 0)
        shapeWindow->loadFile(arg);
    shapeWindow->show();
}

void LoadRouteEditor(){
    if(Game::serverLogin.length() > 0)
        Game::ServerMode = true;
    
    if(Game::ServerMode){
        Game::useQuadTree = true;
        Undo::UndoEnabled = false;
        // Create Server Client
        Game::serverClient = new RouteEditorClient();
    }
    
    RouteEditorWindow *window = new RouteEditorWindow();
    if(Game::fullscreen){
        window->setWindowFlags(Qt::CustomizeWindowHint);
        window->setWindowState(Qt::WindowMaximized);
    } else {
        window->resize(1280, 800);
    }
        
    if(!Game::ServerMode){
        LoadWindow *loadWindow = new LoadWindow();
        QObject::connect(window, SIGNAL(exitNow()), loadWindow, SLOT(exitNow()));
        QObject::connect(loadWindow, SIGNAL(showMainWindow()), window, SLOT(showRoute()));

        if(Game::checkRoot(Game::root) && (Game::checkRoute(Game::route) || Game::createNewRoutes)){
            window->showRoute();
        } else {
            loadWindow->show();
        }
    } else {
        QObject::connect(Game::serverClient, SIGNAL(loadRoute()), window, SLOT(showRoute()));
        Game::serverClient->connectNow();
    }
}

void RunRouteEditorServer(int port){
    Game::loadAllWFiles = true;
    Game::gui = false;
    RouteEditorServer *server = new RouteEditorServer(port);
    //..server->run();
}

int main(int argc, char *argv[]){

   // #ifdef  Q_OS_WIN32 
   //     ::ShowWindow( ::GetConsoleWindow(), SW_HIDE ); //hide console window
   // #endif

    QLocale lepsze(QLocale::English);
    //loc.setNumberOptions(lepsze.numberOptions());
    QLocale::setDefault(lepsze);
        
    QSurfaceFormat format;
#ifdef __APPLE__
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
#endif
    //format.setDepthBufferSize(32);
    //format.setStencilBufferSize(8);
    format.setSamples(Game::AASamples);
    //format.set
    format.setSwapInterval(0);
    //format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts, true);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    //QApplication::pr
    QApplication app(argc, argv);
    
    QString workingDir = QDir::currentPath();
    
    if(!Game::UseWorkingDir){
        QDir::setCurrent(QCoreApplication::applicationDirPath());
    }
    
    logFile.setFileName("log.txt");
    logFile.open(QIODevice::WriteOnly);
    logFileOut.setDevice(&logFile);
    qInstallMessageHandler( myMessageOutput );
    
    qDebug() << "workingDir" << workingDir;
    
    Game::load();
    
    //app.set
    Game::PixelRatio = app.devicePixelRatio();
    qDebug() << "devicePixelRatio"<< app.devicePixelRatio();
    app.setStyle(QStyleFactory::create("Fusion"));
    if(!Game::systemTheme){
        //app.setStyle(QStyleFactory::create("Fusion"));
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
        darkPalette.setColor(QPalette::Disabled, QPalette::Text , QColor(153,153,153));
        darkPalette.setColor(QPalette::Disabled, QPalette::WindowText , QColor(153,153,153));
        app.setPalette(darkPalette);
        app.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
        app.setStyleSheet("QPushButton:checked{background-color: #666666;} ");
        Game::StyleMainLabel = "#c4a480";
        Game::StyleGreenButton = "#008800";
        Game::StyleRedButton = "#880000";
        Game::StyleYellowButton = "#888800";
        Game::StyleGreenText = "#55FF55";
        Game::StyleRedText = "#FF5555";
    } else {
        QPalette palette = app.palette();
        palette.setColor(QPalette::Disabled, QPalette::Text , QColor(160,90,64));
        palette.setColor(QPalette::Disabled, QPalette::WindowText , QColor(160,90,64));
        palette.setColor(QPalette::Highlight, QColor(160, 90, 64));
        palette.setColor(QPalette::Inactive, QPalette::HighlightedText, Qt::white);
        app.setPalette(palette);
        app.setStyleSheet("QPushButton:checked{background-color: #e0c0a4;} ");
    }
    
    Game::InitAssets();
    
    //Game::window.resize(1280, 720);
    //window.resize(window.sizeHint());
     
    //int desktopArea = QApplication::desktop()->width() *
    //                  QApplication::desktop()->height();
    //int widgetArea = window.width() * window.height();
    //if (((float)widgetArea / (float)desktopArea) < 1.0f)
    //    window.show();
    //else
    //    window.showMaximized();
    
    QStringList args = app.arguments();
    //Check if file opened with "open in TSRE"
    if(args.count() == 2){
        if(QFileInfo::exists(args[1])){
            args[1] = "--shapeview "+args[1];        }
    }
    if(args.count() > 1){
        qDebug() << "arg1 " << args[1];    
        if(args[1] == "--aceconv"){
            // Run ace converter
            qDebug() << "Run ace converter";
            return app.exec();
        }
        if(args[1] == "--conedit"){
            // Run ace converter
            qDebug() << "Run con editor";
            LoadConEditor();
            return app.exec();
        }
        if(args[1].startsWith("--shapeview")){
            // Run ace converter
            qDebug() << "Run shape viewer";
            LoadShapeViewer(args[1].section(" ",1,-1));
            
            return app.exec();
        }
        if(args[1] == "--play"){
            // Play
            if(args.length() == 3){
                Game::ActivityToPlay = args[2];
            } else if(args.length() == 4){
                Game::route = args[2];
                Game::ActivityToPlay = args[3];
            } else {
                Game::ActivityToPlay = "#";
            }
            qDebug() << "Play" << Game::route << Game::ActivityToPlay;
        }
        if(args[1] == "--server"){
            // Run ace converter
            // Game::route = "bbb";
            Game::checkRoute(Game::route);
            //routeDir = Game::route;
            //trkName = Game::trkName;
            qDebug() << "Run server";
            int port = 65534;
            if(args.size() > 2)
                port = args[2].toInt();
            RunRouteEditorServer(port);
            
            return app.exec();
        }
    }
    // Run route editor
    LoadRouteEditor();

    //MapWindow aaa;
    //aaa.show();
    return app.exec();
 }
