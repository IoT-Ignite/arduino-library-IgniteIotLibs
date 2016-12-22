#ifndef _INCL_IGNITE_WIFI_NODE_CONFIG
#define _INCL_IGNITE_WIFI_NODE_CONFIG

#include "WString.h"

class IgniteWifiNodeConfig {

  public:

    virtual String read() = 0;
    virtual bool write(String str) = 0;
    virtual void setup();
    virtual bool writeConfigs();

    void setGatewayId(String gwId);
    String getGatewayId();
    void setWifiCredentials(String ssid, String password);
    String getSSID();
    String getPassword();
    void setNodeId(String nodeId);
    String getNodeId();
    void setState(String state);
    String getState();

    virtual void clear();


 private :
 String gatewayId;
 String password;
 String ssid;
 String nodeId;
 String state;

};

#endif
