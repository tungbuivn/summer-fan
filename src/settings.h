#pragma once
#include "tbt_thread.h"
#include "events.h"
#include "pins.h"

using namespace std;
#define MAX_PWM_DUTY pow(2, PWM_RESOLUTION)

class Settings;
extern Settings *settings;
enum EN_FAN_AUTO {
    
    OFF,
    ON
    
};
enum SPEED_TYPE {
    PWM,
    PFM
};
class Settings : public BaseThread
{
    static bool __handler(EventData *dt) { return settings->handler(dt); };
    bool handler(EventData *dt);
    void handleGear(EncoderData *dt);

public:
    int temperature=0; 
    int sleepTime=60*3;
    int sleepRemain=60;
    long powerOff=60*60*10;
    long powerOffRemain=0;
    int swing=0;

    int lcdMaxBrightness=MAX_PWM_DUTY-1;
    int lcdBrightness=MAX_PWM_DUTY-1;
   

    int minTemperature=25;
    int maxTemperature=35;
    EN_FAN_AUTO autoGear=EN_FAN_AUTO::ON;
    int gear = 0;
    int maxGear = 20;

    
    bool inited = false;
    int pwmDuty();
    bool started = false;
    int minFreq=PFM_MIN_FREQUENCY;
    int maxFreq=PFM_MAX_FREQUENCY;
    // this will be calculate from gear, so dont touch it
    int pwmFrequency();
    SPEED_TYPE speedType=SPEED_TYPE::PFM;
    Settings();
    
    void execute() override;
    void handleSleep(ThreadData *sleepTh);
    void handleAutoSpeed(ThreadData *data);
    void handleAutoPowerOff(ThreadData *data);
    queue<int> soundState;
    void playSound(ThreadData *data);
    void resetSleep();
    // EEPROM
    int storage[10]={};
    int eepromError=0;
    uint8_t readByte( unsigned int add);
    void writeByte( unsigned int add, uint8_t data);
    void loadSettings();
    void saveSetting();
    void initData();
};
