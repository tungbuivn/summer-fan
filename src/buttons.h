#pragma once

#include "tbt_thread.h"
#include "events.h"
#include "pins.h"
#include "eventData.h"
// #include <queue>


class Buttons : public BaseThread
{
private:
  
public:
    Buttons();
    
    void execute() override;
};
