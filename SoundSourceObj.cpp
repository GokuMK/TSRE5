/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "SoundSourceObj.h"
#include "PoleObj.h"
#include "GLMatrix.h"
#include "TS.h"
#include "ParserX.h"
#include "FileBuffer.h"
#include "SoundList.h"
#include "Game.h"

SoundSourceObj::SoundSourceObj() {
}

SoundSourceObj::~SoundSourceObj() {
}

bool SoundSourceObj::allowNew(){
    return true;
}

bool SoundSourceObj::isSoundItem(){
    return true;
}

void SoundSourceObj::load(int x, int y) {
    this->x = x;
    this->y = y;
    this->position[2] = -this->position[2];
    Quat::fill(this->qDirection);
    this->loaded = true;

    setMartix();
}

void SoundSourceObj::set(QString sh, int val){
    if (sh == ("ref_value")) {
        this->fileName = "";
        for (auto it = Game::soundList->sources.begin(); it != Game::soundList->sources.end(); ++it ){
            if(it->second->id == val){
                this->fileName = it->second->file1;
            }
        }
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void SoundSourceObj::set(QString sh, QString val){
    if (sh == ("filename")) {
        fileName = val;
        return;
    }
    if (sh == ("ref_filename")) {
        fileName = val;
        return;
    }
    WorldObj::set(sh, val);
    return;
}

void SoundSourceObj::set(int sh, FileBuffer* data) {
    if (sh == TS::FileName) {
        data->off++;
        int slen = data->getShort()*2;
        fileName = *data->getString(data->off, data->off + slen);
        data->off += slen;
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SoundSourceObj::set(QString sh, FileBuffer* data) {
    if (sh == ("filename")) {
        fileName = ParserX::GetString(data);
        return;
    }
    WorldObj::set(sh, data);
    return;
}

void SoundSourceObj::render(GLUU* gluu, float lod, float posx, float posz, float* pos, float* target, float fov, int selectionColor, int renderMode) {
    if (!loaded) return;
    if(!Game::viewInteractives || renderMode == gluu->RENDER_SHADOWMAP) 
        return;
        
    Mat4::multiply(gluu->mvMatrix, gluu->mvMatrix, matrix);
    gluu->currentShader->setUniformValue(gluu->currentShader->mvMatrixUniform, *reinterpret_cast<float(*)[4][4]> (gluu->mvMatrix));
    
    if(selectionColor != 0){
        int wColor = (int)(selectionColor/65536);
        int sColor = (int)(selectionColor - wColor*65536)/256;
        int bColor = (int)(selectionColor - wColor*65536 - sColor*256);
        gluu->disableTextures((float)wColor/255.0f, (float)sColor/255.0f, (float)bColor/255.0f, 1);
    } else {
        gluu->enableTextures();
    }
    
    if(pointer3d == NULL)
        pointer3d = new PoleObj();
        
    pointer3d->render(selectionColor);
};

int SoundSourceObj::getDefaultDetailLevel(){
    return -4;
}

void SoundSourceObj::save(QTextStream* out){
    if (!loaded) return;
    
*(out) << "	Soundsource (\n";
*(out) << "		Position ( "<<this->position[0]<<" "<<this->position[1]<<" "<<-this->position[2]<<" )\n";
*(out) << "		FileName ( "<<this->fileName<<" )\n";
*(out) << "		UiD ( "<<this->UiD<<" )\n";
*(out) << "	)\n";
}