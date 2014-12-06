#ifndef REF_H
#define	REF_H

#include <QString>
#include <unordered_map>

class Ref {
public:
    struct RefItem{
        QString type;
        QString clas;
        QString filename;
        QString align;
        QString description;
        int value = 0;
    };

    std::unordered_map<std::string, std::vector<RefItem>> refItems;
    
    Ref();
    Ref(QString path);
    virtual ~Ref();
    bool loaded;
    RefItem *selected = NULL;
private:
    
};

#endif	/* REF_H */

