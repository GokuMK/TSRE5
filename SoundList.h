/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef SOUNDLISTITEM_H
#define	SOUNDLISTITEM_H

#include <QString>
#include <unordered_map>
#include "QHash"

struct SoundListItem {
    enum SoundType {
        Source = 0,
        Region = 1
    };
    SoundType type;
    QString name;
    QString file1;
    QString file2;
    int id;
    
    SoundListItem();
    virtual ~SoundListItem();
private:

};

class SoundList {
public:
    QHash<QString, SoundListItem*> sources;
    QHash<QString, SoundListItem*> regions;
    bool sourcesLoaded = false;
    bool regionsLoaded = false;
    void loadSoundSources(QString path);
    void loadSoundRegions(QString path);
private:

};

#endif	/* SOUNDLISTITEM_H */

