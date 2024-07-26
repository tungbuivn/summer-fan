#pragma once
#include "tbt_thread.h"
#include <queue>
#include "events.h"
#include "eventData.h"

using namespace std;



class EncoderClick : public BaseThread {
private:
    // vector<std::function<void()>> clicks;
public:
    void execute();
    // void onClick(std::function<void()> callback);
   
};
enum Stage
{
   
    lblLoop,
    lblCW,
    lblCW1,
    lblCW2,
    lblCW3,
    lblCCW,
    lblCCW1,
    lblCCW2,
    lblCCW3,
  

};
class EncoderRotate : public BaseThread
{
private:
    int currentA;
    int currentB;
   
    Stage phase;
  
    queue<EncoderData*> ecdata;
    bool countRotate(int ra,int rb);
    queue<int> data;
    static void triggerA();
    static void triggerB();
    void handleEncoderRaw(ThreadData *data);
    void handleEncoderInterupt(ThreadData *data);
public:
    
    EncoderRotate();
    void execute() override;
   
   
};
