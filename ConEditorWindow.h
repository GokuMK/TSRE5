#ifndef CONEDITORWINDOW_H
#define	CONEDITORWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QMainWindow>

class EngListWidget;
class ConListWidget;
class ConUnitsWidget;
class GlShapeWidget;
class QScrollBar;
class Camera;
class Consist;
class EngLib;
class Eng;
class AboutWindow;

class ConEditorWindow : public QMainWindow {
    Q_OBJECT

public:
    ConEditorWindow();
    virtual ~ConEditorWindow();
    
public slots:
    void engListSelected(int id);
    void conListSelected(int id);
    void conSliderValueChanged(int val);
    void viewConList(bool show);
    void viewEngList1(bool show);
    void viewEngList2(bool show);
    void viewConUnits(bool show);
    void viewEngView(bool show);
    void viewConView(bool show);
    void about();
    void conUnitSelected(int uid);
    void saveCurrentConsist();
    void refreshCurrentCon();
    void addToConSelected(int id, int count);
    void newConsist();
    void cFileNameSelected(QString n); 
    void cDisplayNameSelected(QString n); 
    void cReverseSelected();
    void cDurabilitySelected(double val);
    void vResetShapeViewSelected();
    
signals:
    void showEng(QString path, QString name);
    void showCon(int id);

private:
    void setCurrentEng(int id);
    
    EngListWidget *eng1;
    EngListWidget *eng2;
    ConListWidget *con1;
    ConUnitsWidget *units;
    GlShapeWidget *glShapeWidget;
    GlShapeWidget *glConWidget;
    QScrollBar *conSlider;
    
    AboutWindow* aboutWindow;
    
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QMenu *consistMenu;
    QAction *cReverse;
    QAction *fSave;
    QAction *fNew;
    QAction *fExit;
    QAction *vConList;
    QAction *vEngList1;
    QAction *vEngList2;
    QAction *vConUnits;
    QAction *vEngView;
    QAction *vConView;
    QAction *vResetShapeView;
    QAction *aboutAction;
    
    QWidget *conInfo;
    QWidget *engInfo;
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
};

#endif	/* CONEDITORWINDOW_H */

