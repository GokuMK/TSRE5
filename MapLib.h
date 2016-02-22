#ifndef MAPLIB_H
#define	MAPLIB_H

#include <QThread>
#include "Texture.h"
#include <unordered_map>


class MapLib : public QThread
 {
     Q_OBJECT

public:
    MapLib();
    Texture* texture;

private:
    
protected:
     void run();
};


#endif	/* MAPLIB_H */

