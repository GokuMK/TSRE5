/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef REF_H
#define	REF_H

#include <QString>
#include <QVector>
#include <QMap>

class FileBuffer;
class QTextStream;

class Ref {
public:
    struct RandomTransformation {
        float rbX = 0, reX = 0;
        float rbY = 0, reY = 0;
        float rbZ = 0, reZ = 0;
        float tbX = 0, teX = 0;
        float tbY = 0, teY = 0;
        float tbZ = 0, teZ = 0;
    };
    struct RefItem {
        QString type;
        QString clas;
        QVector<QString> filename;
        QString currentFilename = "%";
        QString align;
        QString description;
        QString selectionMethod;
        long long int value = 0;
        unsigned int staticFlags = 0;
        
        RandomTransformation* randomTransformation = NULL;
        
        QString getShapePath();
        QString getShapeName();
        QString getNextShapeName();
        void setRandomRotDegX(float b, float e);
        void setRandomRotDegY(float b, float e);
        void setRandomRotDegZ(float b, float e);
        void setRandomTranslationX(float b, float e);
        void setRandomTranslationY(float b, float e);
        void setRandomTranslationZ(float b, float e);
    };

    QMap<QString, QVector<RefItem>> refItems;
    
    Ref();
    Ref(QString path);
    virtual ~Ref();
    void loadFile(QString path);
    void loadUtf16Data(FileBuffer *data, QString path);
    void saveToStream(QTextStream *out);
    bool loaded;
    RefItem *selected = NULL;
    
private:
    
};

#endif	/* REF_H */

