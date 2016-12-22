#ifndef _INCL_IGNITE_MDNS
#define _INCL_IGNITE_MDNS

#include "WString.h"

class IgniteMdns {

  public:
    virtual int queryService() = 0;
    virtual bool findGateway() = 0;

    String getGatewayIp();
    int getGatewayPort();
    void setGatewayId(String gatewayId);

  protected :
    String service;
    String proto;
    String gatewayId;
    String gatewayIp;
    int gatewayPort;
};


#endif
