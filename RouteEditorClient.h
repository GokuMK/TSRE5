/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ROUTEEDITORCLIENT_H
#define ROUTEEDITORCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QHash>

class QWebSocket;
class FileBuffer;
class WorldObj;
class TRnode;
class TRitem;
class Terrain;
class TrackShape;
class TSection;
class ClientInfo;

class RouteEditorClient : public QObject {
    Q_OBJECT
    
public:
    QHash<QString, ClientInfo*> clientUsersList;
    QString username;
    
    RouteEditorClient();
    RouteEditorClient(const RouteEditorClient& orig);
    virtual ~RouteEditorClient();
    void sendTextMessage(QString msg);
    void sendUtf16Message(QString msg);
    void updateWorldObjData(WorldObj *o);
    void updateTrackNodeData(int nid, int tdbType, TRnode *n);
    void updateTrackItemData(int iid, int tdbType, TRitem *n);
    void updateTrackSectionData(TSection *s);
    void updateTrackShapeData(TrackShape *s);
    void updateTerrainHeightmap(Terrain *t);
    void updateTerrainTFile(Terrain *t);
    void updatePointerPosition(int X, int Z, float x, float y, float z);
    void connectNow();
     
public slots:
    void close();
    void onConnected();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    
signals:
    void loadRoute();
    void refreshObjLists();
    
private:
    QWebSocket * m_webSocket = NULL;
    QString password;
    QString url;
    void readUtf16Message(QWebSocket *client, FileBuffer* data);
    void readBinaryMessage(QWebSocket *client, FileBuffer* data);
};

#endif /* ROUTEEDITORCLIENT_H */

