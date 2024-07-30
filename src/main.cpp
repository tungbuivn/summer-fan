#include <Arduino.h>
#include <Wire.h>
// #include <Adafruit_SPIDevice.h>
// #include <Adafruit_BusIO_Register.h>
#include "mylcd.h"
#include <USBSerial.h>
#include "led-blink.h"
#include "serialcmd.h"
#include "pwm_ctrl.h"
#include "encode2.h"
#include "buzzer.h"
#include "temperature.h"
#include "events.h"
#include "buttons.h"
#include "UI/manager.h"
#include "settings.h"
#include "swing_ctrl.h"


Events eventSystem;
Settings *settings = NULL;
LCDThread *mylcd = NULL;
SwingControl *swctrl;
ManagerPage *man = NULL;
vector<BaseThread *> listThread;
int len;

void setup()
{

    Serial.begin(115200);
    // delay(2000);

    settings = new Settings();
    listThread.assign({&eventSystem,
    // new I2CScan(),
                       new Buttons(),
                       new EncoderRotate(),
                       new EncoderClick(),
                       new PWMControl(),
                       new Temperature(),
                       mylcd = new LCDThread(),
                       new LedBlink(),
                       new Buzzer(),
                       new SerialCmd(),
                       swctrl = new SwingControl(),
                       man=new ManagerPage(),
                       settings

    });
    len = listThread.size();

   
};

void loop()
{

    // eventSystem.execute();
    for (int i = 0; i < len; i++)
    {
        // debug_printf("execute %d\n",i);
        listThread.at(i)->execute();
        // delay(200);
    }
}