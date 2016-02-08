#include "IghCoords.h"
#include <qDebug>

constexpr double MstsCoordinates::IghLongitudeCenter[12];

IghCoordinate* MstsCoordinates::ConvertToIgh(PreciseTileCoordinate* coordinates) {
    return ConvertToIgh(coordinates->TileX, coordinates->TileZ, coordinates->X, coordinates->Z);
}

// MSTS Tile -> IGH

IghCoordinate* MstsCoordinates::ConvertToIgh(int tilex, int tilez, double x, double z) {
    //Debug.Assert(z >= 0, "tileZ is off the top");
    //Debug.Assert(z <= 1, "tileZ is off the bottom");
    //Debug.Assert(x >= 0, "tileX is off the left");
    //Debug.Assert(x <= 1, "tileX is off the right");
    return new IghCoordinate(2048 * (16384 - tilez - 1 + z), 2048 * (tilex + 16384 + x));
}

// IGH -> MSTS Precise Tile

PreciseTileCoordinate* MstsCoordinates::ConvertToTile(IghCoordinate* coordinates) {
    double tileX = coordinates->Sample / 2048;
    double tileZ = coordinates->Line / 2048;
    double x = tileX - floor(tileX);
    double z = tileZ - floor(tileZ);
    //qDebug() << tileX <<" "<< tileZ;
    //qDebug() << x <<" "<< z;
    //Debug.Assert(z >= 0, "tileZ is off the top");
    //Debug.Assert(z <= 1, "tileZ is off the bottom");
    //Debug.Assert(x >= 0, "tileX is off the left");
    //Debug.Assert(x <= 1, "tileX is off the right");
    return new PreciseTileCoordinate((int) floor(tileX) - 16384, 16384 - (int) floor(tileZ) - 1, x, z);
}

LatitudeLongitudeCoordinate* MstsCoordinates::ConvertToLatLon(IghCoordinate* coordinates) {
    // Line/Sample -> Latitude/Longitude Algorithm
    // Based on C code provided by the USGS, available at ftp://edcftp.cr.usgs.gov/pub/software/misc/gihll2ls.c.
    // By D. Steinwand, HSTX/EROS Data Center, June, 1993.

    //Debug.Assert(coordinates.Line >= 0, "line is off the top");
    //Debug.Assert(coordinates.Line <= IghImageHeight, "line is off the bottom");
    //Debug.Assert(coordinates.Sample >= 0, "line is off the left");
    //Debug.Assert(coordinates.Sample <= IghImageWidth, "line is off the right");

    double y = (IghImageTop - coordinates->Line) / IghRadius;
    double x = (IghImageLeft + coordinates->Sample) / IghRadius;

    //Debug.Assert(y >= -M_PI / 2, "y is off the bottom");
    //Debug.Assert(y <= +M_PI / 2, "y is off the top");
    //Debug.Assert(x >= -M_PI, "x is off the left");
    //Debug.Assert(x <= +M_PI, "x is off the right");

    int region = -1;
    if (y >= IghParallel41) { /* If on or above 40 44' 11.8" */
        if (x <= -IghMeridian40) { /* If to the left of -40 */
            region = 0;
        } else {
            region = 2;
        }
    } else if (y >= 0.0) { /* Between 0.0 and 40 44' 11.8" */
        if (x <= -IghMeridian40) { /* If to the left of -40 */
            region = 1;
        } else {
            region = 3;
        }
    } else if (y >= -IghParallel41) { /* Between 0.0 & -40 44' 11.8" */
        if (x <= -IghMeridian100) { /* If between -180 and -100 */
            region = 4;
        } else if (x <= -IghMeridian20) { /* If between -100 and -20 */
            region = 5;
        } else if (x <= IghMeridian80) { /* If between -20 and 80 */
            region = 8;
        } else { /* If between 80 and 180 */
            region = 9;
        }
    } else { /* Below -40 44' 11.8" */
        if (x <= -IghMeridian100) { /* If between -180 and -100 */
            region = 6;
        } else if (x <= -IghMeridian20) { /* If between -100 and -20 */
            region = 7;
        } else if (x <= IghMeridian80) { /* If between -20 and 80 */
            region = 10;
        } else { /* If between 80 and 180 */
            region = 11;
        }
    }
    x = x - IghLongitudeCenter[region];

    double lat = 0;
    double lon = 0;
    if ((region == 1) || (region == 3) || (region == 4) || (region == 5) || (region == 8) || (region == 9)) {
        lat = y;
        if (fabs(lat) > M_PI / 2) {
            // giherror("Input data error", "Goode-inverse");
            return 0;
        }
        double temp = fabs(lat) - M_PI / 2;
        if (fabs(temp) > 0.00000001) {
            temp = IghLongitudeCenter[region] + x / cos(lat);
            lon = adjust_lon(temp);
        } else {
            lon = IghLongitudeCenter[region];
        }
    } else {
        double arg = (y + 0.0528035274542 * sign(y)) / 1.4142135623731;
        if (fabs(arg) > 1.0) {
            return 0;
        }
        double theta = asin(arg);
        lon = IghLongitudeCenter[region] + (x / (0.900316316158 * cos(theta)));
        if (lon < -M_PI) {
            return 0;
        }
        arg = (2.0 * theta + sin(2.0 * theta)) / M_PI;
        if (fabs(arg) > 1.0) {
            return 0;
        }
        lat = asin(arg);
    }

    ///////////////////////////////////////////////////////////////////

    return new LatitudeLongitudeCoordinate(lat * 180 / M_PI, lon * 180 / M_PI);
}

