#include "base-sprite.h"
#include "mylcd.h"
#include "TFT_eSPI.h"
BaseSprite::BaseSprite(int ax, int ay, int aw, int ah) {
    x = ax;
    y = ay;
    w = aw;
    h = ah;
    transparent=TFT_BLACK;
}

BaseSprite::BaseSprite(int ax, int ay, int aw, int ah, int atransparent): BaseSprite(ax,ay,aw,ah)
{
   
    transparent = atransparent;
    
}
void BaseSprite::begin(int aangle)
{
    isRotate=angle!=0;
    angle=aangle;
    if (!buf) {
        buf = (uint16_t *)malloc(((w) + 2) * ((h) + 2) * 2);
        mylcd->tft->readRect(x, y, w, h, buf);
    }
    if (!sp) {
        sp = new TFT_eSprite(mylcd->tft);
    }
    
    sp->setColorDepth(8); // Set colour depth first
    sp->createSprite(w, h); // then create the sprite
    sp->fillSprite(transparent);
    
}
void BaseSprite::setDrawPoint(int x,int y) {
    hasDp=true;
    dpx=x;
    dpy=y;
};
void BaseSprite::end() {
    int pvx;
    int pvy;
    int rx=x;
    int ry=y;
    
    if (hasDp) {
        rx=dpx;
        ry=dpy;
    }
    mylcd->tft->pushRect(rx, ry, w, h, buf);
         
    if (!isRotate) 
        sp->pushSprite(rx, ry, transparent);
    else {
        pvx= mylcd->tft->getPivotX();
        pvy= mylcd->tft->getPivotY();
        mylcd->tft->setPivot(rx+w/2,ry+h/2);
      
        sp->pushRotated(rotateAngle,transparent);
        mylcd->tft->setPivot(pvx,pvy);
        rotateAngle+=angle;
        if (rotateAngle>360) rotateAngle-=360;
        if (rotateAngle<-360) rotateAngle+=360;
    }
        
    sp->deleteSprite();
  
        
   
}
BaseSprite::~BaseSprite() {
    if (sp) delete sp;
    if (buf) free(buf);
}