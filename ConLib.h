#ifndef CONLIB_H
#define	CONLIB_H

#include <unordered_map>
#include <QString>

class Consist;

class ConLib {
public:
    static int jestcon;
    static std::unordered_map<int, Consist*> con;
    ConLib();
    virtual ~ConLib();
    static int addCon(QString path, QString name);
    static int loadAll(QString gameRoot);
private:

};
#endif	/* CONLIB_H */

