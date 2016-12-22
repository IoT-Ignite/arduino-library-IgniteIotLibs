#ifndef _INCL_IGNITE_ESP8266_WIFI_MANAGER
#define _INCL_IGNITE_ESP8266_WIFI_MANAGER

#include "IgniteWifiManager.h"
#include "IgniteEsp8266WebServer.h"
#include "IgniteEsp8266Hotspot.h"
#include "IgniteEsp8266WifiClient.h"
#include "IgniteEsp8266MDNS.h"
#include "IgniteEsp8266Timer.h"
#include "IgniteEsp8266WifiNodeConfig.h"

class IgniteEsp8266WifiManager : public IgniteWifiManager {

  public :
    IgniteEsp8266WifiManager(IgniteThingHandler *thingHandler);
    ~IgniteEsp8266WifiManager();
    virtual bool setup();
    virtual void loop();
    virtual void resetConfigurations();
    static void sendHeartbeat();
    static void reset();

  private :

    void connectToWifi();

    IgniteEsp8266WebServer *espWebServer;
    IgniteEsp8266Hotspot espHotspot;
    IgniteEsp8266WifiClient espWifiClient;
    IgniteEsp8266MDNS igniteEsp8266MDNS;
    IgniteEsp8266Timer *heartbeatTimer;
    IgniteEsp8266WifiNodeConfig wifiNodeConfig;
};

#endif
