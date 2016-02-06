#ifndef SPEEDPOSTDAT_H
#define	SPEEDPOSTDAT_H

#include <QString>
//#include "SpeedPost.h"
class SpeedPost;

class SpeedPostDAT {
public:
    QString speed_Warning_Sign_Shape;
    QString restricted_Shape;
    QString end_Restricted_Shape;
    
    std::vector<SpeedPost*> speedPost;
    
    SpeedPostDAT();
    virtual ~SpeedPostDAT();
private:

};

#endif	/* SPEEDPOSTDAT_H */