// Lat/Lon -> MSTS IGH
IghCoordinate* MstsCoordinates::ConvertToIgh(LatitudeLongitudeCoordinate* coordinates) {
    MstsCoordinates::ConvertToIgh(coordinates->Latitude, coordinates->Longitude);
}

IghCoordinate* MstsCoordinates::ConvertToIgh(double lat, double lon) {
    // Latitude/Longitude -> Line/Sample Algorithm
    // Based on C code provided by the USGS, available at ftp://edcftp.cr.usgs.gov/pub/software/misc/gihll2ls.c.
    // By D. Steinwand, HSTX/EROS Data Center, June, 1993.

    //Debug.Assert(coordinates.Latitude >= -90, "latitude is off the bottom");
    //Debug.Assert(coordinates.Latitude <= 90, "latitude is off the top");
    //Debug.Assert(coordinates.Longitude >= -180, "longitude is off the left");
    //Debug.Assert(coordinates.Longitude <= 180, "longitude is off the right");

    lat = lat * M_PI / 180;
    lon = lon * M_PI / 180;

    int region = -1;
    if (lat >= IghParallel41) { /* If on or above 40 44' 11.8" */
        if (lon <= -IghMeridian40) { /* If to the left of -40 */
            region = 0;
        } else {
            region = 2;
        }
    } else if (lat >= 0.0) { /* Between 0.0 and 40 44' 11.8" */
        if (lon <= -IghMeridian40) { /* If to the left of -40 */
            region = 1;
        } else {
            region = 3;
        }
    } else if (lat >= -IghParallel41) { /* Between 0.0 & -40 44' 11.8" */
        if (lon <= -IghMeridian100) { /* If between -180 and -100 */
            region = 4;
        } else if (lon <= -IghMeridian20) { /* If between -100 and -20 */
            region = 5;
        } else if (lon <= IghMeridian80) { /* If between -20 and 80 */
            region = 8;
        } else { /* If between 80 and 180 */
            region = 9;
        }
    } else { /* Below -40 44' 11.8" */
        if (lon <= -IghMeridian100) { /* If between -180 and -100 */
            region = 6;
        } else if (lon <= -IghMeridian20) { /* If between -100 and -20 */
            region = 7;
        } else if (lon <= IghMeridian80) { /* If between -20 and 80 */
            region = 10;
        } else { /* If between 80 and 180 */
            region = 11;
        }
    }

    double y = 0;
    double x = 0;
    if ((region == 1) || (region == 3) || (region == 4) || (region == 5) || (region == 8) || (region == 9)) {
        double delta_lon = adjust_lon(lon - IghLongitudeCenter[region]);
        y = lat;
        x = IghLongitudeCenter[region] + delta_lon * cos(lat);
    } else {
        double delta_lon = adjust_lon(lon - IghLongitudeCenter[region]);
        double theta = lat;
        double constant = M_PI * sin(lat);

        /* Iterate using the Newton-Raphson method to find theta
          -----------------------------------------------------*/
        for (int i = 0;; i++) {
            double delta_theta = -(theta + sin(theta) - constant) / (1.0 + cos(theta));
            theta += delta_theta;
            if (fabs(delta_theta) < 0.00000000001) break;
            if (i >= 30) {
                //giherror("Iteration failed to converge", "Goode-forward");
                return 0;
            }
        }
        theta /= 2.0;
        y = 1.4142135623731 * sin(theta) - 0.0528035274542 * sign(lat);
        x = IghLongitudeCenter[region] + 0.900316316158 * delta_lon * cos(theta);
    }

    //Debug.Assert(y >= -M_PI / 2, "y is off the bottom");
    //Debug.Assert(y <= +M_PI / 2, "y is off the top");
    //Debug.Assert(x >= -M_PI, "x is off the left");
    //Debug.Assert(x <= +M_PI, "x is off the right");

    IghCoordinate* igh = new IghCoordinate(IghImageTop - y * IghRadius, x * IghRadius - IghImageLeft);

    //Debug.Assert(igh.Line >= 0, "line is off the top");
    //Debug.Assert(igh.Line <= IghImageHeight, "line is off the bottom");
    //Debug.Assert(igh.Sample >= 0, "line is off the left");
    //Debug.Assert(igh.Sample <= IghImageWidth, "line is off the right");

    return igh;
}

double MstsCoordinates::adjust_lon(double temp) {
    if (fabs(temp) >= M_PI) {
        if (temp > 0)
            return temp - 2 * M_PI;
        if (temp < 0)
            return temp + 2 * M_PI;
    }
    return temp;
}

double MstsCoordinates::sign(double a) {
    if (a < 0) return -1;
    return 1;
}