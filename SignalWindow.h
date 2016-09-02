/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIGNALWINDOW_H
#define	SIGNALWINDOW_H

#include <QtWidgets>

class SignalObj;

class SignalWindow : public QDialog {
    Q_OBJECT

public:
    SignalWindow();
    virtual ~SignalWindow();
    void showObj(SignalObj* obj);
    
public slots:
    void exitNow();
    void chSubEnabled(int i);
    void bLinkEnabled(int i);
    
private:
    static const int maxSubObj = 30;
    QLineEdit name;
    QLineEdit description;
    QCheckBox chSub[maxSubObj];
    QPushButton bSub[maxSubObj];
    QLineEdit dSub[maxSubObj];
    QGridLayout vSub[maxSubObj];
    QWidget wSub[maxSubObj];
    QSignalMapper signalsChSect;
    QSignalMapper signalsLinkButton;
    SignalObj* sobj;
};

#endif	/* SIGNALWINDOW_H */

