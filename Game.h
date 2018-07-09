/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef GAMESETTINGS_H
#define	GAMESETTINGS_H

#include <QString>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class RouteEditorWindow;
class LoadWindow;
class TDB;
class SoundList;
class ShapeLib;
class EngLib;
class QColor;
class Route;
class GameObj;
class TerrainLib;
class GeoWorldCoordinateConverter;

class Game {
public:
    static GeoWorldCoordinateConverter *GeoCoordConverter;
    
    static TDB *trackDB;
    static TDB *roadDB;
    static SoundList *soundList;
    static TerrainLib *terrainLib;
    
    static QString AppVersion;
    static QString AppDataVersion;
    static QString AppName;
    static QString root;
    static QString route;
    static QString routeName;
    static QString trkName;
    static QString mainWindowLayout;
    static QString ceWindowLayout;
    static QString ActivityToPlay;
    static bool useQuadTree;
    static bool playerMode;
    static bool useNetworkEng;
    static float objectLod;
    static int tileLod;
    static int allowObjLag;
    static int maxObjLag;
    static bool ignoreLoadLimits;
    static void load();
    static void InitAssets();
    static bool loadRouteEditor();
    static bool loadConEditor();
    static bool checkRoot(QString dir);
    static bool checkCERoot(QString dir);
    static bool checkRoute(QString dir);
    template<class T>
    static void check_coords(T&& x, T&& z, float* p);
    template<class T, class K>
    static void check_coords(T&& x, T&& z, K&& px, K&& pz);
    static int start;
    static int startTileX;
    static int startTileY;
    static bool ignoreMissingGlobalShapes;
    static bool deleteTrWatermarks;
    static bool deleteViewDbSpheres;
    static bool createNewRoutes;
    static bool writeEnabled;
    static bool writeTDB;
    static bool systemTheme;
    static bool toolsHidden;
    static bool usenNumPad;
    static float cameraFov;
    static float cameraSpeedMin;
    static float cameraSpeedStd;
    static float cameraSpeedMax;
    static float mouseSpeed;
    static bool cameraStickToTerrain;
    static bool mstsShadows;
    static bool viewWorldGrid;
    static bool viewTileGrid;
    static bool viewTerrainGrid;
    static bool viewTerrainShape;
    static bool viewInteractives;
    static bool viewTrackDbLines;
    static bool viewTsectionLines;
    static bool viewMarkers;
    static bool viewSnapable;
    static bool viewCompass;
    static bool warningBox;
    static bool viewPointer3d;
    static bool leaveTrackShapeAfterDelete;
    static bool renderTrItems;
    static int newRouteX;
    static int newRouteZ;
    static QString geoPath;
    static ShapeLib *currentShapeLib;
    static EngLib *currentEngLib;
    static Route *currentRoute;
    static GameObj *currentSelectedGameObj;
    static QColor *colorConView;
    static QColor *colorShapeView;
    static bool consoleOutput;
    static int fpsLimit;
    static bool ortsEngEnable;
    static bool sortTileObjects;
    static int oglDefaultLineWidth;
    static bool showWorldObjPivotPoints;
    static int shadowMapSize;
    static int shadowsEnabled;
    static float sunLightDirection[];
    static int textureQuality;
    static float snapableRadius;
    static bool snapableOnlyRot;
    static float trackElevationMaxPm;    
    static bool proceduralTracks;
    static QString StyleMainLabel;
    static QString StyleGreenButton;
    static QString StyleRedButton;
    static QString StyleYellowButton;
    static QString StyleGreenText;
    static QString StyleRedText;
    static QString imageMapsUrl;
    static int mapImageResolution;
    static bool autoNewTiles;
    static bool autoGeoTerrain;
    static bool useSuperelevation;
    static bool soundEnabled;
    static bool fullscreen;
    
    static float fogDensity;// = 0.7;
    //float fogColor[4]{0.5, 0.75, 1.0, 1.0};
    static float fogColor[4];//{230.0/255.0,248.0/255,255.0/255.0, 1.0};
    static float skyColor[4];//{230.0/255.0,248.0/255,255.0/255.0, 1.0};
    
    static int AASamples;
    static float PixelRatio;
    
    static int DefaultElevationBox;
    static float DefaultMoveStep;
private:
    static RouteEditorWindow* window;
    static LoadWindow* loadWindow;
    static void CreateNewSettingsFile();
    static void DownloadAppData(QString path);
    static void CheckForOpenAl();
};

#endif	/* GAMESETTINGS_H */

