/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CONEDITORWINDOW_H
#define	CONEDITORWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>
#include <QVector>

class EngListWidget;
class ConListWidget;
class ConUnitsWidget;
class ShapeViewerGLWidget;
class QScrollBar;
class Camera;
class Consist;
class EngLib;
class Eng;
class AboutWindow;
class RandomConsist;

class ConEditorWindow : public QMainWindow {
    Q_OBJECT

public:
    ConEditorWindow();
    virtual ~ConEditorWindow();
    
public slots:
    void engListSelected(int id);
    void conListSelected(int id);
    void conListSelected(int aid, int id);
    void conSliderValueChanged(int val);
    void viewConList(bool show);
    void viewEngList1(bool show);
    void viewEngList2(bool show);
    void viewConUnits(bool show);
    void viewEngView(bool show);
    void viewConView(bool show);
    void about();
    void conUnitSelected(int uid);
    void save();
    void refreshCurrentCon();
    void addToConSelected(int id, int pos, int count);
    void newConsist();
    void cFileNameSelected(QString n); 
    void cDisplayNameSelected(QString n); 
    void cReverseSelected();
    void cCloneSelected();
    void cDeleteSelected();
    void cOpenInExternalEditor();
    void cDurabilitySelected();
    void cSaveAsEngSetSelected();
    void eFindConsistsByEng(); 
    void eOpenInExternalEditor(); 
    void eOpenLegacyInExternalEditor(); 
    void eReloadEnabled();
    void addToRandomConsist(int id); 
    void vResetShapeViewSelected(); 
    void vGetImgShapeViewSelected(); 
    void vSaveImgShapeViewSelected();
    void copyImgShapeView();
    void saveImgShapeView();
    void vSetColorShapeViewSelected();
    void vSetColorConViewSelected(); 
    void sLoadEngSetsByDefaultSelected(bool show);
    void engSetAddSelected();
    void engSetFlipAndAddSelected();
    void engSetShowSelected();
    void engSetHideSelected();
    void engSetShowSet(QString n);

signals:
    void showEng(QString path, QString name);
    void showEngSet(int id);
    void showEng(Eng *e);
    void showCon(int id);
    void showCon(int aid, int id);
    
protected:
    virtual void closeEvent ( QCloseEvent * event );
    
private:
    void setCurrentEng(int id, int engSetId = -1);
    void fillCurrentEng(int engSetId);
    void saveCurrentConsist();
    void saveCurrentActivity();
    EngListWidget *eng1;
    EngListWidget *eng2;
    ConListWidget *con1;
    ConUnitsWidget *units;
    ShapeViewerGLWidget *glShapeWidget;
    ShapeViewerGLWidget *glConWidget;
    QScrollBar *conSlider;
    
    AboutWindow* aboutWindow;
    RandomConsist* randomConsist;
    
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *view3dMenu;
    QMenu *engMenu;
    QMenu *settingsMenu;
    QMenu *helpMenu;
    QMenu *consistMenu;
    QAction *cReverse;
    QAction *cClone;
    QAction *cDelete;
    QAction *cOpenInExtEditor;
    QAction *cSaveAsEngSet;
    QAction *fSave;
    QAction *fNew;
    QAction *fExit;
    QAction *eFindCons;
    QAction *eOpenInExtEditor;
    QAction *eOpenLegacyInExtEditor;
    QAction *eReload;
    QAction *vConList;
    QAction *vEngList1;
    QAction *vEngList2;
    QAction *vConUnits;
    QAction *vEngView;
    QAction *vConView;
    QAction *vResetShapeView;
    QAction *vGetImgShapeView;
    QAction *vSaveImgShapeView;
    QAction *vSetColorShapeView;
    QAction *vSetColorConView;
    QAction *sLoadEngSetsByDefault;
    QAction *aboutAction;
    
    QWidget *conInfo;
    QWidget *engInfo;
    QWidget *engSetsWidget;
    QComboBox engSetsList;
    QVector<int> engSets;
    
    Consist *currentCon = NULL;
    Eng *currentEng = NULL;
    Camera *conCamera;
    Camera *engCamera;
    EngLib* englib;
    //EngLib* conEngLib;
    
    QLineEdit eName;
    QLineEdit eFileName;
    QLineEdit eDirName;
    QLineEdit eType;
    QLineEdit eBrakes;
    QLineEdit eCouplings;
    QLineEdit eMass;
    QLineEdit eMaxSpeed;
    QLineEdit eMaxForce;
    QLineEdit eMaxPower;
    QLineEdit eShape;
    QLineEdit eSize;
            
    QLineEdit cFileName;
    QLineEdit cDisplayName;
    QLineEdit cMass;
    QLineEdit cEmass;
    QLineEdit cWmass;
    QLineEdit cLength;
    QLineEdit cUnits;
    QDoubleSpinBox cDurability;
    
    bool loadEngSetsByDefault = true;
};

#endif	/* CONEDITORWINDOW_H */

