/* 
 * File:   AboutWindow.h
 * Author: Goku
 *
 * Created on 16 maja 2015, 13:43
 */

#ifndef ABOUTWINDOW_H
#define	ABOUTWINDOW_H

#include <QtWidgets>

class AboutWindow : public QWidget {
    Q_OBJECT

public:
    AboutWindow();
    AboutWindow(const AboutWindow& orig);
    virtual ~AboutWindow();
    
public slots:
    void exitNow();
private:

};

#endif	/* ABOUTWINDOW_H */

