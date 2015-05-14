/* 
 * File:   FileFunctions.cpp
 * Author: Goku
 * 
 * Created on 14 maja 2015, 03:03
 */

#include "FileFunctions.h"
#include <QFile>
#include <QDir>
#include <QDebug>

void FileFunctions::copyFiles(QString dirIn, QString dirOut){
    QDir dir(dirIn);
    qDebug() << dirIn;
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        qDebug() << dirFile;
        if(!QDir(dirFile).exists())
            QFile::copy(dirIn +"/"+ dirFile, dirOut +"/"+ dirFile);
    }
}
