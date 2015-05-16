/* 
 * File:   loadWindow.h
 * Author: Goku
 *
 * Created on 16 maja 2015, 01:22
 */

#ifndef LOADWINDOW_H
#define	LOADWINDOW_H

#include <QtWidgets>

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
    QPushButton *exit;
    QLineEdit *nowaTrasa;
    QWidget* nowa;
    bool newRoute = false;
};

#endif	/* LOADWINDOW_H */

