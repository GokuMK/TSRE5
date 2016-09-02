/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CELOADWINDOW_H
#define	CELOADWINDOW_H

#include <QtWidgets>

class PreciseTileCoordinate;
class IghCoordinate;
class LatitudeLongitudeCoordinate;

class CELoadWindow : public QWidget {
    Q_OBJECT

public:
    CELoadWindow();
    
public slots:
    void exitNow();
    void handleBrowseButton();
    void routeLoad();
    void dirSelected();
    
signals:
    void showMainWindow();

private:
    void listRoutes();
    void listInfo();
    QListWidget recentDirs;
    //QListWidget routeList;
    QPushButton *browse;
    QPushButton *load;
    QPushButton *neww;
    QPushButton *exit;
    QLineEdit *nowaTrasa;
    QWidget* nowa;
    QWidget* info;
    QLineEdit conNumber;
    QLineEdit trainsNumber;
};

#endif	/* CELOADWINDOW_H */

