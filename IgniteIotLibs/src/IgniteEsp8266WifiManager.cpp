#include "IgniteEsp8266WifiManager.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#define SERIAL_BAUDRATE 115200

IgniteEsp8266WifiManager::IgniteEsp8266WifiManager(IgniteThingHandler *thingHandler): IgniteWifiManager(&espHotspot, new IgniteEsp8266WebServer(SEARCH_GATEWAY_STA), &espWifiClient, &igniteEsp8266MDNS,
      thingHandler, new IgniteEsp8266Timer(sendHeartbeat), &wifiNodeConfig) {

  thingHandler->setResetFunction(reset);
}

IgniteEsp8266WifiManager::~IgniteEsp8266WifiManager() {

}

bool IgniteEsp8266WifiManager::setup() {


    Serial.begin(SERIAL_BAUDRATE);
    delay(100);
    Serial.println("\r\n");
    Serial.print("Chip ID: 0x");
    Serial.println(ESP.getChipId(), HEX);
  if (!IgniteWifiManager::setup()) {
    ESP.restart();
    return false;

  }

  return true;
}

void IgniteEsp8266WifiManager::loop() {

  if (loopState.equals(SEARCH_GATEWAY_STA)) {
    if (WiFi.status() == WL_CONNECTED) {
      IgniteWifiManager::loop();
    } else {
      connectToWifi();
    }
  } else {
    IgniteWifiManager::loop();
  }
}


void IgniteEsp8266WifiManager::connectToWifi() {

  String stationSSID = configUtil->getSSID();
  String stationPSK = configUtil->getPassword();


  Serial.println("SSID name is " );
  Serial.println(WiFi.SSID());
  String msg = "Trying to connect ->  SSID : " + WiFi.SSID()  + "  password " + stationPSK + "  " ;
  Serial.println(msg);
  unsigned long startTime = millis();
  if (WiFi.getMode() == WIFI_STA) {
    Serial.println("Wifi mode is sta trying to auto connect....");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WL_CONNECTED");
      Serial.println(WiFi.localIP());
      Serial.println(WiFi.SSID());
      WiFi.reconnect();
    } else {
      Serial.println("WL_NOT CONNECTED");
    }
  } else {
    Serial.println("Setting wifi mode to trying to connect....");
    WiFi.mode(WIFI_STA);
    delay(500);
    WiFi.begin(stationSSID.c_str(), stationPSK.c_str());
  }

  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    Serial.write('.');
    delay(500);
  }

}

void IgniteEsp8266WifiManager::resetConfigurations() {
  reset();
}

void IgniteEsp8266WifiManager::sendHeartbeat() {
  // generate heartbeat json and send to gw.
  String jsonMessage = "";
  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  root["messageType"] = "heartbeat";
  root["heartbeat"] = "_--^--_--^--_--^--_--^--_";

  root.printTo(jsonMessage);

  Serial.println("Heartbeat :");
  Serial.println(jsonMessage);
  jsonMessage += "\n";

  sendDataToServerIfAvailable(jsonMessage);
}

void IgniteEsp8266WifiManager::reset() {
  configUtil->clear();
  WiFi.mode(WIFI_AP);
  ESP.restart();

}
