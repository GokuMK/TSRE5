/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef ERRORMESSAGEPROPERTIES_H
#define ERRORMESSAGEPROPERTIES_H

#include <QtWidgets>

class ErrorMessage;
class PreciseTileCoordinate;
class GameObj;

class ErrorMessageProperties : public QWidget {
    Q_OBJECT 
public:
    ErrorMessageProperties(QWidget* parent);
    virtual ~ErrorMessageProperties();
    void showMessage(ErrorMessage *msg);
    
public slots:
    void jumpToLocation();
    
    void bSelectReleased();
    
signals:
    void jumpTo(PreciseTileCoordinate *c);
    void selectObject(GameObj *o);
    
protected:
    ErrorMessage *currentMessage = NULL;
    QLabel lMessage;
    QLineEdit eMessage;
    QLabel lAction;   
    QLabel lLocation;
    QLineEdit eLocation;
    QPushButton bLocation;
    QPlainTextEdit eAction;
    
    QPushButton bSelect;

};

#endif /* ERRORMESSAGEPROPERTIES_H */

