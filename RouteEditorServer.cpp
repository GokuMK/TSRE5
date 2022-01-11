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
#include "RouteEditorServer.h"
//#include "websockets/qwebsocketserver.h"
//#include "websockets/qwebsocket.h"
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include "Game.h"
#include "ShapeLib.h"
#include "Route.h"
#include "EngLib.h"
#include "ParserX.h"
#include "Tile.h"
#include "TerrainLib.h"
#include "Terrain.h"
#include "TS.h"
#include <QSocketNotifier>
#include <iostream>
#include "TSectionDAT.h"
#include "Trk.h"
#include "ClientInfo.h"
#include "TRitem.h"
#include "QuadTree.h"
#include <QTimer>

RouteEditorServer::RouteEditorServer(int port) {
    if(!loadRoute()){
        return;
    }
    m_pWebSocketServer = new QWebSocketServer(
            QStringLiteral("Echo Server"),
            QWebSocketServer::NonSecureMode, this);

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "Echoserver listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
            this, &RouteEditorServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &RouteEditorServer::closed);
    };

    c = new ConsoleThread();
    c->start();
    connect(c, SIGNAL(sendCommand(QString)), this, SLOT(readCommand(QString)));
    
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&RouteEditorServer::update));
    timer->start(100);
}

void RouteEditorServer::update(){
    /*qDebug() << "server update info";
    foreach (ClientInfo *value, clients) {
        if(value == NULL)
            continue;
        qDebug() << value->username << value->X << value->Z << value->x << value->y << value->z << value->lastAction;
    }*/
        
    QByteArray outd;
    QTextStream out(&outd);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "users_info (\n";
    foreach (ClientInfo *value, clients) {
        if(value == NULL)
            continue;
        out << "user ( ";
        out << value->username+" ";
        out << QString::number(value->X)+" ";
        out << QString::number(value->Z)+" ";
        out << QString::number(value->x)+" ";
        out << QString::number(value->y)+" ";
        out << QString::number(value->z)+" ";
        out << ParserX::AddComIfReq(value->lastAction) +" ";
        out << ")\n";
    }
    out << ")\n";
    out.flush();
    sendMessageToClients(NULL, outd);
}

ConsoleThread::ConsoleThread(){
    cin = new QTextStream(stdin);
}

ConsoleThread::~ConsoleThread(){

}

void ConsoleThread::run(){
    while(true){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        consoleCommand = cin->readLine();
        emit sendCommand(consoleCommand);
    }
}

void RouteEditorServer::readCommand(QString val){
    if (val == "quit") {
        qApp->quit();
    } else if (val == "save") {
        route->save();
    }
}

bool RouteEditorServer::loadRoute(){
    Game::currentShapeLib = new ShapeLib();
    Game::currentEngLib = new EngLib();

    route = new Route();
    route->load();
    if (!route->loaded){ 
        return false;
    }
    return true;
}


RouteEditorServer::RouteEditorServer(const RouteEditorServer& orig) {
}

RouteEditorServer::~RouteEditorServer() {
    m_pWebSocketServer->close();
    qDeleteAll(clients.begin(), clients.end());
    if(c != NULL)
        c->quit();
}


void RouteEditorServer::onNewConnection(){
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();
    qDebug() << "New User Connected";
    connect(pSocket, &QWebSocket::textMessageReceived, this, &RouteEditorServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &RouteEditorServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &RouteEditorServer::socketDisconnected);

    
    clients[pSocket] = new ClientInfo();
    clients[pSocket]->socket = pSocket;
    clients[pSocket]->lastAction = "Waiting for authentication";
}

void RouteEditorServer::processTextMessage(QString message){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "Text Message received:" << message;
    //if (pClient) {
    //    pClient->sendTextMessage(message);
    //}
}

