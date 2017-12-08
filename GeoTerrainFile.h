/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GEOTERRAINFILE_H
#define	GEOTERRAINFILE_H

#include <QString>

class QImage;

class GeoTerrainFile {
public:
    GeoTerrainFile();
    virtual ~GeoTerrainFile();
    
    virtual bool load(int lat, int lon);
    virtual void draw(QImage* &image);
    virtual bool isLoaded();
    virtual float getHeight(float lat, float lon);
    QString pathid;
private:

};

#endif	/* GEOTERRAINFILE_H */

