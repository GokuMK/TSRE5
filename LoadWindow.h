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
};

#endif	/* LOADWINDOW_H */

