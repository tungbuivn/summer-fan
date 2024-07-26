
#include "tbt_thread.h"
BaseThread::BaseThread() {
    ctx=new ThreadContext();
    ctx->ip=0;
    ctx->priority=5;
    ctx->type=THREAD_EXECUTION::FOREVER;
    ctx->finished=0;
    ctx->done=0;
};
BaseThread::~BaseThread() {
    delete ctx;
}
int BaseThread::isFinished() {
    return ctx->finished;
}
void BaseThread::init() {
    
}
void ThreadFunc::execute() {
    
}