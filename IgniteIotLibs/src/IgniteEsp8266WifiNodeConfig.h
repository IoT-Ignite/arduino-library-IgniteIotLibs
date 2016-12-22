#ifndef _INCL_IGNITE_ESP_8266_WIFI_NODE_CONFIG
#define _INCL_IGNITE_ESP_8266_WIFI_NODE_CONFIG

#include "IgniteWifiNodeConfig.h"

class IgniteEsp8266WifiNodeConfig : public IgniteWifiNodeConfig{

  public :

  virtual String read();
  virtual bool write(String str);
  virtual void setup();
  bool loadConfigurationFile(String *fileContent);
  bool saveConfigurationFile(String *fileContent);
  void startFileSystem();

  private :

};

#endif
