
#ifndef _INCL_IGNITE_ESP8266_HOTSPOT
#define _INCL_IGNITE_ESP8266_HOTSPOT

#include "IgniteHotspot.h"



class IgniteEsp8266Hotspot : public IgniteHotspot {

  public :
    IgniteEsp8266Hotspot();
    ~IgniteEsp8266Hotspot();

    virtual void startHotspot();

    bool isHotspotStarted();


  private :

    bool isStarted;


};


#endif
