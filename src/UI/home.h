#pragma once
#include "tbt_thread.h"
#include "base.h"
#include "TFT_eSPI.h"
#include "base-sprite.h"

class HomePage : public BasePage
{

    BaseSprite *pwmSpeed = NULL;
    BaseSprite *gearSpeed = NULL;
    BaseSprite *startStop = NULL;
    BaseSprite *swing = NULL;
   
    BaseSprite *sleepClk = NULL;
    BaseSprite *powerClk = NULL;
    BaseSprite *temp = NULL;
    BaseSprite *freqSpeed = NULL;
   

    int lastTemp = -1;
    int lastPwm = -1;
    int lastGear = -1;
    int lastFreq = -1;
    int lastSleep=-1;
    char lastPowerOff[15]={};
    void drawSleep();
    void drawPwm(int pwmValue);
    void drawGear(int gear);
    void drawTemp(int tempVal);
    void drawFreq(int afreq);
    void drawOnOff(int state);
    void drawSwing(int state);
    void drawPowerOff();
    void drawSwingThread(ThreadData *data) ;
    void drawOnOffThread(ThreadData *data) ;
    void drawSleepThread(ThreadData *data);
    void drawPowerOffThread(ThreadData *data);

public:
    HomePage();
    bool render() override;

    void execute() override;
    bool handleEvent(EventData *dt) override;

};