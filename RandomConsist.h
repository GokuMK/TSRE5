/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef RANDOMCONSIST_H
#define	RANDOMCONSIST_H

#include <QtWidgets>

class Consist;

class RandomConsist : public QWidget {
    Q_OBJECT
public:
    RandomConsist(QWidget *parent);
    virtual ~RandomConsist();
    QListWidget items;
    int changed = 0;
    
public slots:
    void ok();
    void cancel();
    void save();
    void listSelected(QString val);
    
signals:
    void addToConSelected(int id, int pos, int count);
    
private:
    QLineEdit number;
    QLineEdit name;
    QComboBox list;
    
    void loadSets();
    
    QVector<Consist*> sets;
};

#endif	/* RANDOMCONSIST_H */

