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

class ActivityEvent;
class ActivityEvent::Outcome;

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
    void bAddOutcomeSelected();
    void bRemoveOutcomeSelected();
    void eOutcomeMessageSelected();
    void eActivationLevelSelected();
    void eNameSelected();
    void eTriggeredTextSelected(QString val);
    void eUntriggeredTextSelected(QString val);
    void eNotesSelected(QString val);
    void eTimeSelected(QTime val);
        
signals:
    
private:
    QWidget actionWidget;
    QWidget locationWidget;
    QWidget timeWidget;
    
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
    QTimeEdit eTime;
    //QLineEdit eOutcomeEvent;
    QCheckBox cReversable;
    QCheckBox cAutoContinueLabel;
    QLineEdit eAutoContinue;
    QLabel lReversable;
    QLineEdit eActionInfo;
    QLineEdit eLocationPosition;
    QLineEdit eLocationRadius;
    QCheckBox cLocationStop;
    QListWidget outcomeList;
    
    ActivityEvent* event = NULL;
    ActivityEvent::Outcome* outcome = NULL;
    
    void selctOutcomeOnList(int id);
};

#endif	/* ACTIVITYEVENTACTIONPROPERTIES_H */

