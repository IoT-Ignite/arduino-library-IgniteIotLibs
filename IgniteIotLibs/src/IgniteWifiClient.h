#ifndef _INCL_IGNITE_WIFI_CLIENT
#define _INCL_IGNITE_WIFI_CLIENT

#include "WString.h"

class IgniteWifiClient {

  public:

    virtual bool isConnected() = 0;
    virtual bool connect(String ip, int port) = 0;
    virtual void close() = 0;
    virtual String readStringUntil(char chr) = 0;
    virtual int write(String message) = 0;

};

#endif
