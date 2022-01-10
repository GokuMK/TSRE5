/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "RouteClient.h"
#include "RouteEditorClient.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include "TSectionDAT.h"
#include "GLUU.h"
#include "Tile.h"
#include "GLMatrix.h"
#include "TerrainLib.h"
#include "TerrainLibSimple.h"
#include "TerrainLibQtClient.h"
#include "Game.h"
#include "TrackObj.h"
#include "Path.h"
#include "Terrain.h"
#include "FileFunctions.h"
#include "ParserX.h"
#include "ReadFile.h"
#include "DynTrackObj.h"
#include "PlatformObj.h"
#include "CarSpawnerObj.h"
#include "Flex.h"
#include "ForestObj.h"
#include "Coords.h"
#include "CoordsMkr.h"
#include "CoordsKml.h"
#include "CoordsGpx.h"
#include "CoordsRoutePlaces.h"
#include "SoundList.h"
#include "ActLib.h"
#include "Trk.h"
#include "AboutWindow.h"
#include "TrkWindow.h"
#include "PlatformObj.h"
#include "GroupObj.h"
#include "Undo.h"
#include "Activity.h"
#include "Service.h"
#include "Traffic.h"
#include "Path.h"
#include "Environment.h"
#include "OrtsWeatherChange.h"
#include "GeoCoordinates.h"
#include "Consist.h"
#include "Skydome.h"
#include "TRitem.h"
#include "ActionChooseDialog.h"
#include "ErrorMessagesLib.h"
#include "ErrorMessage.h"
#include "AceLib.h"
#include "Renderer.h"
#include "RenderItem.h"

RouteClient::RouteClient() {
    
}

void RouteClient::load(){
    switch(loadingProgress){
        case 0:
            Game::currentRoute = this;
            trkName = Game::trkName;
            routeDir = Game::route;

            qDebug() << "# Load Remote Route";

            if(!Game::useQuadTree)
                terrainLib = new TerrainLibSimple();
            else
                terrainLib = new TerrainLibQtClient();
            Game::terrainLib = terrainLib;

            Game::serverClient->sendUtf16Message("request_trk ( ) \n");
            break;
            
        case 1:
            Game::useSuperelevation = trk->tsreSuperelevation;

            if(trk->tsreProjection != NULL){
                qDebug() << "TSRE Geo Projection";
                Game::GeoCoordConverter = new GeoTsreCoordinateConverter(trk->tsreProjection);
            } else {
                qDebug() << "MSTS Geo Projection";
                Game::GeoCoordConverter = new GeoMstsCoordinateConverter();
            }
            env = new Environment(Game::root + "/routes/" + Game::route + "/ENVFILES/editor.env");
            Game::routeName = trk->routeName.toLower();
            routeName = Game::routeName;
            qDebug() << Game::routeName;

            this->tsection = new TSectionDAT(false, false);
            Game::serverClient->sendUtf16Message("request_tsection ( ) \n");
            break;
            
        case 2:
            Game::serverClient->sendUtf16Message("request_tdb ( ) \n request_rdb ( ) \n ");
            Game::serverClient->sendUtf16Message("request_terrain_qt ( ) \n ");
            break;

        case 6:
            this->ref = new Ref("");
            Game::serverClient->sendUtf16Message("request_addons ( ) \n ");
            //loadAddons();

            //loadMkrList();
            //createMkrPlaces();
            //loadServices();
            //loadTraffic();
            //loadPaths();
            //loadActivities();

            soundList = new SoundList();
            soundList->loadSoundSources(Game::root + "/routes/" + Game::route + "/ssource.dat");
            soundList->loadSoundRegions(Game::root + "/routes/" + Game::route + "/ttype.dat");
            Game::soundList = soundList;/**/

            //Game::terrainLib->loadQuadTree();
            //OrtsWeatherChange::LoadList();
            ForestObj::LoadForestList();
            ForestObj::ForestClearDistance = trk->forestClearDistance;
            CarSpawnerObj::LoadCarSpawnerList();

            loaded = true;

            Vec3::set(placementAutoTranslationOffset, 0, 0, 0);
            Vec3::set(placementAutoRotationOffset, 0, 0, 0);

            skydome = new Skydome();

            emit initDone();
            break;
        default:
            break;
    }
}

RouteClient::RouteClient(const RouteClient& orig) {
}

RouteClient::~RouteClient() {
}

Tile * RouteClient::requestTile(int x, int z, bool allowNew){
    Tile *tTile;
    tTile = tile[((x)*10000 + z)];
    if (tTile != NULL)
        return tTile;

    Game::serverClient->sendUtf16Message("request_tile ( "+QString::number(x)+" "+QString::number(z)+" )");
    tile[((x)*10000 + z)] = new Tile();
    //tile[(x)*10000 + z] = new Tile(x, z);
    /*tTile = tile[((x)*10000 + z)];
    if (tTile->loaded == -2) {
        if (Game::terrainLib->isLoaded(x, z)) {
            tTile->initNew();
        } else {
            return NULL;
        }
    }*/
    return NULL;
}

void RouteClient::save(){
    
}

void RouteClient::getUnsavedInfo(QVector<QString> &items){
    
}

