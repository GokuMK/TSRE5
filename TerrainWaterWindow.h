/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TERRAINWATERWINDOW_H
#define	TERRAINWATERWINDOW_H

#include <QtWidgets>

class TerrainWaterWindow : public QDialog {
    Q_OBJECT
public:
    float WSW;
    float WSE;
    float WNE;
    float WNW;
    QLineEdit eALL;
    QLineEdit eWSW;
    QLineEdit eWSE;
    QLineEdit eWNE;
    QLineEdit eWNW;
    bool changed = false;
    void setWater();
    TerrainWaterWindow();
    virtual ~TerrainWaterWindow();
    
public slots:
    void cancel();
    void ok();
    void eWSWtextEdited(QString val);
    
private:

};

#endif	/* TERRAINWATERWINDOW_H */

