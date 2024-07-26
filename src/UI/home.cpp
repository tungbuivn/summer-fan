#include "home.h"
#include "TFT_eSPI.h"
#include "../settings.h"
#include "../utils.h"
#include "rotate-icon.h"
// #include "sfui20.h"

HomePage::HomePage() : BasePage()
{
}

void HomePage::drawSwingThread(ThreadData *data)
{
    AUTO_THC(data,
             TBT_IF_TRUE(settings->started && settings->swing,
                         drawSwing(settings->swing),
                         TBT_DELAY(10), ))
}
/**
 * draw animation rotate fan
 */
void HomePage::drawOnOffThread(ThreadData *data)
{
    AUTO_THC(data,
             TBT_IF_TRUE(settings->started,
                         drawOnOff(settings->started),
                         TBT_DELAY(10), ))
}
void HomePage::drawSleepThread(ThreadData *data)
{
    AUTO_THC(data,
        drawSleep(),
        TBT_DELAY(1000),
        
    )
}
void HomePage::drawPowerOffThread(ThreadData *data)
{
    AUTO_THC(data,
        drawPowerOff(),
        TBT_DELAY(1000),
        
    )
}
void HomePage::execute()
{
    static ThreadData swingTh;
    static ThreadData onOffTh;
    static ThreadData sleepTh;
    static ThreadData powerTh;

    TBT_THC(5,
            drawSwingThread(&swingTh),
            drawOnOffThread(&onOffTh), 
            drawSleepThread(&sleepTh),
            drawPowerOffThread(&powerTh)
            )
}
bool HomePage::handleEvent(EventData *dt)
{
    
    if (!ready && dt->type != EventType::RENDER)
        return false;
    
    switch (dt->type)
    {
    case EventType::ENCODER_ROTATE:

        if (settings->started )
        {
            debug_printf("home rotated !\n");
            
            eventSystem.dispatchMessage(EventType::SETTINGS_UPDATE_GEAR, dt->data);
        }

        return false;

        break;
    case EventType::GEAR_UPDATED:
        if (settings->started)
        {
         
           lastFreq=-1;
           lastPwm=-1;
            drawGear(settings->gear);
            drawFreq(0);
            drawPwm(0);
        }
        return false;
        break;
    case EventType::ENCODER_CLICK:
    {
       
        eventSystem.dispatchMessage(EventType::GOTO_PAGE, 1);
        return true;
    }
    break;
    case EventType::RENDER:
        render();
        break;
    case EventType::PWM_UPDATED:
        if (settings->started)
        {
            drawFreq(settings->pwmFrequency());
            drawPwm(settings->pwmDuty());
        }

        break;
    case TEMPERATURE_UPDATED:
    case EventType::TEMPERATURE_CHANGE:
    {
        drawTemp(settings->temperature);
    }

    break;
    case EventType::ON_OFF_UPDATED:
    {
        int state = dt->data;
        debug_printf("draw on/off %d\n", state);
        if (state == 0)
        {
            drawGear(0);
            drawPwm(0);
        }
        else
        {
            drawGear(settings->gear);
            drawPwm(settings->pwmDuty());
        }
        drawFreq(settings->pwmFrequency());
        drawOnOff(state);
        drawSwing(0);
    }

    break;
    case EventType::SWING_UPDATED:
        drawSwing(settings->swing);
        break;
    default:
        break;
    }
    return false;
};
void HomePage::drawOnOff(int state)
{
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, int state)
    {
        if (state)
        {
            sp->pushImage(2,2,40,40,(uint16_t*)&rotate_img,16);
        }
        
    };
    

    DRAW_ROTATE_SPRITE(startStop, 15, settings->started);

}
void HomePage::drawSwing(int state)
{
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, int state)
    {
        if (settings->started && settings->swing)
        {
            sp->setFreeFont(FSI30pt);
            sp->setTextColor(TFT_GREEN);
            sp->setTextDatum(MC_DATUM);
            sp->drawString("1", bs->w / 2, bs->h / 2, 1);
        }
    };

    DRAW_ROTATE_SPRITE(swing, -15, settings->swing);
}
void HomePage::drawTemp(int tempVal)
{
    auto draw = [](BaseSprite *temp, TFT_eSprite *sp, int tempVal)
    {
        char buf[20] = {};
        // debug_printf("begin draw temp");
        sp->setFreeFont(FS9pt);
        sp->setTextColor(TFT_WHITE);
        sp->setTextDatum(MC_DATUM);
        sprintf(buf, "%d*C", tempVal);
        sp->drawString(buf, temp->w / 2, temp->h / 2, 1);
    };
    CHECK_CHANGED(lastTemp,settings->temperature,DRAW_SPRITE(temp, tempVal))
   
}

