#ifndef _INCL_IGNITE_ESP_8266_WIFI_CLIENT
#define _INCL_IGNITE_ESP_8266_WIFI_CLIENT

#include "IgniteWifiClient.h"
#include <ESP8266WiFi.h>


class IgniteEsp8266WifiClient : public IgniteWifiClient {

  public :

    IgniteEsp8266WifiClient();
    ~IgniteEsp8266WifiClient();
    virtual bool connect(String ipAddress, int port);
    virtual bool isConnected();
    virtual void close();
    virtual String readStringUntil(char c);
    virtual int write(String message);


  private :
    WiFiClient wifiClient;
};
#endif
