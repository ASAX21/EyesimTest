#include "eyebot.h"
#include <stdio.h>
#include <math.h>
#include <pthread.h>

int x,y,phi;
int linSpeed, angSpeed;
int done = 0, stop = 1;

void ReturnDistance(){
     VWGetPosition(&x, &y, &phi);
     VWGetSpeed(&linSpeed, &angSpeed);
     LCDPrintf("position is %d,%d,%d\n",x,y,phi);
     LCDPrintf("PSDGet: left = %d, front = %d, right = %d\n",PSDGet(1),PSDGet(2),PSDGet(3));
     LCDPrintf("PSDGetRaw: left = %d, front = %d, right = %d\n",PSDGetRaw(1),PSDGetRaw(2),PSDGetRaw(3)); 
     LCDPrintf("linSeed=%d angSpeed=%d\n", linSpeed, angSpeed);
     return;
 }

void Turnning(int cases){
     int a,b,c;
     VWSetSpeed(0,0);
     if(cases == 1){
        do{
            a = PSDGet(1);
            VWTurn(-1, 90); 
            b = PSDGet(2);
            c = PSDGet(1);
        }while(b<500 || a>c);
        VWTurn(-1, 90);
        return;
     }
}
void *slave(void *arg){
     do{
        if(!stop){
            VWSetSpeed(300,0);
            do{ 
                if(PSDGet(1)<=150||PSDGet(2)<=200||PSDGet(3)<=150){
                    Turnning(1);
                    break;
                }
            }while(1);
        }
        OSWait(1000);
     }while(1);  
    VWSetSpeed(0,0);   
    return NULL;  
 }

void *master(void *arg){
    int k;
    LCDMenu("GO", "Stop", "Check","Exit");
    do{
        switch(k = KEYGet()) {
        case KEY1: stop = 0;
                    break;
        case KEY2: stop = 1;
                    VWSetSpeed(0,0);
                    break;
        case KEY3: ReturnDistance();
                    break;
        }
    }while(k != KEY4);
    return NULL;
}
int main(){   
  pthread_t t1, t2;

  XInitThreads();
  pthread_create(&t2, NULL, slave, (void *) 1);
  pthread_create(&t1, NULL, master, (void *) 0);

  pthread_exit(0);  // will temrinate program
}