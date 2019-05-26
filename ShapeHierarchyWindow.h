/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPEHIERARCHYWINDOW_H
#define	SHAPEHIERARCHYWINDOW_H

#include <QWidget>
#include <QtWidgets>

class ShapeHierarchyInfo;
class SFile;

class ShapeHierarchyWindow : public QWidget {
    Q_OBJECT
public:
    SFile *currentShape = NULL;
    ShapeHierarchyWindow(QWidget* parent);
    virtual ~ShapeHierarchyWindow();
    void clearLists();
    void setHierarchyList(ShapeHierarchyInfo* info);
    
public slots:
    void hierarchyListSelected(QTreeWidgetItem* item, int id);
    
signals:
    
private:
    QTreeWidget hierarchyList;
};

#endif	/* SHAPEHIERARCHYWINDOW_H */

