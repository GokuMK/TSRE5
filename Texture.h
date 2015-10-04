#ifndef TEXTURE_H
#define	TEXTURE_H

#include <QString>
class Brush;

class Texture {
public:
    Texture();
    Texture(const Texture* orig);
    virtual ~Texture();
    
     unsigned char* imageData;		
     int bpp;	
     int imageSize;
     int bytesPerPixel;
     int compressed;
     int width;			
     int height;			
     int texID;			
     int type;		
     int typk;
     unsigned int* tex;
     QString pathid;
     bool loaded = false;
     int ref = 0;
     bool glLoaded = false;
     bool editable = false;

    Texture(QString pathid) {
        this->pathid = pathid;
    }

    bool GLTextures();
    void update();
    void crop(float x1, float y1, float x2, float y2);
    void paint(Brush* brush, float x, float z);
    unsigned char * getImageData(int width, int height);
    void delVBO();
private:
    
    void setEditable();
};

#endif	/* TEXTURE_H */

