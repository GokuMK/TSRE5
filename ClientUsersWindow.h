/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef CLIENTUSERSWINDOW_H
#define CLIENTUSERSWINDOW_H

#include <QtWidgets>
#include <QMap>

class ClientInfo;
class PreciseTileCoordinate;

class ClientUsersWindow : public QWidget {
    Q_OBJECT
public:
    
    ClientUsersWindow(QWidget* parent);
    virtual ~ClientUsersWindow();
    
public slots:
    void show();
    void hideEvent(QHideEvent *e);
    void refreshList();
    //void jumpRequestReceived(PreciseTileCoordinate *c);
    
signals:
    void windowClosed();
    //void jumpTo(PreciseTileCoordinate *c);
    //void selectObject(GameObj *o);
    
private:
    QHash<int, QBrush> brushes;
    QTreeWidget usersList;
    //ErrorMessageProperties *properties;
};

#endif /* CLIENTUSERSWINDOW_H */

