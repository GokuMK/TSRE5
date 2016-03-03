#ifndef ENGLIB_H
#define	ENGLIB_H

#include <unordered_map>
#include <QString>

class Eng;

class EngLib {
public:
    static int jesteng;
    static std::unordered_map<int, Eng*> eng;
    EngLib();
    virtual ~EngLib();
    static int addEng(QString path, QString name);
    static int loadAll(QString gameRoot);
private:

};

#endif	/* ENGLIB_H */

