#pragma once
#include "events.h"
// #include "encode2.h"
#include <string>
using namespace std;
enum EventType {
    NONE,
    SERIAL_CMD,
    ENCODER_CLICK,
    ENCODER_ROTATE,
    TEMPERATURE_CHANGE,
    BUTTON_TRIGGER,
   
    GOTO_PAGE,
    RENDER,
    SLEEP,
    // trigger from home page
    SETTINGS_UPDATE_PWM,
    SETTINGS_UPDATE_GEAR,

    // trigger when pwm value changed
    PWM_UPDATED,
    GEAR_UPDATED,
    ON_OFF_UPDATED,
    SWING_UPDATED,
    TEMPERATURE_UPDATED,
   
   
};

enum ENCODER_DIRECTION {
    NONE_DIRECTION,
    LEFT,
    RIGHT
};
enum BUTTON_STATE {
    BTN_NONE,
    BTN_FAN,
    BTN_SWING
};

class EventData {

public:
    EventType type;
    union
    {
        int pwm;
        int temperature;
        ENCODER_DIRECTION dir;  
        int gear;
        int page;
        BUTTON_STATE btn;
        long long data=0;
    };
    EventData(EventType atype,long long adata) { type=atype;data=adata;}
    // EventData(EventData *aobj) {type=aobj->type;data=aobj->data;}
    virtual ~EventData();
    
};


class EncoderData:public EventData {
public:
    
    uint32_t time;
    EncoderData(EventType atype,long long adata,uint32_t atime):EventData(atype,adata){time=atime;}
   
};


class SerialData : public EventData {
public:
    string str;
    SerialData(string dt):EventData(EventType::SERIAL_CMD,0){str.assign(dt);}
 
};
