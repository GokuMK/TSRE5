/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ShapeTemplates.h"
#include <QString>
#include <QFile>
#include "Game.h"
#include "ReadFile.h"
#include "FileBuffer.h"
#include "ParserX.h"

ShapeTemplates::ShapeTemplates() {
    
    QString resPath = QString("tsre_appdata/")+Game::AppDataVersion+"/procedural/";
    //QString path2 = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/uic60.obj";
    //QString path3 = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/ballast1.obj";
    //QString path3 = QString("tsre_appdata/")+Game::AppDataVersion+"/tracks/ballast2.obj";
    /*if(Files[path1] == NULL)
        Files[path1] = new ObjFile(path1);
    if(Files[path2] == NULL)
        Files[path2] = new ObjFile(path2);
    if(Files[path3] == NULL)
        Files[path3] = new ObjFile(path3);*/
    
    QString path = resPath + "shapetemplates.dat";
    
    QString sh;
    path.replace("//", "/");
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << path << " shape definition file not exist";
        return;
    }

    FileBuffer* data = ReadFile::read(file);
    file->close();
    data->toUtf16();
    data->skipBOM();

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        if (sh == ("templates")) {
            while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                if (sh == ("template")) {
                    QString name = ParserX::GetString(data);
                    templates[name] = new ShapeTemplate();
                    templates[name]->name = name;
                    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
                        if (sh == ("type")) {
                            templates[name]->type = templates[name]->DEFAULT;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("ballast")) {
                            ShapeTemplateElement *e = new ShapeTemplateElement();
                            e->type = e->BALLAST;
                            e->load(data);
                            templates[name]->elements[e->name] = e;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("stretch")) {
                            ShapeTemplateElement *e = new ShapeTemplateElement();
                            e->type = e->STRETCH;
                            e->load(data);
                            templates[name]->elements[e->name] = e;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("point")) {
                            ShapeTemplateElement *e = new ShapeTemplateElement();
                            e->type = e->POINT;
                            e->load(data);
                            templates[name]->elements[e->name] = e;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("rail")) {
                            ShapeTemplateElement *e = new ShapeTemplateElement();
                            e->type = e->RAIL;
                            e->load(data);
                            templates[name]->elements[e->name] = e;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        if (sh == ("tie")) {
                            ShapeTemplateElement *e = new ShapeTemplateElement();
                            e->type = e->TIE;
                            e->load(data);
                            templates[name]->elements[e->name] = e;
                            ParserX::SkipToken(data);
                            continue;
                        }
                        qDebug() << "#Template - undefined token: " << sh;
                        ParserX::SkipToken(data);
                        continue;
                    }
                    ParserX::SkipToken(data);
                    continue;
                }
                qDebug() << "#Templates - undefined token: " << sh;
                ParserX::SkipToken(data);
                continue;
            }
            ParserX::SkipToken(data);
            continue;
            
            continue;
        }
        qDebug() << "#ShapeTemplates - undefined token: " << sh;
        ParserX::SkipToken(data);
        continue;
    }
    delete data;
}

void ShapeTemplateElement::load(FileBuffer* data){
    name = ParserX::GetString(data);
    id = DEFAULT;
    ParserX::GetString(data);
    int shapeCount = ParserX::GetNumber(data);
    for(int i = 0; i < shapeCount; i++)
        shape.push_back(ParserX::GetString(data));
    texture = ParserX::GetString(data);
    yOffset = ParserX::GetNumber(data);
    if(type == RAIL){
        xOffset = ParserX::GetNumber(data);
    }
}

ShapeTemplates::~ShapeTemplates() {
}

