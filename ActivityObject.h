/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYOBJECT_H
#define	ACTIVITYOBJECT_H

#include <QString>
#include <QVector>
#include "GameObj.h"

class FileBuffer;
class Consist;
class Traffic;
class QTextStream;
class GLUU;
class Activity;
class QMenu;
class OglObj;

class ActivityObject : public GameObj {
    Q_OBJECT
public:
    enum TypeActivityObject {
        NONE = 0,
        WAGONLIST = 1,
        RESTRICTEDSPEEDZONE = 2,
        FAILEDSIGNAL = 3
    };
    Consist* con = NULL;
    QString objectType;
    TypeActivityObject objectTypeId = NONE;
    float direction = 0;
    unsigned int id = 0;
    float tile[4];
    ActivityObject();
    ActivityObject(const ActivityObject& orig);
    ActivityObject(int tid);
    ~ActivityObject();
    void load(FileBuffer* data);
    void save(QTextStream* out);
    void remove();
    bool select(int value);
    bool unselect();
    void setPosition(int x, int z, float* p);
    void toggleDirection();
    void reverseWagonListUnit();
    void setParentActivity(Activity *a);
    void setFailedSignalData(int id);
    void setSpeedZoneData(float* drawposition = NULL);
    QString getSpeedZoneName();
    void setModified(bool val);
    bool isUnSaved();
    int getId();
    int getSelectedElementId();
    int getFailedSignalId();
    bool getElementPosition(int id, float *posTW);
    void pushContextMenuActions(QMenu *menu);
    QString getParentName();
    void render(GLUU* gluu, float * playerT, int renderMode, int index);
    
public slots:
    void menuToggleDirection();
    void menuToggleReverse();
    
signals:
        
private:
    Activity *parentActivity = NULL;
    bool modified = false;
    int selectionValue = 0;
    
    struct FailedSignalData {
        float* drawPosition = NULL;
        OglObj* pointer3d = NULL;
        OglObj* pointer3dSelected = NULL;
        int init = 0;
        int failedSignal = -1;
        bool getWorldPosition(float *posTW);
        void render(GLUU* gluu, float * playerT, int selectionColor, bool selected = false);
    };
    
    struct SpeedZone {
        float* drawPositionB = NULL;
        float* drawPositionE = NULL;
        OglObj* lineShape = NULL;
        OglObj* pointer3d = NULL;
        OglObj* pointer3dSelected = NULL;
        int selectionValue = 0;
        float start[4];
        float end[4];
        float rotB, rotE;
        int init = 0;
        float trid[6];
        bool getWorldPosition(float *posTW);
        void render(GLUU* gluu, float * playerT, int selectionColor, bool selected = false);
        void makelineShape();
    };
    
    //void renderZone(GLUU* gluu, float * playerT, int selectionColor);
    SpeedZone* speedZoneData = NULL;
    FailedSignalData* failedSignalData = NULL;
};

#endif	/* ACTIVITYOBJECT_H */

