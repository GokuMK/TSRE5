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
class RandomConsist;

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
    void addToConSelected(int id, int pos, int count);
    void newConsist();
    void cFileNameSelected(QString n); 
    void cDisplayNameSelected(QString n); 
    // microsoft security essentials hangs if these functions have full names
    // on my windows 7 ...
    void f7();//cReverseSelected();
    void f8();//cDurabilitySelected();
    void f4(); //eFindConsistsByEng(); 
    void f0(int id); //addToRandomConsist
    void f1(); //vResetShapeView
    void f2(); //vGetImgShapeView
    void f3(); //vCopyImgShapeView
    void f5(); //vSetColorShapeView
    void f6(); //vSetColorConView
    
signals:
    void showEng(QString path, QString name);
    void showCon(int id);

protected:
    virtual void closeEvent ( QCloseEvent * event );
    
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
    RandomConsist* randomConsist;
    
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *view3dMenu;
    QMenu *engMenu;
    QMenu *helpMenu;
    QMenu *consistMenu;
    QAction *cReverse;
    QAction *fSave;
    QAction *fNew;
    QAction *fExit;
    QAction *eFindCons;
    QAction *vConList;
    QAction *vEngList1;
    QAction *vEngList2;
    QAction *vConUnits;
    QAction *vEngView;
    QAction *vConView;
    QAction *vResetShapeView;
    QAction *vGetImgShapeView;
    QAction *vSetColorShapeView;
    QAction *vSetColorConView;
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
};

#endif	/* CONEDITORWINDOW_H */

