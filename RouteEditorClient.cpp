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
#include "RouteEditorClient.h"
#include "Game.h"
#include <QtWebSockets/QWebSocket>
#include <QDebug>
#include "Route.h"
#include "ParserX.h"
#include "WorldObj.h"
#include "TRnode.h"
#include "TRitem.h"
#include "TS.h"
#include "TerrainLib.h"
#include "Terrain.h"
#include "TSectionDAT.h"
#include "TSection.h"
#include "TrackShape.h"
#include "ClientInfo.h"
#include "QuadTree.h"
#include <QDateTime>
#include <QMessageBox>

RouteEditorClient::RouteEditorClient() {
    QStringList args = Game::serverLogin.split("@");
    if(args.size() < 2){
        qDebug() << "Wrong server address";
        return;
    }
    url = "ws://"+args[1];
    args = args[0].split(":");
    username = args[0];
    if(args.size() > 1)
        password = args[1];
    username.truncate(12);
    for (int i = 0; i < username.size(); ++i) {
        if (!username[i].isLetterOrNumber())
           username.remove(i--,1);
    }
    m_webSocket = new QWebSocket();
    qDebug() << "WebSocket server:" << url;
    connect(m_webSocket, &QWebSocket::connected, this, &RouteEditorClient::onConnected);
    connect(m_webSocket, &QWebSocket::disconnected, this, &RouteEditorClient::close);
    //connect(m_webSocket, &QWebSocket::error, this, &RouteEditorClient::error);
}

RouteEditorClient::RouteEditorClient(const RouteEditorClient& orig) {
}

RouteEditorClient::~RouteEditorClient() {
}

void RouteEditorClient::connectNow() {
    m_webSocket->open(QUrl(url));
}

void RouteEditorClient::onConnected() {
    qDebug() << "WebSocket connected";
    connect(m_webSocket, &QWebSocket::textMessageReceived, this, &RouteEditorClient::processTextMessage);
    connect(m_webSocket, &QWebSocket::binaryMessageReceived, this, &RouteEditorClient::processBinaryMessage);
    srand (QDateTime::currentMSecsSinceEpoch());
    int uu = rand() % 99999;
    //username = "user"+QString::number(uu);
    sendUtf16Message(QString("login ( ")+username+" "+password+" )\n");
}
//! [onConnected]

void RouteEditorClient::sendTextMessage(QString msg) {
    m_webSocket->sendTextMessage(msg);
}

void RouteEditorClient::sendUtf16Message(QString msg) {
    QByteArray data;
    QTextStream out(&data);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << msg;
    out.flush();
    m_webSocket->sendBinaryMessage(data);
}

//! [onTextMessageReceived]

void RouteEditorClient::processTextMessage(QString message) {
    qDebug() << "Message received:" << message;
    //m_webSocket.close();

}
//! [onTextMessageReceived]

void RouteEditorClient::close() {
    qDebug() << "WebSocket ddconnected";
    qApp->quit();
}

void RouteEditorClient::processBinaryMessage(QByteArray message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    //qDebug() << "Binary Message received:";
    //if (pClient) {
    //    pClient->sendBinaryMessage(message);
    //}
    FileBuffer* data = new FileBuffer((unsigned char*) message.data(), message.size());
    if (data->isBOM())
        readUtf16Message(pClient, data);
    else if (data->data[0] == 'B')
        readBinaryMessage(pClient, data);
    //delete data;
}

