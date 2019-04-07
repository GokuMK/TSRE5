/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPEVIEWERWINDOW_H
#define	SHAPEVIEWERWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <QVector>

class ShapeViewerGLWidget;
class ShapeHierarchyWindow;
class ShapeTexturesWindow;
class Camera;
class Consist;
class EngLib;
class Eng;
class SFile;
class AboutWindow;
class ShapeViewerNavigatorWidget;
class EngInfoWidget;
class ConInfoWidget;
class ShapeInfoWidget;

class ShapeViewerWindow : public QMainWindow {
    Q_OBJECT
public:
    ShapeViewerWindow();
    virtual ~ShapeViewerWindow();
    void loadFile(QString path);
    
public slots:
    void about();
    void openFileEnabled();
    void updateTextureInfo();
    
signals:
    void stopUpdateTimer();

private:
    ShapeViewerGLWidget *glShapeWidget;
    ShapeHierarchyWindow *hierarchyWindow;
    ShapeTexturesWindow *texturesWindow;
    AboutWindow* aboutWindow;
    ShapeViewerNavigatorWidget* navigatorWidget;
    EngInfoWidget* engInfo;
    ShapeInfoWidget* shapeInfo;
    ConInfoWidget* conInfo;
    
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *view3dMenu;
    QMenu *engMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;

    QAction *fSave;
    QAction *fNew;
    QAction *fExit;

    QAction *aboutAction;
    
    Camera *engCamera;
    EngLib* englib;
    
    QString currentItemType;
    
    Consist *currentCon = NULL;
    Eng *currentEng = NULL;
    SFile *currentShape = NULL;
};

#endif	/* SHAPEVIEWERWINDOW_H */

