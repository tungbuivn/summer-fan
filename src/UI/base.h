#pragma once
#include "tbt_thread.h"
#include "encode2.h"
#include "../mylcd.h"
#include "../eventData.h"
#include <queue>
#include "base-sprite.h"
using namespace std;
class BasePage;
#define INIT_SPRITE(sp,x,y,w,h) \
        sp = new BaseSprite(x, y, w, h); \
        spQueue.push(sp); 


class BasePage: public BaseThread {
protected:
    bool isSuspend;
    queue<BaseSprite*> spQueue;
    // BasePage *lcd;
public:    
 bool ready=false;
    BasePage();
    bool drawTextSprite(uint16_t* buf,const char *text,int x,int y,int w,int h,int color);
    virtual bool render() { return false;};
    virtual bool handleEvent(EventData *dt)=0;
    void removeSprite();
   
    // virtual bool onButtonTrigger(EventData *bt) { return false;};
};