/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef ERRORMESSAGESWINDOW_H
#define ERRORMESSAGESWINDOW_H

#include <QtWidgets>
#include <QMap>

class ErrorMessageProperties;
class PreciseTileCoordinate;
class GameObj;

class ErrorMessagesWindow : public QWidget {
    Q_OBJECT
public:
    ErrorMessagesWindow(QWidget* parent);
    virtual ~ErrorMessagesWindow();
    void refreshErrorList();
    
public slots:
    void show();
    void hideEvent(QHideEvent *e);
    void errorListSelected(QTreeWidgetItem* item, int column);
    void jumpRequestReceived(PreciseTileCoordinate *c);
    void selectRequestReceived(GameObj *o);
    
signals:
    void windowClosed();
    void jumpTo(PreciseTileCoordinate *c);
    void selectObject(GameObj *o);
    
private:
    QTreeWidget errorList;
    ErrorMessageProperties *properties;
    
};

#endif /* ERRORMESSAGESWINDOW_H */

