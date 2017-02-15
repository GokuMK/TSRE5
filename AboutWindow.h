/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ABOUTWINDOW_H
#define	ABOUTWINDOW_H

#include <QtWidgets>

class AboutWindow : public QWidget {
    Q_OBJECT

public:
    AboutWindow(QWidget* parent);
    AboutWindow(const AboutWindow& orig);
    virtual ~AboutWindow();
    
public slots:
    void exitNow();
private:

};

#endif	/* ABOUTWINDOW_H */

