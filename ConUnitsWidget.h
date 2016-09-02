/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CONUNITSWIDGET_H
#define	CONUNITSWIDGET_H

#include <QWidget>
#include <QtWidgets>

class Consist;
class EngLib;
class ConUnitsWidget : public QWidget {
    Q_OBJECT
public:
    ConUnitsWidget();
    virtual ~ConUnitsWidget();
    void setCon(Consist * c);
    Consist* con;
    EngLib* englib;

public slots:
    void itemsSelected(QListWidgetItem* it);
    void bDelReleased();
    void bUpReleased();
    void bDownReleased();
    void bFlipReleased();
    
signals:
    void selected(int id);
    void refreshItem();
        
private:
    QListWidget items;
    QLineEdit count;
};
#endif	/* CONUNITSWIDGET_H */

