#ifndef ENGLIB_H
#define	ENGLIB_H

#include <unordered_map>
#include <QString>

class Eng;

class EngLib {
public:
    int jesteng = 0;
    std::unordered_map<int, Eng*> eng;
    EngLib();
    virtual ~EngLib();
    int addEng(QString path, QString name);
    int getEngByPathid(QString pathid);
    int loadAll(QString gameRoot);
private:

};

#endif	/* ENGLIB_H */

