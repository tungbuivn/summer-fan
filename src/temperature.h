#pragma once
#include "tbt_thread.h"
#include "events.h"
#include "eventData.h"
using namespace std;


class TempAdc {
public:
    int temp;
    TempAdc *next;
    TempAdc(int val) {temp=val;next=NULL;}
};

class Temperature : public BaseThread {

    int tempCount=0;
    int sum=0;
    int avgTemp=0;
    TempAdc *first=NULL;
    TempAdc *last=NULL;
public:    
    // add temperature and return average value
    int pushTemp(int t);
    
    int getTemp();

    Temperature();
   
    void execute();
};