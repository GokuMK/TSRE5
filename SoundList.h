#ifndef SOUNDLISTITEM_H
#define	SOUNDLISTITEM_H

#include <QString>
#include <unordered_map>

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
    std::unordered_map<std::string, SoundListItem*> sources;
    std::unordered_map<std::string, SoundListItem*> regions;
    bool sourcesLoaded = false;
    bool regionsLoaded = false;
    void loadSoundSources(QString path);
    void loadSoundRegions(QString path);
private:

};

#endif	/* SOUNDLISTITEM_H */

