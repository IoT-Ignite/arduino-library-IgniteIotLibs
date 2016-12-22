#ifndef _INCL_IGNITE_ESP_8266_MDNS
#define _INCL_IGNITE_ESP_8266_MDNS

#include "IgniteMdns.h"
#include <ESP8266mDNS.h>

class IgniteEsp8266MDNS : public IgniteMdns {

  public :

    IgniteEsp8266MDNS();
    ~IgniteEsp8266MDNS();

    virtual int queryService();
    virtual bool findGateway();

    String getMacAddress();

  private :
    String serviceName;
    int serviceCount;
};

#endif
