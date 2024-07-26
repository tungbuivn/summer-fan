#pragma once
#include <Arduino.h>
#include "macro.h"



//copy(Array.from(new Array(20)).map((o,i)=>`#define FOREACH_${i}(na,FN, E, ...) case na-${i}: FN;ip=na-${i}; break; FOREACH_${i-1}(na,E, __VA_ARGS__)`).slice(1).join("\n"))

// #define FOREACH_3(FN, E, ...) case N_VA_ARGS(__VA_ARGS__)+1: FN; break; FOREACH_2(E, __VA_ARGS__)
// #define FOREACH_4(FN, E, ...) case N_VA_ARGS(__VA_ARGS__)+1: FN; break; FOREACH_3(E, __VA_ARGS__)
// #define FOREACH_5(FN, E, ...) case N_VA_ARGS(__VA_ARGS__)+1: FN; break; FOREACH_4(E, __VA_ARGS__)
#define MERGE_(a,b)  a##b
#define LABEL_(a) MERGE_(autolabel_, a)
#define AUTO_LABEL_NAME LABEL_(__LINE__)


#define __FOREACH__(prio,FN, NARGS, ...) \
    if (ctx->finished) return; \
    switch (ctx->ip) { \
        FOREACH_##NARGS(prio,NARGS,FN, __VA_ARGS__ ) \
        default: \
            ctx->done=ctx->ip>MUL(NARGS,prio); \
            if  (ctx->done) { \
                if (ctx->type==THREAD_EXECUTION::FOREVER ) ctx->ip=-1; \
                else ctx->finished=1; \
            } \
        break; \
    } \
    ctx->ip+=1; 

#define __FOREACH_(prio,FN, NARGS, ...) __FOREACH__(prio,FN, NARGS, __VA_ARGS__)
#define TBT_THC(prio,FN, ...)  __FOREACH_(prio,FN, N_VA_ARGS(__VA_ARGS__), __VA_ARGS__)

// thread func
 
#define TBT_BLOCK(...) __VA_ARGS__
#define TBT_IF(cond,trueCond,falseCond) \
    if (!(cond)) goto PP_CONCAT(lbl__false_,__LINE__); ,trueCond,goto PP_CONCAT(lbl__end_,__LINE__),PP_CONCAT(lbl__false_,__LINE__): falseCond ,PP_CONCAT(lbl__end_,__LINE__):


#define TBT_WHILE(cond,...) TBT_BLOCK( \
    PP_CONCAT(lbl__while_,__LINE__): __VA_ARGS__, if (cond) goto PP_CONCAT(lbl__while_,__LINE__)\
)


#define __TBT_DELAY(FN,num)  ctx->time=FN; TBT_WHILE(FN-ctx->time<num,;)

#define TBT_DELAY(num)   __TBT_DELAY(millis(),num)

#define TBT_DELAY_MICRO(num) __TBT_DELAY(micros(),num)

#define TBT_WAIT_THREAD(th) TBT_WHILE(!(th.isFinished()),th.execute());
// #define H(X) HashString(__COUNTER__)



enum THREAD_EXECUTION {
    ONCE=1,
    FOREVER=2,
};
struct ThreadContext {
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

    /* when the last command execute then this value is true */
    bool done;
};

class BaseThread {
protected:
    
//     /**
//      * instruction pointer
//     */
//     int ip;
//     /**
//      * priority of the thread 1~N, max=maxInt/N
//     */
//     int priority;
//     /**
//      * isSuspend
//     */
//    bool isSuspend;
//    uint32_t time;
//    THREAD_EXECUTION type;
//    int finished;

//     /* when the last command execute then this value is true */
//     bool done;

public:
    ThreadContext *ctx;
    BaseThread();
    virtual ~BaseThread();
    int isFinished();
    virtual void init();
    /**
     * this is body of the thread, do not use any local variable here, if you want to, then create a function and call it
    */
    virtual  void execute()=0;
};


class ThreadFunc: public BaseThread {
    
public:
void execute();
};
// void BaseThread::execute() {
//     if (isSuspend) return;
// }
#define PARENTS ()
/* pass class to create context, function willbe call, and set of command before destroy, 
this only used to take out result of function,
do not do anything else */
#define TBT_CREATE_CONTEXT(var,clsName) TBT_BLOCK(static clsName *var=NULL,if (var==NULL) var=new clsName PARENTS;)
#define TBT_DESTROY_CONTEXT(var) delete var;var=NULL; 
#define TBT_EXECUTE_FUNC(clsName,var,func,...)  TBT_BLOCK( \
    TBT_CREATE_CONTEXT(var,clsName), \
    TBT_WHILE(!var->ctx->done, func(var)) \
    ,__VA_ARGS__, \
    TBT_DESTROY_CONTEXT(var))