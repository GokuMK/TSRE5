#ifndef SHAPELIB_H
#define	SHAPELIB_H

#include <unordered_map>
#include <QString>

class SFile;

class ShapeLib {
public:
    static int jestshape;
    static std::unordered_map<int, SFile*> shape;
    ShapeLib();
    ShapeLib(const ShapeLib& orig);
    virtual ~ShapeLib();
    static void reset();
    static void delRef(int texx);
    static void addRef(int texx);
    static int addShape(QString path, QString name);
    static int addShape(QString path, QString name, QString texPath);
private:

};

#endif	/* SHAPELIB_H */

