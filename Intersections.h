#ifndef INTERSECTIONS_H
#define	INTERSECTIONS_H

class Intersections {
public:
    static float pointSegmentDistance(float* v, float* w, float* p, float* intersectionPoint = 0);
    static bool segmentIntersection(float &p0_x, float &p0_y, float &p1_x, float &p1_y, 
    float &p2_x, float &p2_y, float &p3_x, float &p3_y, float &i_x, float &i_y);
private:

};

#endif	/* INTERSECTIONS_H */