void RouteEditorClient::readBinaryMessage(QWebSocket *client, FileBuffer* data) {
    data->get(); // get B;
    int token = data->getInt();
    int x, z;
    Terrain *t;
    QuadTree *qt;
    qDebug() << "Token: " << token <<" : "<< TS::IdName[token];
    switch (token) {
        case TS::TSRE_Requested_TD_File:
            qDebug() << TS::IdName[TS::TSRE_Requested_TD_File];
            data->getInt();
            data->get();
            x = data->getInt();
            z = data->getInt();
            qDebug() << x << z;
            qt = Game::terrainLib->getQuadTreeDetailed();
            qt->loadTD(x, z, data);
            break;
        case TS::TSRE_Requested_TD_Lo_File:
            qDebug() << TS::IdName[TS::TSRE_Requested_TD_Lo_File];
            data->getInt();
            data->get();
            x = data->getInt();
            z = data->getInt();
            qDebug() << x << z;
            qt = Game::terrainLib->getQuadTreeDistant();
            qt->loadTD(x, z, data);
            break;
        case TS::TSRE_Requested_Terrain_tFile:
            qDebug() << TS::IdName[TS::TSRE_Requested_Terrain_tFile];
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
            break;
        case TS::TSRE_Requested_Terrain_RawFile:
            qDebug() << TS::IdName[TS::TSRE_Requested_Terrain_RawFile];
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
            break;
        case TS::TSRE_Requested_Terrain_FtFile:
            qDebug() << TS::IdName[TS::TSRE_Requested_Terrain_FtFile];
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
            break;
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
            t->Terrain::loadTFile(data);
            t->refresh();
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
            t->refresh();
            break;
        default:
            qDebug() << "undefined token";
            break;
    }
}

void RouteEditorClient::readUtf16Message(QWebSocket *client, FileBuffer* data) {
    data->toUtf16();
    data->skipBOM();
    QString sh;
    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("set_username")) {
            username = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("auth_fail")) {
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("users_info")) {
            clientUsersList.clear();
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("user")) {
                    QString username = ParserX::GetStringInside(data);
                    if(clientUsersList[username] == NULL)
                        clientUsersList[username] = new ClientInfo();
                    ClientInfo *n = clientUsersList[username];
                    n->username = username;
                    n->X = ParserX::GetNumberInside(data);
                    n->Z = ParserX::GetNumberInside(data);
                    n->x = ParserX::GetNumberInside(data);
                    n->y = ParserX::GetNumberInside(data);
                    n->z = ParserX::GetNumberInside(data);
                    n->lastAction = ParserX::GetStringInside(data);
                    ParserX::SkipToken(data);
                    continue;
                }
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
            continue;
        }
        qDebug() << sh;
        if (sh == ("requested_tile")) {
            Game::currentRoute->updateTileData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("requested_tdb")) {
            Game::currentRoute->loadTdbData(data, "tdb");
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("requested_rdb")) {
            Game::currentRoute->loadTdbData(data, "rdb");
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("requested_trk")) {
            Game::currentRoute->loadTrkData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("requested_td")) {
            Game::currentRoute->loadQuadTreeDetailed(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("requested_td_lo")) {
            Game::currentRoute->loadQuadTreeDistant(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("requested_addon")) {
            Game::currentRoute->ref = new Ref("");
            Game::currentRoute->ref->loadUtf16Data(data, "");
            emit refreshObjLists();
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("requested_tsection")) {
            Game::currentRoute->loadTSectionData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("load_route")) {
            Game::route = ParserX::GetString(data);
            if (Game::checkRoot(Game::root) && (Game::checkRemoteRoute(Game::route))) {
                emit loadRoute();
            } else {
                qApp->quit();
            }
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_worldobj") {
            Game::currentRoute->updateWorldObjData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_tdb_itrnode") {
            qDebug() << Game::trackDB->TDB::getNextItrNode();
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_rdb_itrnode") {
            qDebug() << Game::roadDB->TDB::getNextItrNode();
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_tdb_itritem") {
            qDebug() << ++Game::trackDB->iTRitems;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "increase_rdb_itritem") {
            qDebug() << ++Game::roadDB->iTRitems;
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_tritem_tdb") {
            Game::trackDB->updateTrItemData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_tritem_rdb") {
            Game::roadDB->updateTrItemData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_tracksection") {
            Game::trackDB->updateTrackSectionData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_trackshape") {
            Game::trackDB->updateTrackShapeData(data);
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_trnode_rdb") {
            Game::roadDB->updateTrNodeData(data);
            Game::roadDB->refresh();
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == "update_trnode_tdb") {
            Game::trackDB->updateTrNodeData(data);
            Game::trackDB->refresh();
            ParserX::SkipToken(data);
            continue;
        }
        ParserX::SkipToken(data);
        continue;
    }
}

void RouteEditorClient::updateTrackSectionData(TSection *s){
    qDebug() << "send track section";
    QByteArray outd;
    QTextStream out(&outd);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "update_tracksection ( \n";
    if(s != NULL)
        s->saveToStream(out);
    out << ")\n";
    out.flush();
    m_webSocket->sendBinaryMessage(outd);
}

