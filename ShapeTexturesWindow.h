/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SHAPETEXTURESWINDOW_H
#define	SHAPETEXTURESWINDOW_H

#include <QWidget>
#include <QtWidgets>

class ShapeTextureInfo;

class ShapeTexturesWindow : public QWidget {
    Q_OBJECT
public:
    ShapeTexturesWindow(QWidget* parent);
    virtual ~ShapeTexturesWindow();
    void clearLists();
    void setTextureList(QHash<int, ShapeTextureInfo*> textureInfoList);
    
public slots:

signals:
    
private:
    QTreeWidget textureList;
};

#endif	/* SHAPETEXTURESWINDOW_H */

