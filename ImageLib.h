#ifndef IMAGELIB_H
#define	IMAGELIB_H

#include <QThread>
#include "Texture.h"

class ImageLib : public QThread
 {
     Q_OBJECT

public:
    ImageLib();
    Texture* texture;
private:
    
protected:
     void run();
};

#endif	/* IMAGELIB_H */

