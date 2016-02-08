#ifndef IGHCOORDS_H
#define	IGHCOORDS_H

#include <math.h>

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
};

/// <summary>
/// Contains a location as a <see cref="Line"/> (Y) and <see cref="Sample"/> (X) in the Interrupted Goode Homolosine projection.
/// </summary>

struct IghCoordinate {
    double Line;
    double Sample;

    IghCoordinate(double line, double sample) {
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

    LatitudeLongitudeCoordinate(double latitude, double longitude) {
        Latitude = latitude;
        Longitude = longitude;
    }
};

class MstsCoordinates {

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
    
    
    static IghCoordinate* ConvertToIgh(PreciseTileCoordinate* coordinates);
    // MSTS Tile -> IGH
    static IghCoordinate* ConvertToIgh(int tilex, int tilez, double x, double z);
    // IGH -> MSTS Precise Tile
    static PreciseTileCoordinate* ConvertToTile(IghCoordinate* coordinates);
    // IGH -> Lat/Lon
    static LatitudeLongitudeCoordinate* ConvertToLatLon(IghCoordinate* coordinates);
    // Lat/Lon -> MSTS IGH
    static IghCoordinate* ConvertToIgh(LatitudeLongitudeCoordinate* coordinates);
    static IghCoordinate* ConvertToIgh(double lat, double lon);
    static double adjust_lon(double temp);
    static double sign(double a);
};


#endif	/* IGHCOORDS_H */

