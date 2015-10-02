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
    void paintColorToolEnabled();
    void paintTexToolEnabled();
    void pickTexToolEnabled();
    void setTexToolEnabled();
    void putTexToolEnabled();
    void chooseColorEnabled();
    void updateTexPrev();
    void setBrushTextureId(int val);
    // brush
    void setBrushSize(int val);
    void setBrushSize(QString val);
    void setBrushAlpha(int val);
    void setBrushAlpha(QString val);
    void setFheight(QString val);
    // embarkment
    void setEsize(int val);
    void setEsize(QString val);
    void setEemb(int val);
    void setEemb(QString val);
    void setEcut(int val);
    void setEcut(QString val);
    void setEradius(int val);
    void setEradius(QString val);
    
signals:
    void enableTool(QString name);
    void setPaintBrush(Brush* brush);
    
private:
    Brush* paintBrush;
    
    QPixmap* texPreview;
    QLabel* texPreviewLabel;
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
};

#endif	/* TERRAINTOOLS_H */

