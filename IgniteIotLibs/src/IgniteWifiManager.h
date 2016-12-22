#ifndef _INCL_IGNITE_WIFI_MANAGER
#define _INCL_IGNITE_WIFI_MANAGER


#define AP_MODE  "AP"
#define SEARCH_GATEWAY_STA "SEARCH_GATEWAY_STA"
#define NODE_READY_STA  "NODE_READY"


#include "IgniteHotspot.h"
#include "IgniteWebServer.h"
#include "IgniteWifiNodeConfig.h"
#include "IgniteManager.h"
#include "IgniteWifiClient.h"
#include "IgniteMdns.h"
#include "IgniteThingHandler.h"
#include "IgniteThingTimer.h"

class IgniteWifiManager : public IgniteManager {


  public :
    IgniteWifiManager(IgniteHotspot *hotspot, IgniteWebServer *webServer,
                      IgniteWifiClient *igniteWifiClient, IgniteMdns *igniteMDNS,
                      IgniteThingHandler *thingHandler, IgniteThingTimer *heartbeatTimer,
                    IgniteWifiNodeConfig *wifiNodeConfig);
    ~IgniteWifiManager();

    virtual bool setup();
    virtual void loop();
    virtual void resetConfigurations() = 0;

  private :

    void connectToGateway(String gatewayIp, int gatewayPort);
    void startDnsAndSearchGateway();
    void sendThingInventory();
    void startCommunicationWithGW();
    void parseRequestMessage(const String &requestJson);
    void handleThingConfigurations(String &thingId, String &configMsg);
    void handleThingActions(String &thingId, String &actionMsg);
    void handleInventoryStatus(const String &msg);
    static bool isInventorySend;


  protected:

    static void sendDataToServerIfAvailable(String msg);
    IgniteHotspot *hotspot;
    IgniteWebServer *webServer;
    static IgniteWifiClient *igniteWifiClient;
    IgniteMdns *igniteMDNS;
    IgniteThingHandler *thingHandler;
    static IgniteWifiNodeConfig *configUtil;
    static String loopState;
    IgniteThingTimer *heartbeatTimer;
};



#endif
