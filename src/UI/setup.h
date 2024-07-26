
#pragma once
#include "base.h"
#include "base-sprite.h"
#include <map>
using namespace std;
enum ST_MENU_KIND {
    ST_BACK,
    ST_FAN_AUTO,
    ST_FAN_GEARS,
    ST_SPEED_TYPE,
    ST_SLEEP_COUNT_DOWN,
    ST_AUTO_POWER_OFF,
    ST_LCD_BRIGHTNESS,
    ST_MIN_TEMPERATURE,
    ST_MAX_TEMPERATURE,
    ST_SAVE,
};
enum MENUTYPE {
    MI_STRING,
    MI_INTEGER,
    MI_LIST,
    MI_RANGE,
    MI_TIME_RANGE
};
class MenuItem {
public:
    char text[15]={};
    char value[15]={};
    long tmp=0;
    // using to scale step when edit range value
    int stepScale=1;
  
    MENUTYPE type;
   
    vector<char*> lst;
    MenuItem(MENUTYPE atype,const char *desc,const char *value);
    MenuItem(MENUTYPE atype,const char *desc,const char *value,const char **lst);
    MenuItem(MENUTYPE atype,const char *desc,int aintValue,const long lst[]);
};
class MainMenu {
public:
    std::map<ST_MENU_KIND,MenuItem*> items;
};
class SetupPage : public BasePage
{
    BaseSprite *row=NULL;
    void drawMenu();
    void drawMenuItem(int i,const char *avalue);
public:
    int selected=0;
    int isEdit=0;
    
    MainMenu mn;
    SetupPage();
    void execute() override;
    bool render() override;
    bool handleEvent(EventData* dt) override;
    void handleRotateEdit(EncoderData *ecd);
    void handleRotateClick(EncoderData *dt);
    void handleEditDone();
};