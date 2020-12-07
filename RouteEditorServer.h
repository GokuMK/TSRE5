/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ROUTEEDITORSERVER_H
#define ROUTEEDITORSERVER_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QHash>

class QWebSocketServer;
class QWebSocket;
class Route;
class FileBuffer;
class QSocketNotifier;
class QByteArray;
class ClientInfo;

class RouteEditorServer : public QObject {
    Q_OBJECT
public:
    RouteEditorServer(int port);
    RouteEditorServer(const RouteEditorServer& orig);
    virtual ~RouteEditorServer();
    void sendUtf16Message(QWebSocket *client, QString msg);
    
public slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void readCommand();
    void update();
    
signals:
    void closed();

private:
    QWebSocketServer *m_pWebSocketServer;
    QHash<QWebSocket *, ClientInfo*> clients;
    bool m_debug;
    Route *route = NULL;
    bool loadRoute();
    void sendMessageToClients(QWebSocket *client, QByteArray &message);
    void readUtf16Message(QWebSocket *client, QByteArray &message, FileBuffer* data);
    void readBinaryMessage(QWebSocket *client, QByteArray &message, FileBuffer* data);
    QSocketNotifier *m_notifier;
};

#endif /* ROUTEEDITORSERVER_H */

