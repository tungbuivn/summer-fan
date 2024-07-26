#include "setup.h"
#include <cstdarg>
#include "base-sprite.h"
#include "../settings.h"
#include "../utils.h"

MenuItem::MenuItem(MENUTYPE atype, const char *adesc, const char *avalue)
{
    type = atype;
    strcpy(text, adesc);
    if (avalue)
    {
        strcpy(value, avalue);
    }
}

MenuItem::MenuItem(MENUTYPE atype, const char *adesc, const char *avalue, const char *alst[]) : MenuItem(atype, adesc, avalue)
{

    char buf[20];
    int j = 0;
    int len = sizeof(alst);

    for (int i = 0; i < len; i++)
    {
        const char *val = alst[i];
        char *buf = (char *)malloc(strlen(val));
        strcpy(buf, val);
        lst.push_back(buf);
    }
}
MenuItem::MenuItem(MENUTYPE atype, const char *desc, int aintValue, const long *alst) : MenuItem(atype, desc, "")
{
    char buf[15];
    itoa(aintValue, buf, 10);
    strcpy(value, buf);
    tmp = aintValue;
    int len = sizeof(alst);
    for (int i = 0; i < len; i++)
    {
        ltoa(alst[i], buf, 10);
        char *it = (char *)malloc(strlen(buf));
        strcpy(it, buf);
        lst.push_back(it);
    }
};

SetupPage::SetupPage() : BasePage()
{
    char buf[15];
    // go back home page
    mn.items.insert({ST_BACK, new MenuItem(MENUTYPE::MI_STRING, "Back", NULL)});

    // save to eeprom
    mn.items.insert({ST_SAVE, new MenuItem(MENUTYPE::MI_STRING, "Save", NULL)});

    // speed type select
    const char *pwmSelect[] = {"PWM", "PFM"}; // must match SPEED_TYPE enum
    mn.items.insert({ST_SPEED_TYPE, new MenuItem(MENUTYPE::MI_LIST, "Speed type", pwmSelect[settings->speedType], pwmSelect)});

    // power off time setting, 10 years =))
    const long powerOff[] = {0, 10 * 12 * 30 * 24 * 60 * 60};
    mn.items.insert({ST_AUTO_POWER_OFF, new MenuItem(MENUTYPE::MI_TIME_RANGE, "Auto power off", settings->powerOff, powerOff)});

    // screen turn off setting
    const long sleepRange[] = {0, 32000};
    auto sleepObj = new MenuItem(MENUTYPE::MI_TIME_RANGE, "Screen idle", settings->sleepTime, sleepRange);
    mn.items.insert({ST_SLEEP_COUNT_DOWN, sleepObj});

    // turn on/off fan auto by temperature, must match enum EN_FAN_AUTO
    const char *fanAuto[] = {"Off","On"};
    mn.items.insert({ST_FAN_AUTO, new MenuItem(MENUTYPE::MI_LIST, "Fan auto speed", fanAuto[settings->autoGear], fanAuto)});

    // number of gear
    const long gearRange[] = {0, 99};
    mn.items.insert({ST_FAN_GEARS, new MenuItem(MENUTYPE::MI_RANGE, "Speed counts", settings->maxGear, gearRange)});

    // lcd brightness
    const long brRange[] = {0, settings->lcdMaxBrightness};
    auto lcdBr = new MenuItem(MENUTYPE::MI_RANGE, "Lcd brightness", settings->lcdBrightness, brRange);
    mn.items.insert({ST_LCD_BRIGHTNESS, lcdBr});

    // min temp brightness
    const long rMinTemp[] = {0, 99};
    mn.items.insert({ST_MIN_TEMPERATURE, new MenuItem(MENUTYPE::MI_RANGE, "Min temp", settings->minTemperature, rMinTemp)});

    const long rMaxTemp[] = {0, 99};
    mn.items.insert({ST_MAX_TEMPERATURE, new MenuItem(MENUTYPE::MI_RANGE, "Max temp", settings->maxTemperature, rMaxTemp)});
}
void SetupPage::execute()
{
}
void SetupPage::drawMenuItem(int i, const char *avalue)
{
    int x = 0;
    int y = 0;
    int h = 35;
    int w = 240;
    auto draw = [](BaseSprite *bs, TFT_eSprite *sp, SetupPage *ts, int i, const char *avalue)
    {
        char buf[15];
        auto amn = ts->mn.items.at((ST_MENU_KIND)i);
        char *text = amn->text;
        bs->setDrawPoint(0, i * bs->h);
        if (i == ts->selected)
        {
            sp->fillSprite(TFT_BLUE);
        }

        sp->setFreeFont(FS9pt);
        sp->setTextDatum(ML_DATUM);
        sp->setTextColor(TFT_WHITE);
        sp->drawString(text, 10, bs->h / 2, 1);

        if (ts->isEdit)
        {
            sp->fillRect(152, 0, 88, bs->h, TFT_BROWN);
        }
        sp->setFreeFont(FS9pt);
        sp->setTextColor(TFT_WHITE);
        sp->setTextDatum(MR_DATUM);
        if (amn->type == MI_TIME_RANGE)
        {
            getTime(buf, amn->tmp, 0);
        }
        else if (amn->type == MI_RANGE)
        {

            ltoa(amn->tmp, buf, 10);
        }
        else
        {
            strcpy(buf, avalue == NULL ? amn->value : avalue);
            // sp->drawString(, 230, bs->h / 2, 1);
        }
        sp->drawString(buf, 230, bs->h / 2, 1);
    };

    DRAW_SPRITE(row, this, i, avalue);
}
bool SetupPage::render()
{
    auto tft = mylcd->tft;
    tft->fillScreen(TFT_BLACK);
    INIT_SPRITE(row, 0, 0, 240, 30);
    for (int i = 0; i < mn.items.size(); i++)
    {
        drawMenuItem(i, NULL);
    }
    return false;
};

