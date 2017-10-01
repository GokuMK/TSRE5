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
    float restrictedSpeedZoneStart[4];
    float restrictedSpeedZoneEnd[4];
    int failedSignal = -1;
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
    void setParentActivity(Activity *a);
    void setModified(bool val);
    bool isUnSaved();
    int getSelectedElementId();
    bool getElementPosition(int id, float *posTW);
    void pushContextMenuActions(QMenu *menu);
    QString getParentName();
    void render(GLUU* gluu, float * playerT, int renderMode, int index);
    
public slots:
    void menuToggleDirection();
    
signals:
        
private:
    Activity *parentActivity = NULL;
    bool modified = false;
    int selectionValue = 0;
};

#endif	/* ACTIVITYOBJECT_H */

