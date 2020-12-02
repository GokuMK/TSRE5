/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "RouteEditorServer.h"
//#include "websockets/qwebsocketserver.h"
//#include "websockets/qwebsocket.h"
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include "Game.h"
#include "ShapeLib.h"
#include "Route.h"
#include "EngLib.h"

RouteEditorServer::RouteEditorServer(int port) {
    
    if(!loadRoute())
        return;
    
    m_pWebSocketServer = new QWebSocketServer(
            QStringLiteral("Echo Server"),
            QWebSocketServer::NonSecureMode, this);

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Echoserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
            this, &RouteEditorServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &RouteEditorServer::closed);
    };
}

bool RouteEditorServer::loadRoute(){
    Game::currentShapeLib = new ShapeLib();
    Game::currentEngLib = new EngLib();

    route = new Route();
    if (!route->loaded){ 
        return false;
    }
    return true;
}


RouteEditorServer::RouteEditorServer(const RouteEditorServer& orig) {
}

RouteEditorServer::~RouteEditorServer() {
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}


void RouteEditorServer::onNewConnection(){
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    qDebug() << "aaa222";
    connect(pSocket, &QWebSocket::textMessageReceived, this, &RouteEditorServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &RouteEditorServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &RouteEditorServer::socketDisconnected);

    m_clients << pSocket;
}

void RouteEditorServer::processTextMessage(QString message){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Message received:" << message;
    if (pClient) {
        pClient->sendTextMessage(message);
    }
}

void RouteEditorServer::processBinaryMessage(QByteArray message){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Binary Message received:" << message;
    if (pClient) {
        pClient->sendBinaryMessage(message);
    }
}

void RouteEditorServer::socketDisconnected(){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}