/* 
 * File:   TerrainTools.h
 * Author: Goku
 *
 * Created on 1 czerwca 2015, 20:39
 */

#include <QtWidgets>
#include "Route.h"

#ifndef TERRAINTOOLS_H
#define	TERRAINTOOLS_H

class Brush;

class TerrainTools : public QWidget{
    Q_OBJECT

public:
    TerrainTools(QString name);
    virtual ~TerrainTools();
    
public slots:
    void heightToolEnabled();
    void paintToolEnabled();
    void pickTexToolEnabled();
    void setTexToolEnabled();
    void putTexToolEnabled();
    void chooseColorEnabled();
    void updateTexPrev();
    void setBrushSize(int val);
    void setBrushAlpha(int val);
    void setBrushTextureId(int val);
    
signals:
    void enableTool(QString name);
    void setPaintBrush(Brush* brush);
    
private:
    Brush* paintBrush;
    QPixmap* texPreview;
    QLabel* texPreviewLabel;
    QPushButton* colorw;
};

#endif	/* TERRAINTOOLS_H */

