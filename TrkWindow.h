/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TRKWINDOW_H
#define	TRKWINDOW_H

#include <QtWidgets>

class Trk;

class TrkWindow : public QDialog {
    Q_OBJECT
public:
    TrkWindow();
    virtual ~TrkWindow();
    Trk *trk = NULL;
    
    int exec();
    
signals:    
    
public slots:
    void bokEnabled();
    void bcancelEnabled();
    void envNameEnabled(QString item);
    
private:
    QLabel imageGraphic;
    QLabel imageLoad;
    
    QLineEdit idName;
    QLineEdit displayName;
    QPlainTextEdit description;
    QLineEdit routeName;
    QComboBox electrified;
    //d int mountains;
    QDoubleSpinBox overheadWireHeight;
    QDoubleSpinBox speedLimit;
    QDoubleSpinBox terrainErrorScale;
    QLineEdit startTileX;
    QLineEdit startTileZ;
    QLineEdit startpX;
    QLineEdit startpZ;
    QComboBox milepostUnitsKilometers;
    QDoubleSpinBox maxLineVoltage;
    QDoubleSpinBox tempRestrictedSpeed;
    QComboBox envName;
    QLineEdit envValue;
    QComboBox iList;
    QPushButton iCopy;
    QPushButton iPaste;
};

#endif	/* TRKWINDOW_H */

