/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  This file is conversion of:
 *  Jgr.Msts library, part of MSTS Editors & Tools 
 *  (http://jgrmsts.codeplex.com/).
 * 
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GeoCoordinates.h"
#include <QDebug>

constexpr double GeoMstsCoordinateConverter::IghLongitudeCenter[12];

IghCoordinate* GeoMstsCoordinateConverter::ConvertToInternal(PreciseTileCoordinate* coordinates, IghCoordinate* out) {
    return ConvertToInternal(coordinates->TileX, coordinates->TileZ, coordinates->X, coordinates->Z, out);
}

// MSTS Tile -> IGH

IghCoordinate* GeoMstsCoordinateConverter::ConvertToInternal(int tilex, int tilez, double x, double z, IghCoordinate* out) {
    //Debug.Assert(z >= 0, "tileZ is off the top");
    //Debug.Assert(z <= 1, "tileZ is off the bottom");
    //Debug.Assert(x >= 0, "tileX is off the left");
    //Debug.Assert(x <= 1, "tileX is off the right");
    if(out == 0)
        return new IghCoordinate(2048 * (16384 - tilez - 1 + z), 2048 * (tilex + 16384 + x));
    out->set(2048 * (16384 - tilez - 1 + z), 2048 * (tilex + 16384 + x));
    return out;
}

// IGH -> MSTS Precise Tile

PreciseTileCoordinate* GeoMstsCoordinateConverter::ConvertToTile(IghCoordinate* coordinates, PreciseTileCoordinate* out) {
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
    if(out == 0)
        return new PreciseTileCoordinate((int) floor(tileX) - 16384, 16384 - (int) floor(tileZ) - 1, x, z);
    out->set((int) floor(tileX) - 16384, 16384 - (int) floor(tileZ) - 1, x, z);
    return out;
}

LatitudeLongitudeCoordinate* GeoMstsCoordinateConverter::ConvertToLatLon(IghCoordinate* coordinates, LatitudeLongitudeCoordinate* out) {
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
    if(out == 0)
        return new LatitudeLongitudeCoordinate(lat * 180 / M_PI, lon * 180 / M_PI);
    out->set(lat * 180 / M_PI, lon * 180 / M_PI);
    return out;
}

// Lat/Lon -> MSTS IGH
IghCoordinate* GeoMstsCoordinateConverter::ConvertToInternal(LatitudeLongitudeCoordinate* coordinates, IghCoordinate* out) {
    return ConvertToInternal(coordinates->Latitude, coordinates->Longitude, out);
}

IghCoordinate* GeoMstsCoordinateConverter::ConvertToInternal(double lat, double lon, IghCoordinate* out) {
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

    //IghCoordinate* igh = new IghCoordinate(IghImageTop - y * IghRadius, x * IghRadius - IghImageLeft);

    //Debug.Assert(igh.Line >= 0, "line is off the top");
    //Debug.Assert(igh.Line <= IghImageHeight, "line is off the bottom");
    //Debug.Assert(igh.Sample >= 0, "line is off the left");
    //Debug.Assert(igh.Sample <= IghImageWidth, "line is off the right");

    if(out == 0)
        return new IghCoordinate(IghImageTop - y * IghRadius, x * IghRadius - IghImageLeft);
    out->set(IghImageTop - y * IghRadius, x * IghRadius - IghImageLeft);
    return out;
}

double GeoMstsCoordinateConverter::adjust_lon(double temp) {
    if (fabs(temp) >= M_PI) {
        if (temp > 0)
            return temp - 2 * M_PI;
        if (temp < 0)
            return temp + 2 * M_PI;
    }
    return temp;
}

double GeoMstsCoordinateConverter::sign(double a) {
    if (a < 0) return -1;
    return 1;
}

GeoTsreCoordinateConverter::GeoTsreCoordinateConverter(double *latLonXY){
    centerLat = latLonXY[0];
    centerLon = latLonXY[1];
    centerX = latLonXY[2];
    centerZ = latLonXY[3];
    
    double centerLatRad = (centerLat*M_PI)/180.0;
    stepLat = 111132.92 - 559.82 * cos( 2 * centerLatRad ) + 1.175 * cos( 4 * centerLatRad) - 0.0023 * cos( 6 * centerLatRad);
    stepLon = 111412.84 * cos ( centerLatRad ) - 93.5 * cos ( 3*centerLatRad ) ;
    qDebug() << "Projection "<<centerLat << centerLon;
    qDebug() << "Projection step "<<stepLat << stepLon;
}

IghCoordinate* GeoTsreCoordinateConverter::ConvertToInternal(PreciseTileCoordinate* coordinates, IghCoordinate* out){
    return ConvertToInternal(coordinates->TileX, coordinates->TileZ, coordinates->X, coordinates->Z, out);
}

IghCoordinate* GeoTsreCoordinateConverter::ConvertToInternal(int tilex, int tilez, double x, double z, IghCoordinate* out){
    //qDebug() << "tile to internal" <<centerX << centerZ << tilex << tilez << x << z;
    if(out == 0)
        return new IghCoordinate(2048.0 * (tilez + (1.0 - z)) - centerZ, 2048.0 * (tilex + x) - centerX);
    out->set(2048.0 * (tilez + (1.0 - z)) - centerZ, 2048.0 * (tilex + x) - centerX);
    return out;
}

PreciseTileCoordinate* GeoTsreCoordinateConverter::ConvertToTile(IghCoordinate* coordinates, PreciseTileCoordinate* out){
    double tileX = (coordinates->Sample + centerX) / 2048;
    double tileZ = (coordinates->Line + centerZ) / 2048;
    double x = tileX - floor(tileX);
    double z = tileZ - floor(tileZ);
    if(out == 0)
        return new PreciseTileCoordinate((int) floor(tileX), (int) floor(tileZ), x, 1.0 - z);
    out->set((int) floor(tileX), (int) floor(tileZ), x, 1.0 - z);
    return out;
}

LatitudeLongitudeCoordinate* GeoTsreCoordinateConverter::ConvertToLatLon(IghCoordinate* coordinates, LatitudeLongitudeCoordinate* out){
    double lat = centerLat + coordinates->Line / stepLat;
    double lon = centerLon + coordinates->Sample / stepLon;
    if(out == 0)
        return new LatitudeLongitudeCoordinate(lat, lon);
    out->set(lat, lon);
    return out;
}

IghCoordinate* GeoTsreCoordinateConverter::ConvertToInternal(LatitudeLongitudeCoordinate* coordinates, IghCoordinate* out){
    return ConvertToInternal(coordinates->Latitude, coordinates->Longitude, out);
}

IghCoordinate* GeoTsreCoordinateConverter::ConvertToInternal(double lat, double lon, IghCoordinate* out){
    double line = (lat - centerLat) * stepLat;
    double sample = (lon - centerLon) * stepLon;
    if(out == 0)
        return new IghCoordinate(line, sample);
    out->set(line, sample);
    return out;
}

float LatitudeLongitudeCoordinate::distanceTo(LatitudeLongitudeCoordinate *c){
    double centerLat = (Latitude + c->Latitude)/2;
    double centerLon = (Longitude + c->Longitude)/2;
    
    double centerLatRad = (centerLat*M_PI)/180.0;
    double stepLat = 111132.92 - 559.82 * cos( 2 * centerLatRad ) + 1.175 * cos( 4 * centerLatRad) - 0.0023 * cos( 6 * centerLatRad);
    double stepLon = 111412.84 * cos ( centerLatRad ) - 93.5 * cos ( 3*centerLatRad ) ;
    //qDebug() << "Projection "<<Latitude << c->Latitude << Longitude<<c->Longitude;
    //qDebug() << "Projection "<<centerLat << centerLon;
    //qDebug() << "Projection step "<<stepLat << stepLon;
    
    double deltaLat = fabs(Latitude - c->Latitude);
    double deltaLon = fabs(Longitude - c->Longitude);
    return sqrt(pow(deltaLat * stepLat, 2) + pow(deltaLon * stepLon, 2));
}