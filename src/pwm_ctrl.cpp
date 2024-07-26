#include <Arduino.h>
#include "pwm_ctrl.h"
#include "tbt_thread.h"
#include "mylcd.h"
#include "pins.h"
#include "events.h"
#include "buttons.h"
#include "settings.h"

PWMControl::PWMControl()
{

   
    this->trg = 0;
  
    this->duty = 0;
    pinMode(PWM_BLDC, OUTPUT);
  
    analogWriteFrequency(settings->pwmFrequency());
    analogWrite(PWM_BLDC, 0);
    /**
     * The on/off button can be trigger by hardware or software from lcd
     */
    eventSystem.addListener(EventType::ON_OFF_UPDATED, PWMControl::onOff);
    eventSystem.addListener(EventType::PWM_UPDATED, PWMControl::updatePWM);
    eventSystem.addListener(EventType::GEAR_UPDATED, PWMControl::updatePWM);
}
bool PWMControl::updatePWM(EventData *adt)
{
   
    if (settings->started)
    {
      
        analogWriteFrequency(settings->pwmFrequency());
        analogWrite(PWM_BLDC, settings->pwmDuty());
            
        debug_printf("pwm: %d, freq: %d\n", settings->pwmDuty(), settings->pwmFrequency());
    }
    else
    {
        analogWrite(PWM_BLDC, 0);
    }

    return false;
}
bool PWMControl::onOff(EventData *dt)
{

    updatePWM(dt);

    return false;
}
void PWMControl::execute2()
{
}
void PWMControl::execute() {

   
    TBT_THC(1,

         

            )
};