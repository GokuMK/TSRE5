#ifndef GAMESETTINGS_H
#define	GAMESETTINGS_H

#include <QString>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Window;
class LoadWindow;
class TDB;

class Game {
public:
    static TDB *trackDB;
    static TDB *roadDB;     
    
    static QString root;
    static QString route;
    static QString routeName;
    static QString trkName;
    static float objectLod;
    static int tileLod;
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
    static bool usenNumPad;
    static float cameraFov;
    static bool viewWorldGrid;
    static bool viewTileGrid;
    static bool viewInteractives;
    static bool viewTrackDbLines;
    static bool viewTsectionLines;
    static bool viewMarkers;
    static bool warningBox;
    static bool viewPointer3d;
    static bool leaveTrackShapeAfterDelete;
    static bool renderTrItems;

private:
    static Window* window;
    static LoadWindow* loadWindow;
};

#endif	/* GAMESETTINGS_H */

