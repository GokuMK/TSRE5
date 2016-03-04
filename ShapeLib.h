#ifndef SHAPELIB_H
#define	SHAPELIB_H

#include <unordered_map>
#include <QString>

class SFile;

class ShapeLib {
public:
    int jestshape = 0;
    std::unordered_map<int, SFile*> shape;
    ShapeLib();
    ShapeLib(const ShapeLib& orig);
    virtual ~ShapeLib();
    void reset();
    void delRef(int texx);
    void addRef(int texx);
    int addShape(QString path, QString name);
    int addShape(QString path, QString name, QString texPath);
private:

};

#endif	/* SHAPELIB_H */

