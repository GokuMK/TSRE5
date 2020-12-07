/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ClientInfo.h"
#include "OglObj.h"
#include "TextObj.h"

ClientInfo::ClientInfo() {
    username = "UNDEFINED";
    X = Z = x = y = z = 0;
}

ClientInfo::ClientInfo(const ClientInfo& orig) {
}

ClientInfo::~ClientInfo() {
}

void ClientInfo::render(float playerRot) {

    if (stick == NULL) {
        stick = new OglObj();
        float *punkty = new float[3 * 2];
        int ptr = 0;
        int i = 0;

        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 0;
        punkty[ptr++] = 20;
        punkty[ptr++] = 0;

        stick->setMaterial(1.0, 1.0, 0.0);
        stick->init(punkty, ptr, RenderItem::V, GL_LINES);
        delete[] punkty;
    }
    if(name == NULL){
        name = new TextObj(this->username, 16, 2.0);
        name->setColor(0,0,0);
        name->pos[1] = 20;
    } 
    stick->render();
    name->render(playerRot);
};