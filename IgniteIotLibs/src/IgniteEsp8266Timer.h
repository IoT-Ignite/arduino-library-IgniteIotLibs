#ifndef _INCL_IGNITE_ESP_8266_TIMER
#define _INCL_IGNITE_ESP_8266_TIMER

#include "IgniteThingTimer.h"
#include <Timer.h>

typedef void (*DataFunc)();

class IgniteEsp8266Timer : public IgniteThingTimer {

    public :

    IgniteEsp8266Timer(DataFunc dataFunc);
    IgniteEsp8266Timer();
    ~IgniteEsp8266Timer();
    virtual void init();
    virtual void update();
    virtual void stop();

    private :
    Timer timer;
    int timerInt;
    DataFunc dataFunc;

};


#endif