void HomePage::drawPwm(int pwm)
{
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, int pwm)
    {
        debug_printf("begin update pwm\n");
        sp->setFreeFont(FS9pt);
        sp->setTextColor(TFT_ORANGE);
        sp->setTextDatum(MC_DATUM);
        char buf[15];
        sprintf(buf, "%d%%", (int)(pwm * 100 / (MAX_PWM_DUTY - 1)));

        sp->drawString(buf, bs->w / 2, bs->h / 2, 1);
        debug_printf("done update pwm\n");
    };
    CHECK_CHANGED(lastPwm,settings->pwmDuty(),   
        DRAW_SPRITE(pwmSpeed, settings->pwmDuty())
    )
   
}
void HomePage::drawSleep()
{
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, int val,int old)
    {
        char buf[15];
        auto tft=mylcd->tft;
        tft->setFreeFont(FS9pt);
        tft->setTextDatum(ML_DATUM);
        tft->setTextColor(TFT_SKYBLUE);
        getTime(buf,old,0);
        tft->drawString(buf, 5, 24 / 2, 1);

        
        getTime(buf,val,0);
        tft->setTextColor(TFT_WHITE);
        tft->drawString(buf, 5, 24 / 2, 1);
       
    };

    int cap=settings->sleepRemain;
    if (lastSleep!=cap) {
        DRAW_SPRITE(sleepClk, cap,lastSleep)
        lastSleep=cap;
    }
    // auto sp = pwmSpeed->sp;
    
}
void HomePage::drawPowerOff()
{
  
    char buf[15];
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, char *buf,char *old)
    {
        debug_printf("draw poer off\n");
        auto tft=mylcd->tft;
        tft->setFreeFont(FS9pt);
        tft->setTextColor(TFT_SKYBLUE);
        tft->setTextDatum(MR_DATUM);
        tft->drawString(old, 240-5, 24 / 2);
        tft->setTextColor(TFT_WHITE);
        tft->drawString(buf, 240-5, 24 / 2);
        debug_printf("done draw poer off\n");
    };
    getTime(buf,settings->powerOffRemain,0);
    auto rqDraw=true;
    if (lastPowerOff[0]!=0) {
        if (strcmp(lastPowerOff,buf)==0) {
            rqDraw=false;
        }
    }
    if (rqDraw) {
        DRAW_SPRITE(powerClk,buf, lastPowerOff);
        strcpy(lastPowerOff,buf);
    }
}
void HomePage::drawGear(int gear)
{
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, int val,int old)
    {
        auto tft=mylcd->tft;
        debug_printf("Drawing gear\n");
        tft->setFreeFont(FSB45pt);
        tft->setTextDatum(MC_DATUM);
        tft->setTextColor(TFT_PURPLE);
        tft->drawNumber(old,bs->x+ 100 / 2,bs->y+ 74 / 2);
        
        tft->setTextColor(TFT_GREEN);
        tft->drawNumber(val,bs->x+ 100 / 2,bs->y+ 74 / 2);
        debug_printf("Done drawing gear:%d\n",val);
        
    };
    int speed=settings->started? settings->gear:0;
    if(lastGear!=speed) {
        if (lastGear==-1) lastGear=speed;
        DRAW_SPRITE(gearSpeed, speed,lastGear);
        lastGear=speed;
    }
        
    
  
}
void HomePage::drawFreq(int freq)
{
    
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, int val)
    {
       
        sp->setFreeFont(FS9pt);
        sp->setTextColor(TFT_WHITE);
        sp->setTextDatum(MC_DATUM);
        char buf[20] = {};
        sprintf(buf, "%d Hz",val);
        sp->drawString(buf, bs->w / 2, bs->h / 2, 1);
       
    };
    freq=settings->started? settings->pwmFrequency():
        (settings->speedType==PWM?settings->maxFreq:settings->minFreq);

    CHECK_CHANGED(lastFreq,freq,
        DRAW_SPRITE(freqSpeed, freq)
    )
   
}

bool HomePage::render()
{
    lastTemp = -1;
    lastPwm = -1;
    lastGear=-1;
    lastFreq = -1;
    lastPowerOff[0]=0;
    
   
    auto tft = mylcd->tft;
    tft->fillScreen(TFT_BLACK);

    tft->fillRect(33, 0, 174, 42, TFT_SKYBLUE);
    tft->fillSmoothCircle(0, 0, 64, TFT_SKYBLUE);
    tft->fillSmoothCircle(240, 0, 64, TFT_SKYBLUE);
    tft->fillSmoothCircle(120, 112, 100, TFT_WHITE);
    tft->fillSmoothCircle(120, 112, 90, TFT_PURPLE);

   
    tft->setTextDatum(MC_DATUM);
    tft->setTextColor(TFT_WHITE);
    tft->setFreeFont(FS9pt);
    
    // tft->loadFont(sfui20);
    tft->drawString(F("Speed"), 92 + 28, 35 + 17, 1);

    INIT_SPRITE(sleepClk, 0, 0, 1, 1);
    drawSleep();

    INIT_SPRITE(powerClk, 240, 0, 1, 1);
    drawPowerOff();

    INIT_SPRITE(gearSpeed, 70, 75, 1, 1);
    // INIT_SPRITE(gearSpeed, 70, 75, 100, 74);
    drawGear(settings->gear);

    INIT_SPRITE(pwmSpeed, 96, 163, 48, 26);
    drawPwm(settings->pwmDuty());


    // memory card icon
    tft->setFreeFont(FSI30pt);
    tft->setTextDatum(ML_DATUM);
    tft->setTextColor(settings->eepromError?TFT_RED: TFT_GREEN);
    tft->drawString("4", 188, 230,1);

    // temp icon
    tft->setFreeFont(FSI30pt);
    tft->setTextDatum(ML_DATUM);
    tft->setTextColor(TFT_BLUE);
    tft->drawString("2", 184, 264,1);

    INIT_SPRITE(temp, 184, 284, 46, 26);
    drawTemp(settings->temperature);
   
   
    tft->setFreeFont(FSI30pt);
    tft->setTextDatum(ML_DATUM);
    tft->setTextColor(TFT_YELLOW);
    tft->drawString("3", 10, 260);

    INIT_SPRITE(freqSpeed, 10, 284, 56, 26);
    drawFreq(settings->pwmFrequency());
    INIT_SPRITE(startStop, 95, 268, 44, 44);
    drawOnOff(settings->started);

    INIT_SPRITE(swing, 95, 218, 45, 45);
    drawSwing(settings->swing);
    return false;
};
