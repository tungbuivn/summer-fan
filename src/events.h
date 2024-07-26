#pragma once
#include <vector>
#include <functional>
#include <map> 
#include <queue>
#include <functional>
#include "tbt_thread.h"
#include "eventData.h"

using namespace std;
// class EventData;
typedef  function<bool(EventData*)> FN_CALLBACK;
typedef  vector<FN_CALLBACK> LIST_CALLBACK;



class Events :public BaseThread {
private:
    queue<EventData*> eventQueue;
    std::map<EventType,LIST_CALLBACK*> callbacks;
    bool processCallback();


public:
    Events();
    void execute() override;
    void addListener(EventType evt, FN_CALLBACK callback);
    // void dispatchMessage(EventType evt,EventData* data);
    void dispatchMessage(EventData *data);
    void dispatchMessage(EventType event,long long adata);
    bool available();
};
extern Events eventSystem;