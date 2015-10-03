#include "ShapeLib.h"
#include "Game.h"
#include <QDebug>
#include "SFile.h"

int ShapeLib::jestshape;
std::unordered_map<int, SFile*> ShapeLib::shape;

ShapeLib::ShapeLib() {
}

ShapeLib::ShapeLib(const ShapeLib& orig) {
}

ShapeLib::~ShapeLib() {
}

void ShapeLib::reset() {
    jestshape = 0;
    shape.clear();
}
        
void ShapeLib::delRef(int texx) {
    /*if(!mtex.containsKey(texx)) return;
    mtex.get(texx).ref--;
    if(mtex.get(texx).ref<=0){

        if(mtex.get(texx).glLoaded){
            mtex.get(texx).delVBO(gl);
            mtex.remove(texx);
        }
    }*/
}
        
void ShapeLib::addRef(int texx) {
    //if(!mtex.containsKey(texx)) return;
    //mtex.get(texx).ref++;
}
int ShapeLib::addShape(QString path, QString name){
    return addShape(path, name, Game::root+"/routes/"+Game::route+"/textures");
}       
int ShapeLib::addShape(QString path, QString name, QString texPath) {
    QString pathid = (path + "/" + name).toLower();
    pathid.replace("\\", "/");
    pathid.replace("//", "/");
    //console.log(pathid);
    for ( auto it = shape.begin(); it != shape.end(); ++it ){
        if (((SFile*) it->second)->pathid.length() == pathid.length())
            if (((SFile*) it->second)->pathid == pathid) {
                ((SFile*) it->second)->ref++;
                return (int)it->first;
            }
    }
    //qDebug() << "Nowy " << jestshape << " shape: " << pathid;

    shape[jestshape] = new SFile(pathid, name, texPath);
    shape[jestshape]->pathid = pathid;

    return jestshape++;
}