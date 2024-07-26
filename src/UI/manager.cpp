#include "manager.h"
#include "../events.h"
#include "../buttons.h"
#include "setup.h"


ManagerPage::ManagerPage()
{
   
   

    page = 0;

}
void ManagerPage::init()
{
    pages.assign({
        // new SetupPage(),
                  new HomePage(),
                  new SetupPage()}
                  );
    eventSystem.addListener(EventType::GOTO_PAGE, ManagerPage::gotoPage);
     eventSystem.addListener(EventType::SLEEP, ManagerPage::handler);

    eventSystem.addListener(EventType::ENCODER_ROTATE, ManagerPage::handler);
    eventSystem.addListener(EventType::ENCODER_CLICK, ManagerPage::handler);
    eventSystem.addListener(EventType::RENDER, ManagerPage::handler);
    eventSystem.addListener(EventType::PWM_UPDATED, ManagerPage::handler);
    eventSystem.addListener(EventType::GEAR_UPDATED, ManagerPage::handler);
    eventSystem.addListener(EventType::TEMPERATURE_CHANGE, ManagerPage::handler);
    eventSystem.addListener(EventType::TEMPERATURE_UPDATED, ManagerPage::handler);

    
    eventSystem.addListener(EventType::ON_OFF_UPDATED, ManagerPage::handler);
    eventSystem.addListener(EventType::SWING_UPDATED, ManagerPage::handler);
    currentPage = pages[0];
    inited=true;
}
bool ManagerPage::handler(EventData *dt)
{
    if (!dt)
        return false;
    bool rs;
    if (man)
    {
        if (dt->type == EventType::SLEEP) {
            man->isSleep=1;
            return false;
        }
         // donot render while lcd is sleep mode
        if (man->isSleep) {
            return false;
        }
       
        if (dt->type == EventType::RENDER || man->currentPage->ready)
        {
            
            rs = man->currentPage->handleEvent(dt);
            man->currentPage->ready = true;
        }

        return rs;
    }
    return false;
}

void ManagerPage::execute()
{
    if (!inited) return;
    static BasePage *pg = NULL;
    TBT_THC(1,

            pages.at(page)->execute(),

    )
}

bool ManagerPage::gotoPage(EventData *dt)
{
    if (!man)
        return false;
    man->currentPage->ready = false;
    man->currentPage->removeSprite();
  
    man->page = dt->page;
    man->isSleep=0;
    man->currentPage = man->pages.at(dt->page);
   
    // EventEmpty ev;
    eventSystem.dispatchMessage(EventType::RENDER, 0ll);
   
    return false;
}