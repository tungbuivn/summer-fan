#pragma once

#include "tbt_thread.h"
// #include <queue>

class LedBlink : public BaseThread
{
private:
    
    // int queueLen;
public:
    LedBlink();
    
    void execute() override;
};
// extern LedBlink *ledBlink;