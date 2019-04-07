/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPETEXTUREINFO_H
#define	SHAPETEXTUREINFO_H

#include <QString>

class ShapeTextureInfo {
public:
    QString shapeName;
    QString textureName;
    QString resolution;
    QString format;
    QString loaded;
    bool enabled = false;
    bool loading = false;
    int textureId = 0;
    
    ShapeTextureInfo();
    ShapeTextureInfo(const ShapeTextureInfo& orig);
    virtual ~ShapeTextureInfo();
private:

};

#endif	/* SHAPETEXTUREINFO_H */

