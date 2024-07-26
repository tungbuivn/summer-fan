#pragma once
#include "tbt_thread.h"
#include <queue>
#include <string>
#include "events.h"
#include "eventData.h"

using namespace std;

class CommandAction:public BaseThread {
   
   private:
   std::string cmd;
public:
    CommandAction(std::string s);
    ~CommandAction();
    void execute();
};

class SerialCmd : public BaseThread
{
private:

    vector<char> data;
    void processCommand();
    void handleTask();
    void readSerial();
    // int cmdCount;
   
    // CommandAction *act;
  
public:
    SerialCmd();
    
    void execute();
    
};
extern SerialCmd mySerial;