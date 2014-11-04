#include "Route.h"
#include "GLUU.h"
#include "Tile.h"
#include "GLMatrix.h"
#include "TerrainLib.h"
#include "Game.h"

Route::Route() {
    this->trackDB = new TDB((Game::root+"/routes/"+Game::route+"/"+Game::route+".tdb"));
}

Route::Route(const Route& orig) {
}

Route::~Route() {
}

void Route::render(GLUU *gluu, float * playerT, float* playerW, float* target, float fov){
        int mintile = -2;
        int maxtile = 2;
        
        TerrainLib::render(gluu, playerT, playerW, target, fov);
        
        //for (var key in this.tile){
       //    this.tile[key].inUse = false;
       // }
        Tile *tTile;
        for(int i=mintile; i<=maxtile; i++){
            for(int j=maxtile; j>=mintile; j--){
                try {
                    tTile = tile.at((playerT[0]+i)*10000+playerT[1]+j);
                    if(tTile->loaded == -2)  continue;
                } catch (const std::out_of_range& oor) {
                    tile[(playerT[0]+i)*10000+playerT[1]+j] = new Tile(playerT[0]+i,playerT[1]+j);
                }
                tTile = tile[(playerT[0]+i)*10000+playerT[1]+j];
                //tTile->inUse = true;
                if(tTile->loaded == 1){
                    gluu->mvPushMatrix();
                    Mat4::translate(gluu->mvMatrix, gluu->mvMatrix, 2048 * i, 0, 2048 * j);
                    tTile->render(playerT, playerW, target, fov);
                    gluu->mvPopMatrix();
                 }
            }
        }
        /*
        for (var key in this.tile){
           if(this.tile[key] === undefined) continue;
           if(this.tile[key] === null) continue;
           //console.log(this.tile[key].inUse);
           if(!this.tile[key].inUse){
               //console.log("a"+this.tile[key]);
               this.tile[key] = undefined;
           } else {
               this.tile[key].inUse = false;
           }
        }*/
}

