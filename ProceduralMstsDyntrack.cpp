/*  This file is part of TSRE5.
 *
 *  TSRE5 - train sim game engine and MSTS/OR Editors. 
 *  Copyright (C) 2016 Piotr Gadecki <pgadecki@gmail.com>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *
 *  See LICENSE.md or https://www.gnu.org/licenses/gpl.html
 */

#include "ProceduralMstsDyntrack.h"
#include "Game.h"
#include "Vector2f.h"
#include "Vector3f.h"
#include "OglObj.h"

void ProceduralMstsDyntrack::GenShape(QVector<OglObj*> &shape, QVector<TSection> &sections) {
    float* pd = new float[55000];
    float* sk = new float[55000];

    int ptr = 0;
    int str = 0;
    
    Vector2f tx;
    Vector2f ty;
    Vector2f offpos(0.0, 0.0);
    Vector2f b1;
    Vector2f a1;
    Vector2f a;
    Vector2f b;
    Vector2f dl;
    
    float offrot = 0;
    GLUU *gluu = GLUU::get();
    float alpha = -gluu->alphaTest;
    
    for (int i = 0; i < sections.size(); i++) {
        //if (sections[i].sectIdx > 100000000) continue;
        //prosta
        if (sections[i].type == 0) {
            //podklady

            b.set(2.5, 0.0);
            b.rotate(offrot, 0);
            a1.set(0.0, sections[i].angle);
            a1.rotate(offrot, 0);

            pd[ptr++] = offpos.x + b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x - b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x - b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].angle;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x + b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].angle;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x + b.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y + b.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = -0.139000;
            pd[ptr++] = -1.0;
            pd[ptr++] = alpha;

            pd[ptr++] = offpos.x - b.x + a1.x;
            pd[ptr++] = 0.2;
            pd[ptr++] = offpos.y - b.y + a1.y;
            pd[ptr++] = 0.0;
            pd[ptr++] = 1.0;
            pd[ptr++] = 0.0;
            pd[ptr++] = 0.862000;
            pd[ptr++] = -1.0 + 0.2 * sections[i].angle;
            pd[ptr++] = alpha;
            //szyny

            tx.set(-0.717500, 0.0);
            ty.set(-0.867500, 0.0);

            
            for (int jj = 0; jj < 2; jj++) {
                tx.rotate(offrot, 0);
                ty.rotate(offrot, 0);

                sk[str++] = offpos.x + tx.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2270;
                sk[str++] = alpha;

                sk[str++] = offpos.x + ty.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.1330;
                sk[str++] = alpha;

                sk[str++] = offpos.x + ty.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.1330;
                sk[str++] = alpha;

                sk[str++] = offpos.x + tx.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.2270;
                sk[str++] = alpha;

                sk[str++] = offpos.x + tx.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + tx.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2270;
                sk[str++] = alpha;

                sk[str++] = offpos.x + ty.x + a1.x;
                sk[str++] = 0.325000;
                sk[str++] = offpos.y + ty.y + a1.y;
                sk[str++] = 0.0;
                sk[str++] = 1.0;
                sk[str++] = 0.0;
                sk[str++] = 0.2;
                sk[str++] = 0.1330;
                sk[str++] = alpha;
                
                tx.set(0.867500, 0.0);
                ty.set(0.717500, 0.0);
            }
            ///
            tx.set(0.717500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;
            
            tx.set(-0.717500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;
            
            tx.set(-0.867500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            tx.set(0.867500, 0.0);
            tx.rotate(offrot, 0);

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x;
            sk[str++] = 0.2;
            sk[str++] = offpos.y + tx.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.07;
            sk[str++] = alpha;

            sk[str++] = offpos.x + tx.x + a1.x;
            sk[str++] = 0.325;
            sk[str++] = offpos.y + tx.y + a1.y;
            sk[str++] = 1.0;
            sk[str++] = 0.0;
            sk[str++] = 0.0;
            sk[str++] = 0.069;
            sk[str++] = 0.002;
            sk[str++] = alpha;
            
            offpos.x += a1.x;
            offpos.y += a1.y;
        }
        //krzywa
        if(sections[i].type==1){
            float kierunek = 1;
            if(sections[i].angle > 0){
                kierunek = -1;
            }
            float aa = 0;
            float angle;
            float angle2;
            for(angle = 0, angle2 = 0; angle2>sections[i].angle*kierunek; angle-=0.03*kierunek,angle2-=0.03){
                if(sections[i].angle*kierunek-angle2<-0.03) 
                    aa = -0.03*kierunek;
                else 
                    aa = (sections[i].angle*kierunek-angle2)*kierunek;
                //podklady
                b1.set(-2.5, 0.0);
                a1.set(2.5, 0.0);
                a.set(-2.5, 0.0);
                b.set(2.5, 0.0);
                a1.rotate(angle, sections[i].radius*kierunek);
                b1.rotate(angle, sections[i].radius*kierunek);           
                a.rotate(angle+aa, sections[i].radius*kierunek);
                b.rotate(angle+aa, sections[i].radius*kierunek);
                a1.rotate(offrot, 0);
                b1.rotate(offrot, 0);
                a.rotate(offrot, 0);
                b.rotate(offrot, 0);
                dl.set(0.0, 0.0);
                dl.rotate(aa, sections[i].radius*kierunek); 
                float dlugosc = dl.getDlugosc();
                    
                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                pd[ptr++] = alpha;
                    
                pd[ptr++] = offpos.x+b1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0;
                pd[ptr++] = alpha;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                pd[ptr++] = alpha;
                  
                pd[ptr++] = offpos.x+b.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+b.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0 + 0.2*dlugosc;
                pd[ptr++] = alpha;

                pd[ptr++] = offpos.x+a1.x;  pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a1.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = -0.139000;      pd[ptr++] = -1.0;
                pd[ptr++] = alpha;
                    
                pd[ptr++] = offpos.x+a.x;   pd[ptr++] = 0.2; pd[ptr++] = offpos.y+a.y;
                pd[ptr++] = 0.0;            pd[ptr++] = 1.0; pd[ptr++] = 0.0;
                pd[ptr++] = 0.862000;       pd[ptr++] = -1.0 + 0.2*dlugosc;
                pd[ptr++] = alpha;
                
                //szyny
                   
                tx.set(-0.717500, 0.0);
                ty.set(-0.867500, 0.0);
              
                for( int jj = 0; jj < 2; jj++){
                    a.set(tx.x, 0.0);
                    b.set(ty.x, 0.0);
                    tx.rotate(angle, sections[i].radius*kierunek);
                    ty.rotate(angle, sections[i].radius*kierunek);
                    a.rotate(angle+aa, sections[i].radius*kierunek);
                    b.rotate(angle+aa, sections[i].radius*kierunek);
                    a.rotate(offrot, 0);
                    b.rotate(offrot, 0);      
                    tx.rotate(offrot, 0); 
                    ty.rotate(offrot, 0); 
                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+ty.x; sk[str++] = 0.325000; sk[str++] = offpos.y+ty.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.1330;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+a.x; sk[str++] =  0.325000; sk[str++] =  offpos.y+a.y;
                        sk[str++] = 0.0; sk[str++] =  1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] =  0.2270;
                        sk[str++] = alpha;

                        sk[str++] = offpos.x+tx.x; sk[str++] = 0.325000; sk[str++] = offpos.y+tx.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.0; sk[str++] = 0.2270;
                        sk[str++] = alpha;
                        
                        sk[str++] = offpos.x+b.x; sk[str++] = 0.325000; sk[str++] = offpos.y+b.y;
                        sk[str++] = 0.0; sk[str++] = 1.0; sk[str++] = 0.0;
                        sk[str++] = 0.2; sk[str++] = 0.1330;
                        sk[str++] = alpha;
                    ty.set(0.717500, 0.0);
                    tx.set(0.867500, 0.0);
                }
                ///
                tx.set(0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].radius*kierunek);
                a.rotate(angle+aa, sections[i].radius*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                tx.set(-0.717500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].radius*kierunek);
                a.rotate(angle+aa, sections[i].radius*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;

                tx.set(-0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].radius*kierunek);
                a.rotate(angle+aa, sections[i].radius*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;                
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;

                tx.set(0.867500, 0.0);
                a.set(tx.x, 0.0);
                tx.rotate(angle, sections[i].radius*kierunek);
                a.rotate(angle+aa, sections[i].radius*kierunek);
                a.rotate(offrot, 0); 
                tx.rotate(offrot, 0); 

                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.325; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.002;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;

                    sk[str++] = offpos.x+a.x; sk[str++] = 0.2; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+tx.x; sk[str++] = 0.2; sk[str++] = offpos.y+tx.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.0; sk[str++] = 0.07;
                    sk[str++] = alpha;
                    
                    sk[str++] = offpos.x+a.x; sk[str++] = 0.325; sk[str++] = offpos.y+a.y;
                    sk[str++] = 1.0; sk[str++] = 0.0; sk[str++] = 0.0;
                    sk[str++] = 0.069; sk[str++] = 0.002;
                    sk[str++] = alpha;
            }
            a.set(0.0, 0.0);
            a.rotate(sections[i].angle, sections[i].radius*kierunek);
            a.rotate(offrot, 0);
            offpos.x+=a.x; offpos.y+=a.y;
            offrot+=sections[i].angle;
        }
    }
    //qDebug() << ptr << "" << str;
    
    QString resPath = Game::root + "/routes/" + Game::route + "/textures";
    QString* texturePath = new QString(resPath.toLower()+"/acleantrack1.ace");
    shape.push_back(new OglObj());
    shape.push_back(new OglObj());
    shape[0]->setMaterial(texturePath);
    texturePath = new QString(resPath.toLower()+"/acleantrack2.ace");
    shape[1]->setMaterial(texturePath);
    shape[0]->init(pd, ptr, OglObj::VNT, GL_TRIANGLES );
    shape[1]->init(sk, str, OglObj::VNT, GL_TRIANGLES );
    
    float bound[6];
    bound[0] = -9999;
    bound[1] = 9999;
    bound[2] = -9999;
    bound[3] = 9999;
    bound[4] = -9999;
    bound[5] = 9999;
    for (int i = 0; i < ptr ; i+=9) {
        if(pd[i+0] < bound[1]) bound[1] = pd[i+0];
        if(pd[i+1] < bound[3]) bound[3] = pd[i+1];
        if(pd[i+2] < bound[5]) bound[5] = pd[i+2];
        if(pd[i+0] > bound[0]) bound[0] = pd[i+0];
        if(pd[i+1] > bound[2]) bound[2] = pd[i+1];
        if(pd[i+2] > bound[4]) bound[4] = pd[i+2];
    }
    for (int i = 0; i < str ; i+=9) {
        if(sk[i+0] < bound[1]) bound[1] = sk[i+0];
        if(sk[i+1] < bound[3]) bound[3] = sk[i+1];
        if(sk[i+2] < bound[5]) bound[5] = sk[i+2];
        if(sk[i+0] > bound[0]) bound[0] = sk[i+0];
        if(sk[i+1] > bound[2]) bound[2] = sk[i+1];
        if(sk[i+2] > bound[4]) bound[4] = sk[i+2];
    }
    
    shape[0]->setBound(bound);
    shape[1]->setBound(bound);
    
    delete[] pd;
    delete[] sk;
    
}