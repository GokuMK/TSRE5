/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ShapeHierarchyInfo.h"

ShapeHierarchyInfo::ShapeHierarchyInfo() {
}

ShapeHierarchyInfo::ShapeHierarchyInfo(const ShapeHierarchyInfo& orig) {
    hierarchy.append(orig.hierarchy);
    matrices.append(orig.matrices);
    parts.append(orig.parts);
}

ShapeHierarchyInfo::ShapePart::ShapePart(){
    
}

ShapeHierarchyInfo::ShapePart::ShapePart(const ShapePart& orig){
    polyCount = orig.polyCount;
    matrixId = orig.matrixId;
    textureName = orig.textureName;
    uid = orig.uid;
    enabled = orig.enabled;
}

ShapeHierarchyInfo::~ShapeHierarchyInfo() {
}

