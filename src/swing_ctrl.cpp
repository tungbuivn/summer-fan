#include <Arduino.h>
#include "swing_ctrl.h"
#include "events.h"
#include "tbt_thread.h"
#include "settings.h"
#include "pins.h"
// #include <TinyStepper_28BYJ_48.h>
// TinyStepper_28BYJ_48 stepper;
#define MOTOR_MAX_RPM 7
#define STEP_PER_REVERLUTION (512)
#define MAX_RPM_MOTOR_CAN_SPIN 14
const uint8_t phaseCWList[] = {
    0b1000,
    0b1100,
    0b0100,
    0b0110,
    0b0010,
    0b0011,
    0b0001,
    0b1001};
// const uint8_t phaseCWList[] = {0b1000, 0b0100, 0b0010, 0b0001};
const int phaseCWLength = sizeof(phaseCWList);


const unsigned long MOTOR_1RPM_DELAY_TIME = 60 * 1e6 / (phaseCWLength * STEP_PER_REVERLUTION);
const unsigned long MOTOR_DELAY_TIME = MOTOR_1RPM_DELAY_TIME / MOTOR_MAX_RPM ;
const unsigned long MOTOR_MIN_DELAY_TIME = MOTOR_1RPM_DELAY_TIME / MAX_RPM_MOTOR_CAN_SPIN;
const long SLOPE=(MOTOR_DELAY_TIME-MOTOR_1RPM_DELAY_TIME);

SwingControl::SwingControl()
{
    pinMode(SWING_A1,OUTPUT);
    pinMode(SWING_A2,OUTPUT);
    pinMode(SWING_B1,OUTPUT);
    pinMode(SWING_B2,OUTPUT);
  
    eventSystem.addListener(EventType::SWING_UPDATED, SwingControl::__handler);
   
}
bool SwingControl::handler(EventData *dt)
{
  
    return false;
}
 
#define _GV(u) (b&u)==u
void SwingControl::spin(int b) {
    
    digitalWrite(SWING_A1,_GV(0b1000));
    digitalWrite(SWING_A2,_GV(0b0100));
    digitalWrite(SWING_B1,_GV(0b0010));
    digitalWrite(SWING_B2,_GV(0b0001));
   
   
    
}
/**
 * Start at 1 rpm and increase speed to target speed in duration 1s, by solve equaltion linear scale
 * when time is zero, start with min RPM delay
 * when time is 1second, start at TARGET speed
 * time unit are seconds
 * TARGET_SPEED=1RPM_DELAY+SLOPE*time
 */
unsigned long SwingControl::getTime() {
    if (motTime<=1e6) {
        long delayTime=(MOTOR_1RPM_DELAY_TIME+SLOPE*(motTime/1e6));
    
        if (delayTime<MOTOR_DELAY_TIME){
            return MOTOR_DELAY_TIME;
        } 
        // overload motor speed ? oh, no
        if (delayTime<MOTOR_MIN_DELAY_TIME) {
            return MOTOR_MIN_DELAY_TIME;
        }
            
        debug_printf("delay= %lu mottime=%lu\n",delayTime,motTime);
        return delayTime;
    }
    return MOTOR_DELAY_TIME;
    
}
void SwingControl::execute()
{
    static int pos=0;
    static int mt=0;
   
    
    TBT_THC(5,
        TBT_IF(settings->swing && settings->started && (settings->gear>0),
            TBT_BLOCK(
            pos=0,
            mt=getTime(),
            TBT_WHILE(pos<phaseCWLength,
                spin(phaseCWList[pos]),
                TBT_DELAY_MICRO(mt),
                pos=pos+1,
            ),
            motTime+=mt*phaseCWLength,
            digitalWrite(SWING_A1,LOW);
            digitalWrite(SWING_A2,LOW);
            digitalWrite(SWING_B1,LOW);
            digitalWrite(SWING_B2,LOW);
          
            ),
            TBT_BLOCK(
                motTime=0,
            )
        )
    )
}
