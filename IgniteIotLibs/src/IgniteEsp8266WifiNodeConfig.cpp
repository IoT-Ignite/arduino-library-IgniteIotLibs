#include "IgniteEsp8266WifiNodeConfig.h"
#include <FS.h>


#define FILE_NODEMCU_CONF "/nodemcu_conf.txt"

String IgniteEsp8266WifiNodeConfig::read(){

  String loadedFile="";
  loadConfigurationFile(&loadedFile);
  return loadedFile;
}


void IgniteEsp8266WifiNodeConfig::setup(){
  startFileSystem();
  IgniteWifiNodeConfig::setup();
}
bool IgniteEsp8266WifiNodeConfig::write(String str){
 saveConfigurationFile(&str);
}

void IgniteEsp8266WifiNodeConfig::startFileSystem() {
  // Initialize file system.
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    ESP.restart();
  }
}

bool IgniteEsp8266WifiNodeConfig::loadConfigurationFile(String *fileContent){
  File configFile = SPIFFS.open(FILE_NODEMCU_CONF, "r");
  if (!configFile) {
    Serial.print("Failed to open [");
    Serial.print(FILE_NODEMCU_CONF);
    Serial.println("] to read.");
    return false;
  }
  String content = configFile.readString();
  configFile.close();
  content.trim();
  *fileContent = content;
  fileContent->trim();
  Serial.print("Loading ");
  Serial.println(" " + *fileContent);
  return true;
}


bool IgniteEsp8266WifiNodeConfig::saveConfigurationFile(String *fileContent) {
  File configFile = SPIFFS.open(FILE_NODEMCU_CONF, "w");
  if (!configFile) {
    Serial.print("Failed to open [");
    Serial.print(FILE_NODEMCU_CONF);
    Serial.println("] for writing");
    return false;
  }
  configFile.println(*fileContent);
  configFile.close();
  Serial.print("Saving ");
  Serial.println(": " + *fileContent);
  return true;
}
