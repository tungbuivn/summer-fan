#include "buzzer.h"
#include "pins.h"

Buzzer::Buzzer() {
    pinMode(BUZZER, OUTPUT);
    type=THREAD_EXECUTION::ONCE;
}

void Buzzer::execute2() {
}

void Buzzer::execute() {
    TBT_THC(5,

    )
};
