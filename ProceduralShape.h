/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef PROCEDURALSHAPE_H
#define	PROCEDURALSHAPE_H

#include <QMap>
#include <QString>
#include "OglObj.h"
#include "TSection.h"

class ObjFile;
class TrackShape;
class GlobalDefinitions;
class ComplexLine;
class ShapeTemplates;
class ShapeTemplateElement;

struct ShapePrimitive {
    float *data;
    ObjFile *templatePtr = NULL;
    int count = 0;
    bool disabled = false;
    float pos[3];
    float matrix[16];
    float quat[4];
    float rotX = 0;
    float rotY = 0;
    float rotZ = 0;
};

class ProceduralShape {
public:
    static bool Loaded;
    static QMap<QString, ObjFile*> Files;
    static void GenShape(QVector<OglObj*> &shape, QVector<TSection> &sections);
    static void GenShape(QVector<OglObj*> &shape, TrackShape* tsh, QMap<int, float> &angles);

private:
    static float Alpha;
    static void Load();
    static ObjFile* GetObjFile(QString name);
    
    static void GenRails(ShapeTemplateElement *stemplate, QVector<OglObj*> &shape, ComplexLine &line);
    static void GenRails(ShapeTemplateElement *stemplate, QVector<OglObj*> &shape, ComplexLine &line, float *sPos, float sAngle, float angleB, float angleE);
    
    static void GenBallast(ShapeTemplateElement *stemplate, QVector<OglObj*> &shape, ComplexLine &line);
    static void GenBallast(ShapeTemplateElement *stemplate, QVector<OglObj*> &shape, ComplexLine &line, float *sPos, float sAngle, float angleB, float angleE);
    
    static void GenTie(ShapeTemplateElement *stemplate, QVector<OglObj*> &shape, ComplexLine &line);
    static void GenTie(ShapeTemplateElement *stemplate, QVector<OglObj*> &shape, ComplexLine &line, float *sPos, float sAngle, float angleB, float angleE);
    
    static void GenAdvancedTie(ShapeTemplateElement *stemplate, QVector<OglObj*> &shape, TrackShape* tsh, QMap<int, float> &angles);
    
    static void PushShapePart(float* &ptr, ObjFile* tFile, float offsetY, float* matrix, float* qrot, float distance = 0);
    static void PushShapePart(float* &ptr, ObjFile* tFile, float offsetY, float* matrix1, float* matrix2, float* qrot, float dist1, float dist2);
    
    static ShapeTemplates *ShapeTemplateFile;
    static GlobalDefinitions* GlobalDefinitionFile;
};

#endif	/* PROCEDURALSHAPE_H */

