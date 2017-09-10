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

class ActivityEvent;

class ActivityEventActionProperties : public QWidget {
    Q_OBJECT
public:
    ActivityEventActionProperties(QWidget* parent);
    virtual ~ActivityEventActionProperties();
    void showEvent(ActivityEvent *e);
    
private:
    QLineEdit eName;
    QLineEdit eActivationLevel;
    QLineEdit eTriggeredText;
    QLineEdit eUntriggeredText;
    QLineEdit eNotes;
    QPlainTextEdit eOutcomeMessage;
    QComboBox cOutcome;
    QLineEdit eOutcomeEvent;
    QCheckBox cReversable;
    QComboBox cActionType;
    QLineEdit eActionInfo;
    QListWidget outcomeList;
    
    ActivityEvent* event = NULL;
};

#endif	/* ACTIVITYEVENTACTIONPROPERTIES_H */

