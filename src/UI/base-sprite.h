#pragma once
#include "TFT_eSPI.h"

#define CHECK_CHANGED(a,b,...) \
    if ((a)!=(b)) {\
        a=b;\
        __VA_ARGS__\
    }

#define DRAW_SPRITE(bsp,...) { \
        bsp->begin(0);\
        TFT_eSprite *sp = bsp->sp; \
        draw(bsp,sp,__VA_ARGS__); \
        bsp->end();\
    } 

#define DRAW_ROTATE_SPRITE(bsp,ang,...) { \
        bsp->begin(ang);\
        TFT_eSprite *sp = bsp->sp; \
        draw(bsp,sp,__VA_ARGS__); \
        bsp->end();\
    } 


class BaseSprite {
     
protected:
   
   
   
    // virtual void onDraw()=0;
public:
uint16_t *buf=NULL;
     int rotateAngle=0;
     int angle=0;
     bool isRotate=false;
    int x;
    int y;
    int w;
    int h;
    int dpx;
    int dpy;
    bool hasDp=false;
    int transparent;
    TFT_eSprite *sp=NULL;
    void setDrawPoint(int x,int y);
    BaseSprite(int x,int y,int w,int h);
    BaseSprite(int x,int y,int w,int h,int transparent);
    
    ~BaseSprite();
    void begin(int angle);
    void end();
};


