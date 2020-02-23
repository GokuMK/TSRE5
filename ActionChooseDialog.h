/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef ACTIONCHOOSEDIALOG_H
#define ACTIONCHOOSEDIALOG_H

#include <QtWidgets>
#include <QHash>

class ActionChooseDialog : public QDialog {
    Q_OBJECT
public:
    QString actionChoosen;
    
    ActionChooseDialog();
    ActionChooseDialog(int buttonCount);
    virtual ~ActionChooseDialog();
    //void setTitle();
    void setInfoText(QString text);
    void pushAction(QString action, QString text);
    
public slots:
    void action(int i);
    
private:
    QHash<int, QString> actions;
    QHash<int, QPushButton*> bok;
    QLabel infoLabel;
    QSignalMapper mapper;
    
    int buttonSize = 10;
    int count = 0;
};

#endif /* ACTIONCHOOSEDIALOG_H */

