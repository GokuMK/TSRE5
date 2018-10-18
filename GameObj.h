/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GAMEOBJ_H
#define	GAMEOBJ_H

#include <QString>
#include <QMap>
#include <QObject>

class QMenu;
class QAction;

class GameObj : public QObject {

public:
    enum TypeObj {
        none = 0,
        undefined = 1,
        worldobj = 2,
        terrainobj = 3,
        consistobj = 4,
        tritemobj = 5,
        activitypath = 6,
        activityobj = 7
    };
    
    TypeObj typeObj = this->undefined;
    
    GameObj();
    GameObj(const GameObj& orig);
    virtual ~GameObj();
    
    virtual bool select();
    virtual bool select(int value);
    virtual bool unselect();
    virtual bool isSelected();
    virtual void remove();
    virtual void updateSim(float deltaTime);
    virtual void translate(float px, float py, float pz);
    virtual void rotate(float x, float y, float z);
    virtual void resize(float x, float y, float z);
    virtual void setPosition(int x, int z, float* p);
    virtual void setPosition(float* p);
    virtual void setMartix();
    virtual void renderHud();
    virtual void getCameraPosition(float* out = NULL);
    virtual void pushContextMenuActions(QMenu *menu);
    virtual QString getName();

protected:
    bool selected = false;
    QMap<QString, QAction*> contextMenuActions;
    static QMap<TypeObj, QString> Names;
};

#endif	/* GAMEOBJ_H */

