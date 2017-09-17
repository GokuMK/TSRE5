/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACTIVITYEVENTACTIONPROPERTIES_H
#define	ACTIVITYEVENTACTIONPROPERTIES_H

#include <QtWidgets>
#include <QMap>
#include "ActivityEvent.h"

class PreciseTileCoordinate;

class ActivityEventProperties : public QWidget {
    Q_OBJECT
public:
    ActivityEventProperties(QWidget* parent);
    virtual ~ActivityEventProperties();
    void showEvent(ActivityEvent *e);
    void setEventList(QMap<int, QString> eventNames);
    
public slots:
    void outcomeListSelected(QListWidgetItem* item);
    void outcomeActoionListSelected(QString item);
    void cOutcomeEventSelected(QString val);
    void cActionTypeSelected(QString item);
    void bActionSidingSelected();
    void bAddOutcomeSelected();
    void bRemoveOutcomeSelected();
    void eOutcomeMessageSelected();
    void eActivationLevelSelected();
    void eNameSelected();
    void eTriggeredTextSelected(QString val);
    void eUntriggeredTextSelected(QString val);
    void eNotesSelected(QString val);
    void eTimeSelected(QTime val);
    void bJumpToEventLocationSelected();
    void bPickEventLocationSelected(bool val);
    void msg(QString text, QString val);
    void eLocationRadiusSelected();
    void cLocationStopSelected(int val);
    void cReversableSelected(int val);
    void cAutoContinueLabelSelected(int val);
    void eAutoContinueSelected();
    void bJumpToCarSelected();
    void bRemoveCarSelected();
    void bDescCarSelected();
    void bPickNewEventWagonToolSelected();
    void eActionSpeedSelected();
        
signals:
    void eventNameChanged(int id);
    void enableTool(QString val);
    void jumpTo(PreciseTileCoordinate* val);
    
private:
    QWidget actionWidget;
    QWidget actionWidgetSpeed;
    QWidget actionWidgetStation;
    QWidget actionWidgetSiding;
    QWidget actionWidgetWagonList;
    QWidget locationWidget;
    QWidget timeWidget;
    QListWidget wagonList;
    
    QMap<int, QWidget*> outcomeProperties;
    
    QLineEdit eName;
    QSpinBox eActivationLevel;
    QLineEdit eTriggeredText;
    QLineEdit eUntriggeredText;
    QLineEdit eNotes;
    QPlainTextEdit eOutcomeMessage;
    QComboBox cOutcome;
    QComboBox cOutcomeEvent;
    QLineEdit eSoundFileName;
    QComboBox cSoundType;
    QLineEdit cWeatherChange;
    QComboBox cActionType;
    QPushButton bActionStationId;
    QLineEdit eActionSiding;
    QPushButton bActionSiding;
    QSpinBox eActionSpeed;
    QTimeEdit eTime;
    //QLineEdit eOutcomeEvent;
    QCheckBox cReversable;
    QCheckBox cAutoContinueLabel;
    QSpinBox eAutoContinue;
    //QLabel lReversable;
    QLineEdit eActionInfo;
    QLineEdit eLocationPosition;
    QSpinBox eLocationRadius;
    QCheckBox cLocationStop;
    QListWidget outcomeList;
    
    ActivityEvent* event = NULL;
    ActivityEvent::Outcome* outcome = NULL;
    
    QMap<QString, QPushButton*> buttonTools;
    PreciseTileCoordinate* coordinate = NULL;
    
    void selctOutcomeOnList(int id);
};

#endif	/* ACTIVITYEVENTACTIONPROPERTIES_H */

