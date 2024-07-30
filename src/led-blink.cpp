#include "led-blink.h"

LedBlink::LedBlink() {
     pinMode(LED_BUILTIN, OUTPUT);
}
void LedBlink::execute(){
 
    TBT_THC(5,
    ,,
        // Serial.println("Begin blink led"),
        digitalWrite(LED_BUILTIN,HIGH),
        TBT_DELAY(500),
        // Serial.println("End blibk led"),
        digitalWrite(LED_BUILTIN,LOW),
        TBT_DELAY(500),
    )
};