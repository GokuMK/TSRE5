/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef IGHCOORDS_H
#define	IGHCOORDS_H

#include <math.h>

#include "Game.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/// <summary>
/// Extends a <see cref="TileCoordinate"/> to contain an object location as <see cref="X"/>, <see cref="Y"/> and <see cref="Z"/> within the tile.
/// </summary>

struct PreciseTileCoordinate {
    int TileX;
    int TileZ;
    int Size;
    double X;
    double Y;
    double Z;
    float wX;
    float wY;
    float wZ;
    float alt;

    PreciseTileCoordinate(int tileX, int tileZ, double x, double z)
    : PreciseTileCoordinate(tileX, tileZ, x, 0, z) {
    }

    PreciseTileCoordinate(int tileX, int tileZ, double x, double y, double z){
        TileX = tileX;
        TileZ = tileZ;
        Size = 1;
        X = x;
        Y = y;
        Z = z;
    }
    void set(int tileX, int tileZ, double x, double z){
        set(tileX, tileZ, x, 0, z);
    }
    void set(int tileX, int tileZ, double x, double y, double z){
        TileX = tileX;
        TileZ = tileZ;
        Size = 1;
        X = x;
        Y = y;
        Z = z;
    }
    void setWxyz(float x, float y, float z){
        this->wX = x;
        this->wY = y;
        this->wZ = z;
        //checkCoords();
        this->X = (x + 1024.0)/2048.0;
        this->Y = y;
        this->Z = (z + 1024.0)/2048.0;
    }
    void setTWxyz(int tx, int ty, float x, float y, float z){
        this->TileX = tx;
        this->TileZ = ty;
        this->wX = x;
        this->wY = y;
        this->wZ = z;
        //checkCoords();
        this->X = (wX + 1024.0)/2048.0;
        this->Y = wY;
        this->Z = (wZ + 1024.0)/2048.0;
    }
    void setTWxyzU(int tx, int ty, float x, float y, float z){
        this->TileX = tx;
        this->TileZ = ty;
        this->wX = x-1024.0;
        this->wY = y;
        this->wZ = z-1024.0;
        //checkCoords();
        this->X = (x)/2048.0;
        this->Y = wY;
        this->Z = (z)/2048.0;
    }
    void setWxyzU(float x, float y, float z){
        this->wX = x;
        this->wY = y;
        this->wZ = z;
        //checkCoords();
        this->X = (wX)/2048.0;
        this->Y = wY;
        this->Z = (wZ)/2048.0;
    }
    void checkCoords(){
        if (wX >= 1024) {
            wX -= 2048;
            TileX++;
        }
        if (wX < -1024) {
            wX += 2048;
            TileX--;
        }
        if (wZ >= 1024) {
            wZ -= 2048;
            TileZ--;
        }
        if (wZ < -1024) {
            wZ += 2048;
            TileZ++;
        }
    }
    void setXYZ(double x, double y, double z){
        this->X = x;
        this->Y = y;
        this->Z = z;
        this->wX = (x*2048.0) - 1024;
        this->wY = y;
        this->wZ = (z*2048.0) - 1024;
    }
    void setXYZ(){
        this->X = (wX + 1024.0)/2048.0;
        this->Y = wY;
        this->Z = (wZ + 1024.0)/2048.0;
    }
    void setWxyz(){
        this->wX = (X*2048.0) - 1024;
        this->wY = Y;
        this->wZ = (Z*2048.0) - 1024;
    }
    PreciseTileCoordinate(){
        wX = wY = wZ = X = Y = Z = TileX = TileZ = 0;
    }
};

/// <summary>
/// Contains a location as a <see cref="Line"/> (Y) and <see cref="Sample"/> (X) in the Interrupted Goode Homolosine projection.
/// </summary>

struct IghCoordinate {
    double Line;
    double Sample;
    
    IghCoordinate() {
        Line = 0;
        Sample = 0;
    }
    IghCoordinate(double line, double sample) {
        Line = line;
        Sample = sample;
    }
    void set(double line, double sample) {
        Line = line;
        Sample = sample;
    }
};
/// <summary>
/// Contains a location as a <see cref="Latitude"/> and <see cref="Longitude"/>.
/// </summary>

struct LatitudeLongitudeCoordinate {
    double Latitude;
    double Longitude;

    LatitudeLongitudeCoordinate(double latitude = 0, double longitude = 0) {
        Latitude = latitude;
        Longitude = longitude;
    }
    void set(double latitude, double longitude) {
        Latitude = latitude;
        Longitude = longitude;
    }
    float distanceTo(LatitudeLongitudeCoordinate *c);
};  