void RouteEditorClient::updateTrackShapeData(TrackShape *s){
    qDebug() << "send track shape";
    QByteArray outd;
    QTextStream out(&outd);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "update_trackshape ( \n";
    s->saveToStream(out);
    out << ")\n";
    out.flush();
    m_webSocket->sendBinaryMessage(outd);
}

void RouteEditorClient::updateTrackItemData(int iid, int tdbType, TRitem *n) {
    qDebug() << "send tritem";
    QByteArray outd;
    QTextStream out(&outd);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    if (tdbType == 0)
        out << "update_tritem_tdb ( \n";
    else
        out << "update_tritem_rdb ( \n";
    out << "id ( " + QString::number(iid) + " )\n";

    if (n != NULL) {
        if (iid != n->trItemId) {
            qDebug() << "fail: Item id doesn't match " << iid << n->trItemId;
        }
        n->save(&out);
    } else {
        out << "remove ( )\n";
    }
    out << ")\n";
    out.flush();
    m_webSocket->sendBinaryMessage(outd);
}

void RouteEditorClient::updateTrackNodeData(int nid, int tdbType, TRnode *n) {
    qDebug() << "send trnode";
    QByteArray outd;
    QTextStream out(&outd);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    if (tdbType == 0)
        out << "update_trnode_tdb ( \n";
    else
        out << "update_trnode_rdb ( \n";
    out << "id ( " + QString::number(nid) + " )\n";
    if (n != NULL)
        n->saveToStream(out, nid);
    else
        out << "remove ( )\n";
    out << ")\n";
    out.flush();
    m_webSocket->sendBinaryMessage(outd);
}

void RouteEditorClient::updateWorldObjData(WorldObj *o) {
    qDebug() << "send wobj";
    QByteArray outd;
    QTextStream out(&outd);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "update_worldobj (\n";
    out << "x ( " + QString::number(o->x) + " )\n";
    out << "z ( " + QString::number(o->y) + " )\n";
    //some temporary fix
    if (o->loaded == false) {
        out << "remove ( ) \n";
    }
    bool tl = o->loaded;
    o->loaded = true;
    o->save(&out);
    o->loaded = tl;
    out << ")\n";
    out.flush();
    m_webSocket->sendBinaryMessage(outd);
}

void RouteEditorClient::updateTerrainHeightmap(Terrain *t) {
    if (t != NULL) {
        qDebug() << "send terrain rawfile";
        QByteArray outd;
        QDataStream out(&outd, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::LittleEndian);
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        out << (qint8) 'B';
        out << TS::TSRE_Terrain_RawFile;
        out << (qint32) 0; //should be size in bytes;
        out << (qint8) 0;
        out << (qint32) t->mojex;
        out << (qint32) t->mojez;
        t->saveRAWfileToStreamFloat(out);
        out.unsetDevice();
        m_webSocket->sendBinaryMessage(outd);
    }
}

void RouteEditorClient::updateTerrainTFile(Terrain *t) {
    if (t != NULL) {
        qDebug() << "send terrain tfile";
        QByteArray outd;
        QDataStream out(&outd, QIODevice::WriteOnly);
        out.setByteOrder(QDataStream::LittleEndian);
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        out << (qint8) 'B';
        out << TS::TSRE_Terrain_tFile;
        out << (qint32) 0; //should be size in bytes;
        out << (qint8) 0;
        out << (qint32) t->mojex;
        out << (qint32) t->mojez;
        t->saveTfileToStream(out);
        out.unsetDevice();
        m_webSocket->sendBinaryMessage(outd);
    }
}

void RouteEditorClient::updatePointerPosition(int X, int Z, float x, float y, float z){
    //qDebug() << "send position";
    QByteArray outd;
    QTextStream out(&outd);
    out.setCodec("UTF-16");
    out.setGenerateByteOrderMark(true);
    out << "update_pointer_position ( ";
    out << QString::number(X) << " ";
    out << QString::number(Z) << " ";
    out << QString::number(x) << " ";
    out << QString::number(y) << " ";
    out << QString::number(z) << " ";
    out << ")\n";
    out.flush();
    m_webSocket->sendBinaryMessage(outd);
}