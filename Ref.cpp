/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "Ref.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "WorldObj.h"

Ref::Ref(QString path) {
    loaded = false;
    loadFile(path);
}

void Ref::loadFile(QString path){
    qDebug() << "Wczytywanie pliku ref: " << path;

    int x, i, j, ii, jj, uu;
    float xx;
    int t;
    QString sh;
    path.replace("//", "/");
    qDebug() << path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(&file);
    data->toUtf16();
    data->skipBOM();    

    while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
        //qDebug() << sh;
        if (sh == ("simisa@@@@@@@@@@jinx0r1t______")) {
            continue;
        }
        if (sh.startsWith("simis")) {
            continue;
        }
        if (sh == ("include")) {
            QString incPath = ParserX::GetStringInside(data);
            ParserX::SkipToken(data);
            data->insertFile(path + "/" + incPath);
            continue;
        }
        if (sh == ("skip")) {
            ParserX::SkipToken(data);
            continue;
        }
        if (sh == ("comment")) {
            ParserX::SkipToken(data);
            continue;
        }
        RefItem item;
        item.type = sh;
        while (!((sh = ParserX::NextTokenInside(data).toLower()) == "")) {
            //qDebug() << sh;
            if (sh == ("class")) {
                item.clas = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("filename")) {
                item.filename.push_back(ParserX::GetString(data));
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("align")) {
                item.align = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("description")) {
                item.description = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("selectionmethod")) {
                /*SequentialSelection*/
                /*RandomSelection*/
                item.selectionMethod = ParserX::GetString(data);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("randomrotx")) {
                float b = ParserX::GetNumberInside(data);
                float e = ParserX::GetNumberInside(data);
                item.setRandomRotDegX(b, e);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("randomroty")) {
                float b = ParserX::GetNumberInside(data);
                float e = ParserX::GetNumberInside(data);
                item.setRandomRotDegY(b, e);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("randomrotz")) {
                float b = ParserX::GetNumberInside(data);
                float e = ParserX::GetNumberInside(data);
                item.setRandomRotDegZ(b, e);
                ParserX::SkipToken(data);
                continue;
            }
            
            if (sh == ("randomtranslationx")) {
                float b = ParserX::GetNumberInside(data);
                float e = ParserX::GetNumberInside(data);
                item.setRandomTranslationX(b, e);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("randomtranslationy")) {
                float b = ParserX::GetNumberInside(data);
                float e = ParserX::GetNumberInside(data);
                item.setRandomTranslationY(b, e);
                ParserX::SkipToken(data);
                continue;
            }
            if (sh == ("randomtranslationz")) {
                float b = ParserX::GetNumberInside(data);
                float e = ParserX::GetNumberInside(data);
                item.setRandomTranslationZ(b, e);
                ParserX::SkipToken(data);
                continue;
            }
            
            ParserX::SkipToken(data);
        }
        if (item.clas != "") {
            refItems[item.clas.trimmed()].push_back(item);
        }
        ParserX::SkipToken(data);
    }

    selected = NULL;
}

Ref::Ref() {
    loaded = false;
}

Ref::~Ref() {
}

QString Ref::RefItem::getShapePath() {
    return WorldObj::getResPath(this);
}

QString Ref::RefItem::getShapeName(){
    if(currentFilename == "%"){
        currentFilename = getNextShapeName();
    }
    return currentFilename;
}

QString Ref::RefItem::getNextShapeName(){
    if(filename.size() == 0){
        return "";
    }
    if(selectionMethod == ""){
        return filename[0];
    }
    
    int shapeId = 0;
    if(selectionMethod == "SequentialSelection"){
        static int shapeCount = 0;
        shapeId = shapeCount % filename.size();
        shapeCount++;
        return filename[shapeId];
    }
    if(selectionMethod == "RandomSelection"){
        shapeId = (std::rand()%filename.size());
        return filename[shapeId];
    }
    return "";
}

void Ref::RefItem::setRandomRotDegX(float b, float e){
    if(randomTransformation == NULL){
        randomTransformation = new RandomTransformation();
    }
    randomTransformation->rbX = b;
    randomTransformation->reX = e;
}

void Ref::RefItem::setRandomRotDegY(float b, float e){
    if(randomTransformation == NULL){
        randomTransformation = new RandomTransformation();
    }
    randomTransformation->rbY = b;
    randomTransformation->reY = e;
}

void Ref::RefItem::setRandomRotDegZ(float b, float e){
    if(randomTransformation == NULL){
        randomTransformation = new RandomTransformation();
    }
    randomTransformation->rbZ = b;
    randomTransformation->reZ = e;
}

void Ref::RefItem::setRandomTranslationX(float b, float e){
    if(randomTransformation == NULL){
        randomTransformation = new RandomTransformation();
    }
    randomTransformation->tbX = b;
    randomTransformation->teX = e;
}

void Ref::RefItem::setRandomTranslationY(float b, float e){
    if(randomTransformation == NULL){
        randomTransformation = new RandomTransformation();
    }
    randomTransformation->tbY = b;
    randomTransformation->teY = e;
}

void Ref::RefItem::setRandomTranslationZ(float b, float e){
    if(randomTransformation == NULL){
        randomTransformation = new RandomTransformation();
    }
    randomTransformation->tbZ = b;
    randomTransformation->teZ = e;
}