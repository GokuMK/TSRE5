#include "Ruch.h"
#include "TRnode.h"
#include "TSectionDAT.h"

Ruch::Ruch(TDB *t) {
    this->trackDB = t;
}

void Ruch::set(int at){
    aktt = at;
    if(trackDB->trackNodes[aktt]!=NULL)
        if(trackDB->trackNodes[aktt]->typ!=0)
            aktt = -1;
    
    pozW.set(0,0,0);
    pozT.set(0,0,0);
    pozO.set(0,0,0);
    idx = -1;
}

void Ruch::toNext(float metry){
    metrpp = metry;
    while(metrpp!=0){
        next();
    }
}

bool Ruch::next(){
        if(aktt==-1) return true;
        
        switch(trackDB->trackNodes[aktt]->typ){
            case 0:
                //System.out.print("byles na endpoint "+aktt);
                
                pozW.x = trackDB->trackNodes[aktt]->UiD[6];
                pozW.y = trackDB->trackNodes[aktt]->UiD[7];
                pozW.z = trackDB->trackNodes[aktt]->UiD[8];
                pozT.x = (int) trackDB->trackNodes[aktt]->UiD[4];
                pozT.z = (int) trackDB->trackNodes[aktt]->UiD[5];
                
                kierunek = trackDB->trackNodes[aktt]->TrPinK[0];
                aktt = trackDB->trackNodes[aktt]->TrPinS[0];
                //System.out.println(" przejdziesz do "+aktt);
                
                akticz = 0;
                
                if(kierunek==1) {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[0].param[0];
                    metry = 0;
                } else {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[trackDB->trackNodes[aktt]->iTrv-1].param[0];
                    metry = trackDB->tsection->sekcja.at(idx)->getDlugosc();
                }

                return false;
            case 1:
                u = trackDB->trackNodes[aktt]->iTrv;
                if(kierunek==1){
                    metry+=metrpp;
                    metrpp = 0;
                    if(metry>=trackDB->tsection->sekcja.at(idx)->getDlugosc()){
                         metrpp = metry - trackDB->tsection->sekcja.at(idx)->getDlugosc();
                         akticz++;
                         metry = 0;
                    }
                    i = akticz;
                    
                    if(i==0){
                        //System.out.println("Wjechales na sciezke "+aktt+" z: "+ u +" czesciami. Jedziesz od poczatku do konca");
                    }
                    if(i<u){
                        //System.out.println("czesc" + i);
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[0];
                        pozW.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[10];
                        pozW.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[11];
                        pozW.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[12];
                        pozO.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[13];
                        pozO.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[14];
                        pozO.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[15];
                        pozT.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[8];
                        pozT.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[9];
                        return true;
                    }
                    
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][1] = Math.abs(kierunek-1);
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][0] = aktt;
                            
                    kierunek = trackDB->trackNodes[aktt]->TrPinK[1];
                    aktt = trackDB->trackNodes[aktt]->TrPinS[1];
                    //System.out.println("zjezdzasz z toru na " + aktt);
                    return false;
                }
                if(kierunek==0){
                    metry-=metrpp;
                    metrpp = 0;
                         
                    if(metry<=0){
                         metrpp = -metry;
                         akticz++;
                         //System.out.println("++ ");
                         i=u-akticz-1;
                         if(i>=0){
                            idx = trackDB->trackNodes[aktt]->trVectorSection[i].param[0];
                            metry=trackDB->tsection->sekcja.at(idx)->getDlugosc();
                         }
                    }
                    i=u-akticz-1;
                    
                    if(i==u-1){
                        //System.out.println("Wjechales na sciezke "+aktt+" z: "+ u +" czesciami. Jedziesz od konca do poczatku");
                    }
                    
                    if(i>=0){
                        //System.out.println("czesc" + i + " m " + metry);
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[0];
                        pozW.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[10];
                        pozW.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[11];
                        pozW.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[12];
                        pozO.x = trackDB->trackNodes[aktt]->trVectorSection[i].param[13];
                        pozO.y = trackDB->trackNodes[aktt]->trVectorSection[i].param[14];
                        pozO.z = trackDB->trackNodes[aktt]->trVectorSection[i].param[15];
                        pozT.x = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[8];
                        pozT.z = (int) trackDB->trackNodes[aktt]->trVectorSection[i].param[9];     
                        return true;
                    }
                    akticz = 0;
                    metry = 0;
                    
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][1] = Math.abs(kierunek-1);
                    //rozjazdy[trackDB.trackNodes[aktt].TrPinS[1]][0] = aktt;
                    
                    kierunek = trackDB->trackNodes[aktt]->TrPinK[0];
                    aktt = trackDB->trackNodes[aktt]->TrPinS[0];
                    //System.out.println("zjezdzasz z toru na " + aktt);
                    return false;
                }
                return false;
            case 2:
                if(kierunek==1){ u = trackDB->trackNodes[aktt]->TrP2;}
                if(kierunek==0){ u = trackDB->trackNodes[aktt]->TrP1;}
                akticz = 0;
                metry = 0;
                pozW.x = trackDB->trackNodes[aktt]->UiD[6];
                pozW.y = trackDB->trackNodes[aktt]->UiD[7];
                pozW.z = trackDB->trackNodes[aktt]->UiD[8];
                pozT.x = (int) trackDB->trackNodes[aktt]->UiD[4];
                pozT.z = (int) trackDB->trackNodes[aktt]->UiD[5];
                
                int kt, at;
                
                if(u==1){
                    //System.out.println("zjezdzasz z rozjazdu "+aktt);
                    kt = trackDB->trackNodes[aktt]->TrPinK[trackDB->trackNodes[aktt]->TrP1*kierunek];
                    at = trackDB->trackNodes[aktt]->TrPinS[trackDB->trackNodes[aktt]->TrP1*kierunek];
                    kierunek=kt; aktt=at;
                    
                    if(kierunek==1) {
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[0].param[0];
                        metry = 0;
                    } else {
                        idx = (int) trackDB->trackNodes[aktt]->trVectorSection[trackDB->trackNodes[aktt]->iTrv-1].param[0];
                        metry = trackDB->tsection->sekcja.at(idx)->getDlugosc();
                    }
                    
                    return false;
                }
                //System.out.println("rozjazd "+aktt);
                //if(rozjazdy[aktt][0]!=0){
                //    kt = rozjazdy[aktt][1];
                //    at = rozjazdy[aktt][0];
                //    if(koniec) rozjazdy[aktt][0] = 0;
                //} else {
                    //if(rozjazd){
                        //rozjazdy[aktt][1] = 
                kt = trackDB->trackNodes[aktt]->TrPinK[trackDB->trackNodes[aktt]->TrP1*kierunek];
                        //rozjazdy[aktt][0] = 
                at = trackDB->trackNodes[aktt]->TrPinS[trackDB->trackNodes[aktt]->TrP1*kierunek];
                    //} else {
                    //    rozjazdy[aktt][1] = kt = trackDB.trackNodes[aktt].TrPinK[trackDB.trackNodes[aktt].TrP1*kierunek+1];
                    //    rozjazdy[aktt][0] = at = trackDB.trackNodes[aktt].TrPinS[trackDB.trackNodes[aktt].TrP1*kierunek+1];
                    //}
                //}
                kierunek = kt; aktt = at;
                if(kierunek==1) {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[0].param[0];
                    metry = 0;
                } else {
                    idx = (int) trackDB->trackNodes[aktt]->trVectorSection[trackDB->trackNodes[aktt]->iTrv-1].param[0];
                    metry = trackDB->tsection->sekcja.at(idx)->getDlugosc();
                    //System.out.println("metry "+metry);
                }
                return false;
        }
        return false;
}

Vector3f* Ruch::getPosition(){
        
        if(aktt==-1) 
            return new Vector3f();
   
        float x = pozW.x;
        float y = pozW.y;
        float z = -pozW.z;
        
        if(trackDB->trackNodes[aktt]->typ==1){
            if(trackDB->tsection->sekcja[idx] == NULL)
                return new Vector3f(x,y,z);
            Vector3f* poss = trackDB->tsection->sekcja[idx]->getDrawPosition(metry);
            poss->rotate(pozO);
            return new Vector3f(x-poss->x, y+poss->y, z-poss->z);
        } else {
            return new Vector3f(x,y,z);
        }
    }

int Ruch::getAktTx(){
        return pozT.x;
    }
int Ruch::getAktTz(){
        return -pozT.z;
    }

