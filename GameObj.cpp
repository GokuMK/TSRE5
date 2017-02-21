/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "GameObj.h"

GameObj::GameObj() {
}

GameObj::GameObj(const GameObj& orig) {
}

GameObj::~GameObj() {
}

bool GameObj::select(){
    this->selected = true;
    return true;
}

bool GameObj::select(int value){
    return select();
}

bool GameObj::unselect(){
    this->selected = false;
    return false;
}

bool GameObj::isSelected(){
    return this->selected;
}

void GameObj::updateSim(float deltaTime){
    return;
}

void GameObj::translate(float px, float py, float pz){

}

void GameObj::rotate(float x, float y, float z){

}

void GameObj::resize(float x, float y, float z){
    
}

void GameObj::setPosition(int x, int z, float* p){

}

void GameObj::setPosition(float* p){

}

void GameObj::setMartix(){
    
}