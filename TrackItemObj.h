#ifndef TRACKITEMOBJ_H
#define	TRACKITEMOBJ_H

#include "OglObj.h"

class TrackItemObj : public OglObj {
public:
    TrackItemObj(int type = 0);
    TrackItemObj(const TrackItemObj& orig);
    virtual ~TrackItemObj();
private:

};

#endif	/* TRACKITEMOBJ_H */

