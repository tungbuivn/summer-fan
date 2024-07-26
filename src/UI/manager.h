#pragma once
#include "tbt_thread.h"
#include "home.h"
#include <vector>
#include "base.h"
#include "../mylcd.h"
#include "settings.h"
using namespace std;
class ManagerPage : public BaseThread {
 
  int page;
  vector<BasePage*> pages;
  static bool handler(EventData *);
 
  static bool gotoPage(EventData *);
  int isSleep=0;
  bool inited=false;
  
  
 
public:
  ManagerPage();
  void execute() override;
  BasePage *currentPage; 
  void init();

};
extern ManagerPage *man;