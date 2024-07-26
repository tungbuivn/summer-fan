// #pragma once
#include "tbt_thread.h"
BaseThread::BaseThread() {
   
   ip=0;
   priority=5;
   type=THREAD_EXECUTION::FOREVER;
   finished=0;
};
void BaseThread::reset() {
    ip=0;
    finished=0;
}
int BaseThread::isFinished() {
    return finished;
}