#ifndef GAMESETTINGS_H
#define	GAMESETTINGS_H

#include <QString>
#include "window.h"
#include "LoadWindow.h"

class Game {
public:
    static QString root;
    static QString route;
    static QString routeName;
    static float objectLod;
    static int allowObjLag;
    static int maxObjLag;
    static void load();
    static bool checkSettings();
    static bool checkRoot(QString dir);
    static bool checkRoute(QString dir);
    template<class T>
    static void check_coords(T&& x, T&& z, float* p);
    template<class T, class K>
    static void check_coords(T&& x, T&& z, K&& px, K&& pz);
    static int start;
    static int startTileX;
    static int startTileY;
    static bool deleteTrWatermarks;
    static bool deleteViewDbSpheres;
    static bool createNewRoutes;
    static bool writeEnabled;
    static bool writeTDB;
    static bool systemTheme;
    static bool toolsHidden;
    
private:
    static Window* window;
    static LoadWindow* loadWindow;
};

#endif	/* GAMESETTINGS_H */

