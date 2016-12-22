
#ifndef _INCL_IGNITE_ESP8266_WEB_SERVER
#define _INCL_IGNITE_ESP8266_WEB_SERVER


#include "IgniteWebServer.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "IgniteEsp8266WifiNodeConfig.h"


class IgniteEsp8266WebServer : public IgniteWebServer {

  public :
    IgniteEsp8266WebServer(String successState);
    ~IgniteEsp8266WebServer();
    virtual void start();
    virtual void loop();

    bool isServerStarted();

  private :

    static void handleConfigurations();
    static void handleNotFound();
    static void saveConfigAndRestart();
    static void refreshIndex();

    static ESP8266WebServer *server;
    static IgniteEsp8266WifiNodeConfig configUtil;
    static String successState;
    static String setupPage;
    bool isStarted;

};

#endif
