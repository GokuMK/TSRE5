#ifndef NEWROUTEWINDOW_H
#define	NEWROUTEWINDOW_H

#include <QtWidgets>

class NewRouteWindow  : public QDialog {
    Q_OBJECT
public:
    NewRouteWindow();
    virtual ~NewRouteWindow();
    QLineEdit name;
    QLineEdit lat;
    QLineEdit lon;
    bool changed = false;
    
public slots:
    void cancel();
    void ok();

};

#endif	/* NEWROUTEWINDOW_H */

