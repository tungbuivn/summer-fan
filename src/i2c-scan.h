#pragma once

#include "tbt_thread.h"
#include <queue>

class I2CScan : public BaseThread
{
private:
    
    // int queueLen;
public:
  
    I2CScan();
    void execute();
};
extern I2CScan i2cscan;