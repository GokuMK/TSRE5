/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef TERRAINTOOLS_H
#define	TERRAINTOOLS_H

#include <QtWidgets>
#include "Route.h"

class Brush;
class ClickableLabel;
class Texture;

class TerrainTools : public QWidget{
    Q_OBJECT

public:
    TerrainTools(QString name);
    virtual ~TerrainTools();
    
public slots:
    void heightToolEnabled(bool val);
    void paintColorToolEnabled(bool val);
    void paintTexToolEnabled(bool val);
    void pickTexToolEnabled(bool val);
    void putTexToolEnabled(bool val);
    void waterTerrToolEnabled(bool val);
    void drawTerrToolEnabled(bool val);
    void lockTexToolEnabled(bool val);
    void gapsTerrToolEnabled(bool val);
    void waterHeightTileToolEnabled(bool val);
    void fixedTileToolEnabled(bool val);
    void setTexToolEnabled();
    void chooseColorEnabled();
    void updateTexPrev();
    void setBrushTextureId(int val);
    // brush
    void setBrushSize(int val);
    void setBrushSize(QString val);
    void setBrushAlpha(int val);
    void setBrushAlpha(QString val);
    void setFheight(QString val);
    void setHtype(int val);
    // embarkment
    void setEsize(int val);
    void setEsize(QString val);
    void setEemb(int val);
    void setEemb(QString val);
    void setEcut(int val);
    void setEcut(QString val);
    void setEradius(int val);
    void setEradius(QString val);
    void msg(QString text, QString val);
    void texPreviewEnabled(int val);
    
signals:
    void enableTool(QString name);
    void setPaintBrush(Brush* brush);
    
private:
    Brush* paintBrush;
    QVector<QImage> brushShapes;
    int currentBrushShape = -1;
    void nextBrushShape();
    QVector<QPair<int, Texture*>> texLastItems;
    //QVector<QImage
    
    QPixmap* texPreview;
    QPixmap* defaultTexPreview;
    QVector<QPixmap*> texPreviews;
    ClickableLabel* texPreviewLabel;
    QVector<ClickableLabel*> texPreviewLabels;
    QSignalMapper texPreviewSignals;
    
    QPushButton* colorw;
    
    // brush gui
    
    QSlider *sSize;
    QSlider *sIntensity;
    QLineEdit *leSize;
    QLineEdit *leIntensity;
    QLineEdit *fheight;
    QComboBox* hType;
    
    QSlider *sEsize;
    QSlider *sEemb;
    QSlider *sEcut;
    QSlider *sEradius;
    QLineEdit *leEsize;
    QLineEdit *leEemb;
    QLineEdit *leEcut;
    QLineEdit *leEradius;
    
    QMap<QString, QPushButton*> buttonTools;

};

#endif	/* TERRAINTOOLS_H */

