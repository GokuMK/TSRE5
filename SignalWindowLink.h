/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SIGNALWINDOWLINK_H
#define	SIGNALWINDOWLINK_H

#include <QtWidgets>

class SignalObj;

class SignalWindowLink : public QDialog {
    Q_OBJECT

public:
    QLineEdit from;
    QLineEdit to;
    bool changed = false;
    SignalWindowLink();
    virtual ~SignalWindowLink();

    
public slots:
    void cancel();
    void ok();
    
private:

};

#endif	/* SIGNALWINDOW_H */

