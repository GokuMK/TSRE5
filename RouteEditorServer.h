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

class QWebSocketServer;
class QWebSocket;
class Route;

class RouteEditorServer : public QObject {
    Q_OBJECT
public:
    RouteEditorServer(int port);
    RouteEditorServer(const RouteEditorServer& orig);
    virtual ~RouteEditorServer();

signals:
    void closed();

private slots:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
    Route *route = NULL;
    bool loadRoute();
};

#endif /* ROUTEEDITORSERVER_H */

