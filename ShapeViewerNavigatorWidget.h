/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPEVIEWERNAVIGATORWIDGET_H
#define	SHAPEVIEWERNAVIGATORWIDGET_H

#include <QWidget>
#include <QtWidgets>
#include <QVector>

class ContentHierarchyInfo;

class ShapeViewerNavigatorWidget : public QWidget {
    Q_OBJECT
public:
    ShapeViewerNavigatorWidget(QWidget* parent);
    virtual ~ShapeViewerNavigatorWidget();
    void listDirectoryFiles(QString filepath);
    void listHierarchy(QVector<ContentHierarchyInfo*> info);
    
private:
    QListWidget dirFiles;
    QLineEdit searchFiles;
    QTreeWidget fileItems;

};

#endif	/* SHAPEVIEWERNAVIGATORWIDGET_H */

