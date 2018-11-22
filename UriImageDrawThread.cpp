/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "UriImageDrawThread.h"
#include "GeoCoordinates.h"

#include <QImage>


void UriImageDrawThread::run(){
    double tx, tz;
    double tlat, tlon;
    double mindist;
    int minId;
    double tdist;
    unsigned char* imageData = myImage->bits();
    int bytesPerPixel = 3;
    bool alpha = false;
    if(myImage->format() == QImage::Format_RGBA8888){
        bytesPerPixel = 4;
        alpha = true;
    }
    for(double i = 0; i < myImage->height(); i++){
        for(double j = wmin; j < wmax; j++){
            tx = i/myImage->height();
            tz = j/myImage->width();
            
            tlat = llpoint00->Latitude*(1.0 - tx)*(1.0 - tz) +
                llpoint01->Latitude*(tx)*(1.0 - tz) +
                llpoint10->Latitude*(1.0 - tx)*(tz) +
                llpoint11->Latitude*(tx)*(tz);
            tlon = llpoint00->Longitude*(1.0 - tx)*(1.0 - tz) +
                llpoint01->Longitude*(tx)*(1.0 - tz) +
                llpoint10->Longitude*(1.0 - tx)*(tz) +
                llpoint11->Longitude*(tx)*(tz);
            
            minId = -1;
            mindist = 999999;
            for(int u = 0; u < images->length(); u++){
                //if(images[u].isPoint(tlat, tlon)){
                //    myImage->setPixelColor(i, j, images[u].getPixel(tlat, tlon));
                //    break;
                //}
                //if(images[u].isPoint(tlat, tlon)){
                tdist = (*images)[u].distanceToCenter(tlat, tlon);
                if( tdist < mindist ){
                    minId = u;
                    mindist = tdist;
                }
                //}
            }
            //QRgb;
            //if(minId >= 0 && minId < images.length());
                //myImage->setPixelColor(i, j, images[minId].getPixel(tlat, tlon));
                //myImage->setPixelColor(i, j, QColor::fromRgba(images[minId].getPixel(tlat, tlon)));
            //myImage->setPixel(i, j, qRgba(128,128,128,128));
            //imageData
            (*images)[minId].getPixel(tlat, tlon, imageData + (int)j*myImage->width()*bytesPerPixel + (int)i*bytesPerPixel, alpha );
            //myImage->setPixel(i, j, images[minId].getPixel(tlat, tlon));
            
        }
    }
}