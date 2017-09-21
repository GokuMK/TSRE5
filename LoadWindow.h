/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef LOADWINDOW_H
#define	LOADWINDOW_H

#include <QtWidgets>

class PreciseTileCoordinate;
class IghCoordinate;
class LatitudeLongitudeCoordinate;

class LoadWindow : public QWidget {
    Q_OBJECT

public:
    LoadWindow();
    
public slots:
    void exitNow();
    void handleBrowseButton();
    void routeLoad();
    void setNewRoute();
    void setLoadRoute();
signals:
    void showMainWindow();
private:
    void listRoutes();
    QListWidget routeList;
    QPushButton *browse;
    QPushButton *load;
    QPushButton *neww;
    QPushButton *exit;
    QLineEdit *nowaTrasa;
    QWidget* nowa;
    bool newRoute = false;
    IghCoordinate* igh = NULL;
    LatitudeLongitudeCoordinate* latlon = NULL;
    PreciseTileCoordinate* aCoords = NULL;
    void downloadTemplateRoute(QString path);
};

#endif	/* LOADWINDOW_H */

