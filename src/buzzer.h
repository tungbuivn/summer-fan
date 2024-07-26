#pragma once

#include "tbt_thread.h"
#include <queue>

class Buzzer : public BaseThread
{
private:
    
    // int queueLen;
public:
    Buzzer();

    void execute2();

    void execute() override;
};
