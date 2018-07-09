/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "TerrainLib.h"
#include "Terrain.h"
#include "GLMatrix.h"
#include <QOpenGLShaderProgram>
#include <set>
#include <math.h>
#include "Game.h"
#include "Brush.h"
#include "HeightWindow.h"
#include "QuadTree.h"
#include "Undo.h"
#include "Route.h"
#include "Environment.h"
#include "TerrainInfo.h"

TerrainLib::TerrainLib() {
    
}

TerrainLib::TerrainLib(const TerrainLib& orig) {
    
}

TerrainLib::~TerrainLib() {
    
}

void TerrainLib::setDetailedAsCurrent(){
    
}

void TerrainLib::setDistantAsCurrent(){
    
}
    
Terrain *TerrainLib::getTerrainByXY(int x, int y, bool load){
    return NULL;
}

void TerrainLib::loadQuadTree(){

}

void TerrainLib::createNewRouteTerrain(int x, int z){

}

void TerrainLib::saveEmpty(int x, int z){

}

bool TerrainLib::isLoaded(int x, int z) {
    return false;
}

bool TerrainLib::load(int x, int z) {
    return false;
}

void TerrainLib::getUnsavedInfo(std::vector<QString> &items){

}

void TerrainLib::save(){

}

bool TerrainLib::reload(int x, int z) {
    return false;
}

float TerrainLib::getHeight(int x, int z, float posx, float posz) {
    return 0;
}

void TerrainLib::refresh(int x, int z) {

}

void TerrainLib::setHeight(int x, int z, float posx, float posz, float h) {

}

Terrain* TerrainLib::setHeight256(int x, int z, int posx, int posz, float h){
    return 0;
}

Terrain* TerrainLib::setHeight256(int x, int z, int posx, int posz, float h, float diffC, float diffE) {
    return 0;
}

float TerrainLib::getHeight(int x, int z, float posx, float posz, bool addR) {
    return 0;
}

void TerrainLib::fillHeightMap(int x, int z, float* data){

}

void TerrainLib::getRotation(float* rot, int x, int z, float posx, float posz){

}

void TerrainLib::setHeightFromGeoGui(int x, int z, float* p){

}

void TerrainLib::setHeightFromGeo(int x, int z, float* p){

}

void TerrainLib::setDetailedTerrainAsCurrent(){
    
}

void TerrainLib::setLowTerrainAsCurrent(){
        
}
    
void TerrainLib::setTextureToTrackObj(Brush* brush, float* punkty, int length, int tx, int tz){

}

void TerrainLib::setTerrainToTrackObj(Brush* brush, float* punkty, int length, int tx, int tz, float* matrix, float offsetY){

}

void TerrainLib::setTerrainTexture(Brush* brush, int x, int z, float* p){

}

void TerrainLib::toggleWaterDraw(int x, int z, float* p, float direction){

}

void TerrainLib::makeTextureFromMap(int x, int z, float* p){

}

void TerrainLib::removeTileTextureFromMap(int x, int z, float* p){

}

void TerrainLib::setTileBlob(int x, int z, float* p){

}

void TerrainLib::setWaterLevelGui(int x, int z, float* p){

}

void TerrainLib::toggleDraw(int x, int z, float* p){

}

int TerrainLib::getTexture(int x, int z, float* p){

}

void TerrainLib::paintTexture(Brush* brush, int x, int z, float* p){

}

void TerrainLib::lockTexture(Brush* brush, int x, int z, float* p){

}

void TerrainLib::toggleGaps(int x, int z, float* p, float direction){

}

void TerrainLib::setFixedTileHeight(Brush* brush, int x, int z, float* p){

}

QSet<Terrain*> TerrainLib::paintHeightMap(Brush* brush, int x, int z, float* p){
    QSet<Terrain*> uterr;
    return uterr;
}

void TerrainLib::fillWaterLevels(float *w, int mojex, int mojez){

}

void TerrainLib::setWaterLevels(float *w, int mojex, int mojez){

}

void TerrainLib::fillRaw(Terrain *cTerr, int mojex, int mojez) {

}

void TerrainLib::render(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, int renderMode) {

}

void TerrainLib::renderLo(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, int renderMode) {

}

void TerrainLib::renderWater(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, int renderMode, int layer) {

}

void TerrainLib::renderWaterLo(GLUU *gluu, float * playerT, float* playerW, float* target, float fov, int renderMode, int layer) {

}

void TerrainLib::renderShadowMap(GLUU *gluu, float * playerT, float* playerW, float* target, float fov) {

}

void TerrainLib::renderEmpty(GLUU *gluu, float * playerT, float* playerW, float* target, float fov) {

}
