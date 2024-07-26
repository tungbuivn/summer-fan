#pragma once
#include <Arduino.h>
#include "macro.h"

// #define DEBUG_PRINT 1
#ifdef DEBUG_PRINT
#define debug_printf(fmt, ...) \
    if (DEBUG_PRINT) { \
        auto p_time_now = millis(); \
        Serial.printf("%u ms " fmt, p_time_now, ##__VA_ARGS__); \
    }

#else
    #define debug_printf(fmt, ...) ;
#endif

//copy(Array.from(new Array(20)).map((o,i)=>`#define FOREACH_${i}(na,FN, E, ...) case na-${i}: FN;ip=na-${i}; break; FOREACH_${i-1}(na,E, __VA_ARGS__)`).slice(1).join("\n"))

// #define FOREACH_3(FN, E, ...) case N_VA_ARGS(__VA_ARGS__)+1: FN; break; FOREACH_2(E, __VA_ARGS__)
// #define FOREACH_4(FN, E, ...) case N_VA_ARGS(__VA_ARGS__)+1: FN; break; FOREACH_3(E, __VA_ARGS__)
// #define FOREACH_5(FN, E, ...) case N_VA_ARGS(__VA_ARGS__)+1: FN; break; FOREACH_4(E, __VA_ARGS__)


#define __FOREACH__(prio,FN, NARGS, ...) \
    if (finished) return; \
    switch (ip) { \
        FOREACH_##NARGS(prio,NARGS,FN, __VA_ARGS__ ) \
        default: \
            if  (ip>MUL(NARGS,prio)) { \
                if (type==THREAD_EXECUTION::FOREVER ) ip=-1; \
                else finished=1; \
            } \
        break; \
    } \
    ip=ip+1;

#define __FOREACH_(prio,FN, NARGS, ...) __FOREACH__(prio,FN, NARGS, __VA_ARGS__)
// max 1000 param in thc contents
#define TBT_THC_WRAP(prio,FN, ...)  __FOREACH_(prio,FN, N_VA_ARGS(__VA_ARGS__), __VA_ARGS__)
#define TBT_THC(prio, ...)  TBT_THC_WRAP(prio,TBT_BLOCK(),TBT_BLOCK(__VA_ARGS__))

#define CONCATENATE(s1, s2) s1##s2

#define TBT_BLOCK(...) __VA_ARGS__

#define TBT_IF_FALSE(cond,...) TBT_IF((!(cond)),TBT_BLOCK(__VA_ARGS__),TBT_BLOCK())

#define TBT_IF_TRUE(cond,...) TBT_IF(cond,TBT_BLOCK(__VA_ARGS__),TBT_BLOCK())



#define TBT_IF_WRAP(labelNum,cond,trueCond,falseCond) \
    if (!(cond)) goto CONCATENATE(lbl__false_,labelNum); \
    ,trueCond,goto CONCATENATE(lbl__end_,labelNum) \
    ,CONCATENATE(lbl__false_,labelNum): falseCond \
    ,CONCATENATE(lbl__end_,labelNum):

#define TBT_IF(cond,trueCond,falseCond) \
    TBT_IF_WRAP(__LINE__,TBT_BLOCK(cond),TBT_BLOCK(trueCond),TBT_BLOCK(falseCond))
    
#define TBT_WHILE_WRAP(labelNum,cond,body) \
    TBT_BLOCK( \
        CONCATENATE(lbl__while_start_, labelNum): \
        if (!(cond)) goto CONCATENATE(lbl__while_end_, labelNum);\
        body, \
        goto CONCATENATE(lbl__while_start_, labelNum); \
        CONCATENATE(lbl__while_end_, labelNum): \
    )

#define TBT_WHILE(cond,...) \
    TBT_WHILE_WRAP(__LINE__,TBT_BLOCK(cond),TBT_BLOCK(__VA_ARGS__))

#define __TBT_DELAY(FN,num)  time=FN; TBT_WHILE(FN-time<num,;)

#define TBT_DELAY(num) __TBT_DELAY(millis(),num)

#define TBT_DELAY_MICRO(num) __TBT_DELAY(micros(),num)

#define TBT_WAIT_THREAD(th) TBT_WHILE(!(th.isFinished()),th.execute());

enum THREAD_EXECUTION {
    ONCE=1,
    FOREVER=2,
};
class ThreadData {
public:
    int ip=0;
    uint32_t time;
    THREAD_EXECUTION type=THREAD_EXECUTION::FOREVER;
    int finished=0;
};

#define AUTO_THC(data,...) \
    int ip=data->ip; \
    int time=data->time; \
    THREAD_EXECUTION type=data->type; \
    int finished=data->finished; \
    TBT_THC(1, \
        TBT_BLOCK(__VA_ARGS__) \
    ) \
    data->ip=ip; \
    data->time=time;
class BaseThread {
protected:
   
    /**
     * instruction pointer
    */
    int ip;
    /**
     * priority of the thread 1~N, max=maxInt/N
    */
    int priority;
    /**
     * isSuspend
    */
    bool isSuspend;
    uint32_t time;
    THREAD_EXECUTION type;
    int finished;

public:
    BaseThread();
    // virtual ~BaseThread();
    int isFinished();
    void reset();
    /**
     * this is body of the thread, do not use any local variable here, if you want to, then create a function and call it
    */
    virtual  void execute()=0;
    // void execute2()=0;
    
    // virtual void execute2()=0;
};
// typedef int (*ThreadCallBack)();
// class ThreadAction : public BaseThread {
// private:
//     ThreadCallBack cb
// public:
//     ThreadAction(ThreadCallBack cb);
// }
// void BaseThread::execute() {
//     if (isSuspend) return;
// }
