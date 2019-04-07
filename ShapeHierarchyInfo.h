/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPEHIERARCHYINFO_H
#define	SHAPEHIERARCHYINFO_H

#include <QString>
#include <QVector>

class ShapeHierarchyInfo {
public:
    struct ShapePart {
        int polyCount = 0;
        int matrixId = 0;
        QString textureName;
        
        ShapePart();
        ShapePart(const ShapePart& orig);
    };
    
    QVector<int> hierarchy;
    QVector<QString> matrices;
    QVector<ShapePart> parts;
    
    ShapeHierarchyInfo();
    ShapeHierarchyInfo(const ShapeHierarchyInfo& orig);
    virtual ~ShapeHierarchyInfo();
private:

};

#endif	/* SHAPEHIERARCHYINFO_H */

