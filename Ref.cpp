#include "Ref.h"
#include <QDebug>
#include "Game.h"
#include "ParserX.h"
#include "ReadFile.h"

Ref::Ref(QString path) {
    loaded = false;

    qDebug() << "Wczytywanie pliku ref: " << path;

    int x, i, j, ii, jj, uu;
    float xx;
    int t;
    QString sh;
    path.replace("//", "/");
    qDebug() << path;
    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly))
        return;
    FileBuffer* data = ReadFile::read(file);

    ParserX::nextLine(data);

    while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
        //qDebug() << sh;
        RefItem item;
        item.type = sh;
        while (!((sh = ParserX::nazwasekcji_inside(data).toLower()) == "")) {
            //qDebug() << sh;
            if(sh == ("class")) {
                item.clas = ParserX::odczytajtc(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("filename")) {
                item.filename = ParserX::odczytajtc(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("align")) {
                item.align = ParserX::odczytajtc(data);
                ParserX::pominsekcje(data);
                continue;
            }
            if(sh == ("description")) {
                item.description = ParserX::odczytajtc(data);
                ParserX::pominsekcje(data);
                continue;
            }
            ParserX::pominsekcje(data);
        }
        if(item.clas != ""){
            refItems[item.clas.toStdString()].push_back(item);
        }
        ParserX::pominsekcje(data);
    }

    //for ( auto it = refItems.begin(); it != refItems.end(); ++it ){
    //    qDebug() << QString::fromStdString(it->first) << " " << it->second.size();
    //  //std::cout << " " << it->first << ":" << it->second;
    //}
}

Ref::Ref() {
    loaded = false;
}

Ref::~Ref() {
}