/**
 * User rotate encode in edit mode
 */
void SetupPage::handleRotateEdit(EncoderData *ecd)
{
    int dirVar = (ecd->dir == ENCODER_DIRECTION::RIGHT ? 1 : -1);
    auto it = mn.items.at((ST_MENU_KIND)selected);
    auto lst = it->lst;
    int i;
    int j;
    int n;
    // char *lstVal;
    switch (it->type)
    {
    case MENUTYPE::MI_LIST:
        // find current item
        i = -1;
        n = it->lst.size() / 2;
        for (char *lstVal : it->lst)
        {
            i++;
           
            if (strcmp(it->value, lstVal) == 0)
            {
                j = ((int)abs(i + dirVar) % n);
              
                strcpy(it->value,lst.at(j));
                drawMenuItem(selected, NULL);
               
                break;
            }
        }

        break;
    case MENUTYPE::MI_TIME_RANGE:
    {
        long from = atoi(it->lst[0]);
        long to = atoi(it->lst[1]);
        long curr = it->tmp;
        curr = timeDistance(it->tmp, dirVar);
        debug_printf("time distance: %lu\n",curr);
        if ((curr >= from) && (curr <= to))
        {
            it->tmp = curr;
           
            drawMenuItem(selected, NULL);
        }
    }
    break;
    case MENUTYPE::MI_RANGE:
    {
        long from = atoi(it->lst[0]);
        long to = atoi(it->lst[1]);
        long curr = it->tmp;
       
        
        auto inc = selected == ST_LCD_BRIGHTNESS ? 5 : 1;
        curr += dirVar * inc;

            
        
        if ((curr >= from) && (curr <= to))
        {
           
            it->tmp = curr;
           
            drawMenuItem(selected, NULL);
        }
    }
    break;
    default:
        break;
    }
}
void SetupPage::handleEditDone()
{
    auto it = mn.items[(ST_MENU_KIND)selected];
    switch (selected)
    {

    case ST_SPEED_TYPE: // Speed type
    {

        int count = 0;

        for (auto v : it->lst)
        {
            if (strcmp(v, it->value) == 0)
            {
                settings->speedType = (SPEED_TYPE)count;
                eventSystem.dispatchMessage(EventType::GEAR_UPDATED, settings->speedType);
                break;
            }
            count++;
        }
    }
    break;
    case ST_LCD_BRIGHTNESS:
        settings->lcdBrightness = it->tmp;
        analogWrite(LCD_LED, settings->lcdBrightness);
        break;
    case ST_SLEEP_COUNT_DOWN: // Speed type
        settings->sleepTime = it->tmp;
       
        break;
    case ST_AUTO_POWER_OFF: // Speed type
        settings->powerOff = it->tmp;
        settings->powerOffRemain=settings->powerOff;
        break;
    case ST_FAN_GEARS: // Speed type
        settings->maxGear = it->tmp;
        if (settings->gear > settings->maxGear)
        {
            settings->gear = settings->maxGear;
        }
        eventSystem.dispatchMessage(EventType::GEAR_UPDATED, settings->gear);
        break;
    case ST_MIN_TEMPERATURE: // temp
        settings->minTemperature = it->tmp;
        if (settings->minTemperature > settings->maxTemperature)
        {
            settings->minTemperature = settings->maxTemperature;
        }
        break;
    case ST_MAX_TEMPERATURE: // temp
        settings->maxTemperature = it->tmp;
        if (settings->minTemperature > settings->maxTemperature)
        {
            settings->maxTemperature = settings->minTemperature;
        }
        break;
    case ST_FAN_AUTO:
    {
        int count = 0;
        for (auto v : it->lst)
        {
            if (strcmp(v, it->value) == 0)
            {
                settings->autoGear = (EN_FAN_AUTO)count;
                eventSystem.dispatchMessage(EventType::GEAR_UPDATED, settings->autoGear);
                break;
            }
            count++;
        }
    }
    break;
    }
}
void SetupPage::handleRotateClick(EncoderData *dt)
{
    auto it = mn.items[(ST_MENU_KIND)selected];
   

    if (selected == ST_BACK)
    {
       
        eventSystem.dispatchMessage(EventType::GOTO_PAGE, 0ll);
    } else if (selected==ST_SAVE) {
        settings->saveSetting();
    }
    else
    {
        isEdit = !isEdit;
        drawMenuItem(selected, NULL);
        // user done edit ?
        if (!isEdit)
        {
            handleEditDone();
        }
    }
}
bool SetupPage::handleEvent(EventData *dt)
{
    EncoderData *ecd = (EncoderData *)dt;
    int dirVar;
    
    switch (dt->type)
    {
    case EventType::ENCODER_ROTATE:
    {
        dirVar = ecd->dir == ENCODER_DIRECTION::RIGHT ? 1 : -1;
        if (!isEdit)
        {
            int len = mn.items.size();
            int sl = selected + dirVar;
            if ((sl >= 0) && (sl < len))
            {
                selected = sl;
                drawMenuItem(selected - dirVar, NULL);
                drawMenuItem(selected, NULL);
            }
        }
        else
        {
            handleRotateEdit((EncoderData *)dt);
        }
    }

    break;
    case EventType::ENCODER_CLICK:

        handleRotateClick((EncoderData *)dt);

        break;
    case EventType::RENDER:

        render();
        break;
    }
    return false;
}