#ifndef ACTLIB_H
#define	ACTLIB_H

#include <unordered_map>
#include <QString>

class Activity;

class ActLib {
public:
    static int jestact;
    static std::unordered_map<int, Activity*> act;
    ActLib();
    virtual ~ActLib();
    static int addAct(QString path, QString name);
    static int loadAll(QString gameRoot);
private:

};

#endif	/* ACTLIB_H */

