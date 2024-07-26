#pragma once
#include "tbt_thread.h"
// #include "eventData.h"
#include "events.h"

using namespace std;
class SwingControl;
extern SwingControl *swctrl;
class SwingControl : public BaseThread
{
    static bool __handler(EventData *dt) { return swctrl->handler(dt);}
    bool handler(EventData *dt);
    unsigned long motTime;
    unsigned long lastDelay;
    int stop=0;
    unsigned long dt;

public:
    SwingControl();
    void execute() override;
    void spin(int b) ;
    unsigned long getTime() ;
};