/* 
 * File:   PaintTexLib.h
 * Author: Goku
 *
 * Created on 14 maja 2015, 22:23
 */

#ifndef PAINTTEXLIB_H
#define	PAINTTEXLIB_H

#include <QThread>
#include "Texture.h"

class PaintTexLib// : public QThread
 {
 //    Q_OBJECT

public:
    Texture* texture;
    void run();
private:
    
protected:
     
};

#endif	/* PAINTTEXLIB_H */

