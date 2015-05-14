#ifndef GAMESETTINGS_H
#define	GAMESETTINGS_H

#include <QString>

class Game {
public:
    static QString root;
    static QString route;
    static float objectLod;
    static int allowObjLag;
    static int maxObjLag;
    static void load();
    static void check_coords(int& x, int& z, float* p);
    static int start;
    static int startTileX;
    static int startTileY;
    static bool deleteTrWatermarks;
    static bool deleteViewDbSpheres;
    static bool createNewRoutes;
    static bool writeEnabled;
private:
};

#endif	/* GAMESETTINGS_H */

