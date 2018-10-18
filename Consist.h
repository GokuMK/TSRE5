/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef CONSIST_H
#define	CONSIST_H

#include <QString>
#include <unordered_map>
#include <QVector>
#include "GameObj.h"

class Eng;
class TextObj;
class FileBuffer;
class QTextStream;
class GLUU;
class Activity;
class SimpleHud;

class Consist : public GameObj {
public:
    static std::unordered_map<int, TextObj*> txtNumbers;
    static int lastTxtNumbersColor;
    static TextObj * txtEngineE;
    static TextObj * txtEngineD;
    static TextObj * txtEngineS;
    static TextObj * txtEngineF;
    static TextObj * txtEngineW;
    static TextObj * txtEngineT;
    struct EngItem{
        bool flip = false;
        int uid = 0;
        int eng = -1;
        Eng *engPointer = NULL;
        float pos = 0;
        float conLength = 0;
        int type = 0;
        TextObj * txt = NULL;
        QString epath;
        QString ename;
    };
    Consist();
    Consist(Consist * con, bool fullCopy = true);
    virtual ~Consist();
    int lastTxtColor;
    QString name;
    QString path;
    QString pathid;
    QString conName;
    QString displayName;
    QString showName;
    unsigned int serial = 0;
    float maxVelocity[2];
    int nextWagonUID = 0;
    float durability = 0;
    float conLength = 0;
    float mass = 0;
    float emass = 0;
    int loaded = -1;
    bool isOnTrack = false;
    bool kierunek = false;
    int ref = 0;
    int posInit = false;
    int selectedIdx = -1;
    float textColor[3];
    QVector<EngItem> engItems;
    Consist(QString p, QString n);
    Consist(QString src, QString p, QString n);
    void load();
    bool load(FileBuffer* data);
    void refreshEngData();
    void save();
    void save(QString woff, QTextStream *out);
    bool select(int idx);
    bool unselect();
    void appendEngItem(int id, int pos = 2, bool flip = false);
    int getSelectedEngId();
    void replaceEngItemSelected(int id);
    void replaceEngItemById(int oldId, int newId);
    void deteleSelected();
    void flipSelected();
    void moveLeftSelected();
    void moveRightSelected();
    bool isNewConsist();
    bool isBroken();
    bool isUnSaved();
    void setModified(bool val);
    void setNewConsistFlag();
    void setFileName(QString n);
    void setDisplayName(QString n);
    void reverse();
    void setTextColor(float *bgColor);
    void setDurability(float val);
    void initOnTrack(float *posTXZ, int direction, QMap<int, int> *junctionDirections = NULL);
    bool getWagonWorldPosition(int id, float *posTW);
    void updateSim(float deltaTime);
    void renderHud();
    void getCameraPosition(float *out);
    void renderOnTrack(GLUU* gluu, float * playerT, int selectionColor);
    void render(int selectionColor = 0, bool renderText = false);
    void render(int aktwx, int aktwz, int selectionColor, bool renderText);
    void initPos();
    void setMaxVelocityFixed(bool val);
    bool isMaxVelocityFixed();
    QString getFirstEngName();
    void setTrainSpeed(float val);
    float getTrainSpeed();
    float getTrainDistanceTravelled();

private:
    bool newConsist = false;
    bool modified = false;
    bool defaultValue = false;
    bool maxVelocityFixed = false;
    float trainSpeed = 0.0;
    float trainTotalDistance = 0.0;
    SimpleHud *hud = NULL;
    
    void replaceEngItem(int id, int pos);
};

#endif	/* CONSIST_H */

