/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef ACELIB_H
#define	ACELIB_H

#include <QThread>
#include "Texture.h"

class AceLib : public QThread
 {
     Q_OBJECT

public:
    AceLib();
    AceLib(const AceLib& orig);
    virtual ~AceLib();
    //static bool LoadACE(Texture* texture);
    Texture* texture;
    static void save(QString path, Texture* t);
private:
    
protected:
     void run();
};

#endif	/* ACELIB_H */