void RouteEditorServer::readUtf16Message(QWebSocket *client, QByteArray &message, FileBuffer* data){
    data->toUtf16();
    data->skipBOM();
    QString sh;
    if(clients[client] == NULL){
        qDebug() << "fatal error client null";
        return;
    }
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {

        if (sh == "update_pointer_position"){
            clients[client]->X = ParserX::GetNumberInside(data);
            clients[client]->Z = ParserX::GetNumberInside(data);
            clients[client]->x = ParserX::GetNumberInside(data);
            clients[client]->y = ParserX::GetNumberInside(data);
            clients[client]->z = ParserX::GetNumberInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        
        qDebug() << sh;
        
        if (sh == "login"){
            clients[client]->username = ParserX::GetStringInside(data);
            foreach (ClientInfo *value, clients) {
                if(value == NULL)
                    continue;
                if(value->socket == client)
                    continue;
                if(value->username == clients[client]->username){
                    srand (QDateTime::currentMSecsSinceEpoch());
                    int uu = rand() % 999999;
                    clients[client]->username = "user"+QString::number(uu);
                    this->sendUtf16Message(client, "set_username ( "+clients[client]->username+" )");
                    break;
                }
            }
            for (int i = 0; i < clients[client]->username.size(); ++i) {
                if (!clients[client]->username[i].isLetterOrNumber())
                   clients[client]->username.remove(i--,1);
            }
            clients[client]->password = ParserX::GetStringInside(data);
            clients[client]->lastAction = "Logged in";
            QString msg = "load_route ( \""+Game::route+"\" )";
            this->sendUtf16Message(client, msg);
            qDebug() << clients[client]->username << " Logged in";
            ParserX::SkipToken(data);
            continue;
        }
        
        if (sh == "request_addons") {
            clients[client]->lastAction = "Requested Addons ";
            QByteArray outd;
            QTextStream out(&outd);
            out.setCodec("UTF-16");
            out.setGenerateByteOrderMark(true);
            out << "requested_addon (\n";
            route->ref->saveToStream(&out);
            out << ")\n";
            out.flush();
            client->sendBinaryMessage(outd);
        }
        
        if (sh == "request_tile") {
            int x = ParserX::GetNumber(data);
            int z = ParserX::GetNumber(data);
            clients[client]->lastAction = "Requested world tile "+QString::number(x)+" "+QString::number(-z);
            //qDebug() << x << z;
            if(route->tile[x*10000+z] != NULL){
                //qDebug() << "send tile";
                QByteArray outd;
                QTextStream out(&outd);
                out.setCodec("UTF-16");
                out.setGenerateByteOrderMark(true);
                out << "requested_tile (\n";
                out << "x ( "+QString::number(x)+" )\n";
                out << "z ( "+QString::number(z)+" )\n";
                route->tile[x*10000+z]->saveToStream(out);
                out << ")\n";
                out.flush();
                client->sendBinaryMessage(outd);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_terrain_qt") {
            QByteArray outd;
            QTextStream out(&outd);
            out.setCodec("UTF-16");
            out.setGenerateByteOrderMark(true);
            out << "requested_td (\n";
            Game::terrainLib->saveQtToStream(out);
            out << ") \n";
            out << "requested_td_lo (\n";
            Game::terrainLib->saveQtLoToStream(out);
            out << ") \n";
            out.flush();
            client->sendBinaryMessage(outd);
            client->flush();
            // send qt files
            QuadTree *qt = Game::terrainLib->getQuadTreeDetailed();
            if(qt != NULL){
                QHashIterator<int, QuadTree::TdFile*> i(qt->td);
                while (i.hasNext()) {
                    i.next();
                    QByteArray outd2;
                    QDataStream out2(&outd2, QIODevice::WriteOnly);
                    out2.setByteOrder(QDataStream::LittleEndian);
                    out2.setFloatingPointPrecision(QDataStream::SinglePrecision);
                    out2 << (qint8)'B';

                    out2 << TS::TSRE_Requested_TD_File;
                    out2 << (qint32)0; //should be size in bytes;
                    out2 << (qint8)0;
                    out2 << (qint32)i.value()->x;
                    out2 << (qint32)i.value()->y;
                    qDebug() << "sending qt";
                    qt->saveTD(i.value()->x, i.value()->y, &out2);
                    out2.unsetDevice();
                    client->sendBinaryMessage(outd2);
                }
            }
            qt = Game::terrainLib->getQuadTreeDistant();
            if(qt != NULL){
                QHashIterator<int, QuadTree::TdFile*> i(qt->td);
                while (i.hasNext()) {
                    i.next();
                    QByteArray outd2;
                    QDataStream out2(&outd2, QIODevice::WriteOnly);
                    out2.setByteOrder(QDataStream::LittleEndian);
                    out2.setFloatingPointPrecision(QDataStream::SinglePrecision);
                    out2 << (qint8)'B';

                    out2 << TS::TSRE_Requested_TD_Lo_File;
                    out2 << (qint32)0; //should be size in bytes;
                    out2 << (qint8)0;
                    out2 << (qint32)i.value()->x;
                    out2 << (qint32)i.value()->y;
                    qDebug() << "sending qtl";
                    qt->saveTD(i.value()->x, i.value()->y, &out2);
                    out2.unsetDevice();
                    client->sendBinaryMessage(outd2);
                }
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_terrain_tfile") {
            int x = ParserX::GetNumber(data);
            int z = ParserX::GetNumber(data);
            clients[client]->lastAction = "Requested terrain T file "+QString::number(x)+" "+QString::number(-z);
            //qDebug() << x << z;
            Terrain *t = Game::terrainLib->getTerrainByXY(x, z, true);
            if(t != NULL){
                //qDebug() << "send terrain tfile";
                QByteArray outd;
                QDataStream out(&outd, QIODevice::WriteOnly);
                out.setByteOrder(QDataStream::LittleEndian);
                out.setFloatingPointPrecision(QDataStream::SinglePrecision);
                out << (qint8)'B';
                out << TS::TSRE_Requested_Terrain_tFile;
                out << (qint32)0; //should be size in bytes;
                out << (qint8)0;
                out << (qint32)x;
                out << (qint32)z;
                t->saveTfileToStream(out);
                out.unsetDevice();
                client->sendBinaryMessage(outd);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_terrain_yfile") {
            int x = ParserX::GetNumber(data);
            int z = ParserX::GetNumber(data);
            clients[client]->lastAction = "Requested terrain Height Map "+QString::number(x)+" "+QString::number(-z);
            //qDebug() << x << z;
            Terrain *t = Game::terrainLib->getTerrainByXY(x, z, true);
            if(t != NULL){
                //qDebug() << "send terrain rawfile";
                QByteArray outd;
                QDataStream out(&outd, QIODevice::WriteOnly);
                out.setByteOrder(QDataStream::LittleEndian);
                out.setFloatingPointPrecision(QDataStream::SinglePrecision);
                out << (qint8)'B';
                out << TS::TSRE_Requested_Terrain_RawFile;
                out << (qint32)0; //should be size in bytes;
                out << (qint8)0;
                out << (qint32)x;
                out << (qint32)z;
                t->saveRAWfileToStreamFloat(out);
                out.unsetDevice();
                client->sendBinaryMessage(outd);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_terrain_ffile") {
            int x = ParserX::GetNumber(data);
            int z = ParserX::GetNumber(data);
            clients[client]->lastAction = "Requested terrain F file "+QString::number(x)+" "+QString::number(-z);
            //qDebug() << x << z;
            Terrain *t = Game::terrainLib->getTerrainByXY(x, z, true);
            if(t != NULL){
                //qDebug() << "send terrain ffile";
                QByteArray outd;
                QDataStream out(&outd, QIODevice::WriteOnly);
                out.setByteOrder(QDataStream::LittleEndian);
                out.setFloatingPointPrecision(QDataStream::SinglePrecision);
                out << (qint8)'B';
                out << TS::TSRE_Requested_Terrain_FtFile;
                out << (qint32)0; //should be size in bytes;
                out << (qint8)0;
                out << (qint32)x;
                out << (qint32)z;
                //t->saveFfileToStream(out);
                out.unsetDevice();
                client->sendBinaryMessage(outd);
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_worldobj") {
            WorldObj *obj = route->updateWorldObjData(data);
            if(obj != NULL)
            clients[client]->lastAction = "Modified world object "+QString::number(obj->x)+" "+QString::number(-obj->y)+" "+QString::number(obj->UiD);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_tdb") {
            clients[client]->lastAction = "Requested Track Database ";
                //qDebug() << "send tdb";
                QByteArray outd;
                QTextStream out(&outd);
                out.setCodec("UTF-16");
                out.setGenerateByteOrderMark(true);
                out << "requested_tdb (\n";
                Game::trackDB->saveToStream(out);
                out << ")\n";
                out.flush();
                client->sendBinaryMessage(outd);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_rdb") {
            clients[client]->lastAction = "Requested Road Database ";
                //qDebug() << "send rdb";
                QByteArray outd;
                QTextStream out(&outd);
                out.setCodec("UTF-16");
                out.setGenerateByteOrderMark(true);
                out << "requested_rdb (\n";
                Game::roadDB->saveToStream(out);
                out << ")\n";
                out.flush();
                client->sendBinaryMessage(outd);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_tdb_itrnode") {
            int n = Game::trackDB->getNextItrNode();
            clients[client]->lastAction = "Increased TrackNode count " + QString::number(n);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_rdb_itrnode") {
            int n = Game::roadDB->getNextItrNode();
            clients[client]->lastAction = "Increased TrackNode count " + QString::number(n);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_tdb_itritem") {
            int n = ++Game::trackDB->iTRitems;
            clients[client]->lastAction = "Increased TrackItem count "+QString::number(n);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_rdb_itritem") {
            int n = ++Game::roadDB->iTRitems;
            clients[client]->lastAction = "Increased TrackItem count "+QString::number(n);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_trnode_tdb") {
            int n = Game::trackDB->updateTrNodeData(data);
            clients[client]->lastAction = "Modified TrackNode " + QString::number(n);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_trnode_rdb") {
            int n = Game::roadDB->updateTrNodeData(data);
            clients[client]->lastAction = "Modified TrackNode " + QString::number(n);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_tritem_tdb") {
            TRitem *n = Game::trackDB->updateTrItemData(data);
            clients[client]->lastAction = "Modified TrackItem " + QString::number(n->trItemId);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_tritem_rdb") {
            TRitem *n = Game::roadDB->updateTrItemData(data);
            clients[client]->lastAction = "Modified TrackItem " + QString::number(n->trItemId);
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_tracksection") {
            Game::trackDB->updateTrackSectionData(data);
            clients[client]->lastAction = "Modified TrackSection ";
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_trackshape") {
            Game::trackDB->updateTrackShapeData(data);
            clients[client]->lastAction = "Modified TrackShape ";
            sendMessageToClients(client, message);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_trk") {
            clients[client]->lastAction = "Requested TRK file ";
                //qDebug() << "send trk";
                QByteArray outd;
                QTextStream out(&outd);
                out.setCodec("UTF-16");
                out.setGenerateByteOrderMark(true);
                out << "requested_trk (\n";
                Game::currentRoute->trk->saveToStream(out);
                out << ")\n";
                out.flush();
                client->sendBinaryMessage(outd);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "request_tsection") {
            clients[client]->lastAction = "Requested TrackSection Database ";
                //qDebug() << "send tsection";
                QByteArray outd;
                QTextStream out(&outd);
                out.setCodec("UTF-16");
                out.setGenerateByteOrderMark(true);
                out << "requested_tsection (\n";
                Game::trackDB->tsection->saveRouteToStream(out);
                out << ")\n";
                out.flush();
                client->sendBinaryMessage(outd);
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
        continue;
    }
}

void RouteEditorServer::sendMessageToClients(QWebSocket *client, QByteArray &message){
    //foreach (QWebSocket *value, clients.keys()) {
    //    qDebug() << value;
    //}
    foreach (ClientInfo *value, clients) {
    //for (int i = 0; i < m_clients.size(); ++i) {
        if(value == NULL)
            continue;
        if(value->socket == client){
            continue;
        } 
        value->socket->sendBinaryMessage(message);
    }
}

void RouteEditorServer::readBinaryMessage(QWebSocket *client, QByteArray &message, FileBuffer* data) {
    data->get(); // get B;
    int token = data->getInt();
    int x, z;
    Terrain *t;
    switch (token) {
        case TS::TSRE_Terrain_tFile:
            qDebug() << TS::IdName[TS::TSRE_Terrain_tFile];
            data->getInt();
            data->get();
            x = data->getInt();
            z = data->getInt();
            qDebug() << x << z;
            t = Game::terrainLib->getTerrainByXY(x, z);
            if (t == NULL) {
                qDebug() << "fail, terrain null";
                break;
            }
            t->loadTFile(data);
            t->setModified();
            sendMessageToClients(client, message);
            break;
        case TS::TSRE_Terrain_RawFile:
            qDebug() << TS::IdName[TS::TSRE_Terrain_RawFile];
            data->getInt();
            data->get();
            x = data->getInt();
            z = data->getInt();
            qDebug() << x << z;
            t = Game::terrainLib->getTerrainByXY(x, z);
            if (t == NULL) {
                qDebug() << "fail, terrain null";
                break;
            }
            t->loadRAWFile(data);
            t->setModified();
            sendMessageToClients(client, message);
            break;
        case TS::TSRE_Terrain_FtFile:
            qDebug() << TS::IdName[TS::TSRE_Terrain_FtFile];
            data->getInt();
            data->get();
            x = data->getInt();
            z = data->getInt();
            qDebug() << x << z;
            t = Game::terrainLib->getTerrainByXY(x, z);
            if (t == NULL) {
                qDebug() << "fail, terrain null";
                break;
            }
            t->loadFFile(data);
            t->setModified();
            break;
        default:
            qDebug() << "undefined token";
            break;
    }
}

void RouteEditorServer::processBinaryMessage(QByteArray message){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    //qDebug() << "Binary Message received:";
    //if (pClient) {
    //    pClient->sendBinaryMessage(message);
    //}
    FileBuffer* data = new FileBuffer((unsigned char*)message.data(), message.size());
    if (data->isBOM())
        readUtf16Message(pClient, message, data);
    else if (data->data[0] == 'B')
        readBinaryMessage(pClient, message, data);
    //delete data;
}

void RouteEditorServer::socketDisconnected(){
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected:" << pClient;
    if (pClient) {
        clients.remove(pClient);
        //clients[pClient] = NULL;
        pClient->deleteLater();
    }
}

void RouteEditorServer::sendUtf16Message(QWebSocket *client, QString msg) {
    QByteArray data;
    QTextStream out(&data);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << msg;
    out.flush();
    client->sendBinaryMessage(data);
}