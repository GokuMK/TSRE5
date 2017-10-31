/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */


#ifndef TERRAINWATERWINDOW2_H
#define	TERRAINWATERWINDOW2_H

#include <QtWidgets>

class Terrain;

class TerrainWaterWindow2 : public QWidget {
    Q_OBJECT
public:
    TerrainWaterWindow2(QWidget* parent);
    virtual ~TerrainWaterWindow2();
    void setTerrain(Terrain *t);
    
public slots:
    void eAvgTextEdited(QString val);   
    void eWaterEdited(QString val);    
    
private:
    Terrain *terrain;
    QLineEdit e[17];
    QLineEdit eAvg;
    QLineEdit eSW;
    QLineEdit eSE;
    QLineEdit eNE;
    QLineEdit eNW;
};

#endif	/* TERRAINWATERWINDOW2_H */

