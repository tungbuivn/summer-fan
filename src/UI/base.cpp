#include "base.h"
BasePage::BasePage() {
   isSuspend=false;
}
bool BasePage::drawTextSprite(uint16_t* buf,const char *text,int x,int y,int w,int h,int color) {
    TFT_eSprite *sp;
    if (mylcd && mylcd->tft)
    {
        sp = new TFT_eSprite(mylcd->tft);
    }
   
    uint16_t transparent = 0; // The transparent colour, can only be 1 or 0

    sp->setColorDepth(1); // Set colour depth first

    sp->createSprite(w, h); // then create the sprite
                                //    tft->readRect(178, 264,  30,  30, tempBuf);
    sp->fillSprite(transparent);

    sp->setFreeFont(FS9pt);
    sp->setTextColor(1);
    sp->setTextDatum(MC_DATUM);
    sp->drawString(text, w/2, h/2, 1);

    sp->setBitmapColor(color, transparent);
    mylcd->tft->pushRect(x, y, w, h, buf);
   
    sp->pushSprite(x, y, transparent);

    sp->deleteSprite();
    delete sp;
}
void BasePage::removeSprite() {
    while (spQueue.size()>0)
    {
        auto sp=spQueue.front();
        delete sp;
        // sp=NULL;
        spQueue.pop();
        debug_printf("free sprite\n");
    }
    
}