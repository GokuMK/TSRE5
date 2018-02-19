/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPETEMPLATES_H
#define	SHAPETEMPLATES_H

#include <QVector>
#include <QHash>
#include <QMap>
#include <QString>

class FileBuffer;

struct ShapeTemplateElement {
public:
    enum ElementType {
        NONE = 0,
        TIE = 1,
        RAIL = 2,
        BALLAST = 3
    };
    enum ElementId {
        EMPTY = 0,
        DEFAULT = 1
    };
    QString name;
    QString shape;
    QString texture;
    ElementType type = NONE;
    ElementId id = EMPTY;
   

    void load(FileBuffer *data);
};

struct ShapeTemplate {
public:
    enum TemplateType {
        NONE = 0,
        DEFAULT = 1,
        TRACK = 2,
        ROAD = 3
    };
    TemplateType type = NONE;
    QString name;
    //QHash<TemplateType, Element*> elements;
    //QHash<TemplateType, Element*> tie;
    //QHash<TemplateType, Element*> rail;
    // QHash<TemplateType, Element*> ballast;
    QHash<QString, ShapeTemplateElement*> elements;
    //ShapeTemplateElement *tie = NULL;
    //ShapeTemplateElement *ballast = NULL;
    //ShapeTemplateElement *rail = NULL;

};

class ShapeTemplates {
public:

    QMap<QString, ShapeTemplate*> templates;

    ShapeTemplates();
    virtual ~ShapeTemplates();
private:

};

#endif	/* SHAPETEMPLATES_H */

