/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ENGLISTWIDGET_H
#define	ENGLISTWIDGET_H

#include <QWidget>
#include <QtWidgets>

class EngLib;

class EngListWidget : public QWidget {
    Q_OBJECT
public:
    EngListWidget();
    virtual ~EngListWidget();
    void fillEngList();
    void fillEngList(QString engFilter, QString couplingFilter, QString searchFilter);
    //EngLib* englib;
    
public slots:
    void itemsSelected();
    void addBegButtonSelected();
    void addCurButtonSelected();
    void addEndButtonSelected();
    void addRndButtonSelected();
    void fs(QString n);
    bool eventFilter(QObject *obj, QEvent *event);
    
signals:
    void engListSelected(int id);
    void addToConSelected(int id, int pos, int count);
    void addToRandomConsist(int id);
    
private:
    void addBegButtonSelected(int count);
    void addCurButtonSelected(int count);
    void addEndButtonSelected(int count);

    QListWidget items;
    QComboBox engType;
    QComboBox couplingType;
    QPushButton addBegButton;
    QPushButton addEndButton;
    QPushButton addCurButton;
    QPushButton addRandButton;
    QLineEdit totalVal;
    QLineEdit addNum;
    QLineEdit searchBox;
};

#endif	/* ENGLISTWIDGET_H */

