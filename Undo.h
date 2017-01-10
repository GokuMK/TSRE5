/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#ifndef UNDO_H
#define	UNDO_H

struct UndoState {
    unsigned long long id;
    
};

class Undo {

public:
    static void UndoLast();
    static void StateBegin();
    static void StateEnd();
    static void PushTerrainHeightMap(int x, int z, float *data);
    //static void PushTerrainTexture(int x, int z, int uu, unsigned char* data);
    
private:
    

};

#endif	/* UNDO_H */