class GeoWorldCoordinateConverter {
public:
    virtual IghCoordinate* ConvertToInternal(PreciseTileCoordinate* coordinates, IghCoordinate* out = 0);
    virtual IghCoordinate* ConvertToInternal(int tilex, int tilez, double x, double z, IghCoordinate* out = 0);
    virtual PreciseTileCoordinate* ConvertToTile(IghCoordinate* coordinates, PreciseTileCoordinate* out = 0);
    virtual LatitudeLongitudeCoordinate* ConvertToLatLon(IghCoordinate* coordinates, LatitudeLongitudeCoordinate* out = 0);
    virtual IghCoordinate* ConvertToInternal(LatitudeLongitudeCoordinate* coordinates, IghCoordinate* out = 0);
    virtual IghCoordinate* ConvertToInternal(double lat, double lon, IghCoordinate* out = 0);
};

class GeoMstsCoordinateConverter : public GeoWorldCoordinateConverter {

public:
        // Original calculations:
    //   r =   6370997
    //   l = -20015500   w = 40031000
    //   t =   8673500   h = 17347000
    static constexpr double IghRadius = 6370997;
    static constexpr double IghImageLeft = -20015000;
    static constexpr double IghImageTop = 8673000;
    static constexpr double IghImageWidth = -2 * IghImageLeft;
    static constexpr double IghImageHeight = 2 * IghImageTop;
    static constexpr double IghLongitudeCenter[12] {
        -(double) 100 / 180 * M_PI, /* -100.0 degrees */
                -(double) 100 / 180 * M_PI, /* -100.0 degrees */
                +(double) 30 / 180 * M_PI, /*   30.0 degrees */
                +(double) 30 / 180 * M_PI, /*   30.0 degrees */
                -(double) 160 / 180 * M_PI, /* -160.0 degrees */
                -(double) 60 / 180 * M_PI, /*  -60.0 degrees */
                -(double) 160 / 180 * M_PI, /* -160.0 degrees */
                -(double) 60 / 180 * M_PI, /*  -60.0 degrees */
                +(double) 20 / 180 * M_PI, /*   20.0 degrees */
                +(double) 140 / 180 * M_PI, /*  140.0 degrees */
                +(double) 20 / 180 * M_PI, /*   20.0 degrees */
                +(double) 140 / 180 * M_PI, /*  140.0 degrees */
    };
    static constexpr double IghParallel41 = (40.0 + (44.0 / 60) + (11.8 / 3600)) / 180 * M_PI; // 40deg 44' 11.8"
    static constexpr double IghMeridian20 = 20.0 / 180 * M_PI; // 20deg
    static constexpr double IghMeridian40 = 40.0 / 180 * M_PI; // 40deg
    static constexpr double IghMeridian80 = 80.0 / 180 * M_PI; // 80deg
    static constexpr double IghMeridian100 = 100.0 / 180 * M_PI; // 100deg
    
    
    IghCoordinate* ConvertToInternal(PreciseTileCoordinate* coordinates, IghCoordinate* out = 0);
    // MSTS Tile -> IGH
    IghCoordinate* ConvertToInternal(int tilex, int tilez, double x, double z, IghCoordinate* out = 0);
    // IGH -> MSTS Precise Tile
    PreciseTileCoordinate* ConvertToTile(IghCoordinate* coordinates, PreciseTileCoordinate* out = 0);
    // IGH -> Lat/Lon
    LatitudeLongitudeCoordinate* ConvertToLatLon(IghCoordinate* coordinates, LatitudeLongitudeCoordinate* out = 0);
    // Lat/Lon -> MSTS IGH
    IghCoordinate* ConvertToInternal(LatitudeLongitudeCoordinate* coordinates, IghCoordinate* out = 0);
    IghCoordinate* ConvertToInternal(double lat, double lon, IghCoordinate* out = 0);
private:
    double adjust_lon(double temp);
    double sign(double a);
};

class GeoTsreCoordinateConverter : public GeoWorldCoordinateConverter {
public:
    GeoTsreCoordinateConverter(double *latLonXY);
    IghCoordinate* ConvertToInternal(PreciseTileCoordinate* coordinates, IghCoordinate* out = 0);
    IghCoordinate* ConvertToInternal(int tilex, int tilez, double x, double z, IghCoordinate* out = 0);
    PreciseTileCoordinate* ConvertToTile(IghCoordinate* coordinates, PreciseTileCoordinate* out = 0);
    LatitudeLongitudeCoordinate* ConvertToLatLon(IghCoordinate* coordinates, LatitudeLongitudeCoordinate* out = 0);
    IghCoordinate* ConvertToInternal(LatitudeLongitudeCoordinate* coordinates, IghCoordinate* out = 0);
    IghCoordinate* ConvertToInternal(double lat, double lon, IghCoordinate* out = 0);
private:
    double centerLat;
    double centerLon;
    double centerX;
    double centerZ;
    double stepLat;
    double stepLon;
};
#endif	/* IGHCOORDS_H */

