#include "IgniteWifiManager.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WiFi.h>

#define CONFIG_REQUEST "configuration"
#define ACTION_REQUEST "action"
#define RESET_REQUEST "reset"
#define DATA_RESPONSE "data"
#define STATUS_REQUEST "inventory-status"


bool IgniteWifiManager::isInventorySend;
IgniteWifiClient *IgniteWifiManager::igniteWifiClient;
IgniteWifiNodeConfig *IgniteWifiManager::configUtil;
String IgniteWifiManager::loopState;

IgniteWifiManager::IgniteWifiManager(IgniteHotspot *hotspot,
                                     IgniteWebServer *webServer, IgniteWifiClient *igniteWifiClient,
                                     IgniteMdns *igniteMDNS, IgniteThingHandler *thingHandler,
                                     IgniteThingTimer *heartbeatTimer,
                                     IgniteWifiNodeConfig *wifiNodeConfig) {
  IgniteWifiManager::loopState = AP_MODE;
  this->hotspot = hotspot;
  this->webServer = webServer;
  IgniteWifiManager::igniteWifiClient = igniteWifiClient;
  this->igniteMDNS = igniteMDNS;
  this->thingHandler = thingHandler;
  IgniteWifiManager::isInventorySend = false;
  this->heartbeatTimer = heartbeatTimer;
  heartbeatTimer->setFreq(5000);
  this->thingHandler->setSendMessageFunction(sendDataToServerIfAvailable);
  IgniteWifiManager::configUtil = wifiNodeConfig;
}

IgniteWifiManager::~IgniteWifiManager() {

}

bool IgniteWifiManager::setup() {

  configUtil->setup();
  webServer->start();

  thingHandler->setNodeId(configUtil->getNodeId());
  String gatewayID = configUtil->getGatewayId();
  loopState = configUtil->getState();
  //if (configUtil.loadState(&loopState)) {
  if ( loopState.equals("")) {
    loopState = AP_MODE;
  }

  thingHandler->inventorySetup();
  thingHandler->setup();
  igniteMDNS->setGatewayId(gatewayID);


  return true;
}

void IgniteWifiManager::loop() {

  Serial.println("\n Loop State");
  Serial.println(loopState);

  if (loopState.equals(AP_MODE)) {
    hotspot->startHotspot();

  } else   if (loopState.equals(SEARCH_GATEWAY_STA)) {
    startDnsAndSearchGateway();
  } else   if (loopState.equals(NODE_READY_STA)) {
    startCommunicationWithGW();
  }
  webServer->loop();
}

void IgniteWifiManager::connectToGateway(String gatewayIp, int gatewayPort) {

  if (!igniteWifiClient->isConnected()) {

    if (igniteWifiClient->connect(gatewayIp, gatewayPort)) {
      Serial.println("IP:");
      Serial.println(gatewayIp);
      Serial.println("PORT:");
      Serial.println(gatewayPort);
      Serial.println("Client connected!");
      loopState = NODE_READY_STA;
    } else {
      Serial.println("Client not connected! Searching...");
      loopState = SEARCH_GATEWAY_STA;
    }
  } else {
    Serial.println("Already connected!");
    loopState = NODE_READY_STA;
  }

  configUtil->setState(loopState);
}

void IgniteWifiManager::startDnsAndSearchGateway() {
  int querySize = igniteMDNS->queryService();
  if (igniteMDNS->findGateway()) {
    String ip = igniteMDNS->getGatewayIp();
    int port = igniteMDNS->getGatewayPort();

    connectToGateway(ip, port);

  }
}


void IgniteWifiManager::sendThingInventory() {
  if (!isInventorySend) {
    sendDataToServerIfAvailable(thingHandler->getInventory());
  }
}


void IgniteWifiManager::sendDataToServerIfAvailable(String msg) {

  msg += "\n";

  if(igniteWifiClient->isConnected()){
  int packetSize = igniteWifiClient->write(msg);


  if (packetSize < 1 ) {
    isInventorySend = false;
    loopState = SEARCH_GATEWAY_STA;
    configUtil->setState(loopState);
    igniteWifiClient->close();
  }
}


}


void IgniteWifiManager::startCommunicationWithGW() {

  if (igniteWifiClient->isConnected()) {

    if (!isInventorySend) {
      Serial.println("Sending Inventory...");
      sendThingInventory();
    }
    String requestJson = igniteWifiClient->readStringUntil('\n');


    if (requestJson.length() > 0) {
      parseRequestMessage(requestJson);
    }
    requestJson = "";

    if (isInventorySend) {
      thingHandler->loop();
      heartbeatTimer->update();
    }

  } else {
    // esp disconnected ? reconnect...
    isInventorySend = false;
    loopState = SEARCH_GATEWAY_STA;
    configUtil->setState(loopState);
  }
}

void IgniteWifiManager::parseRequestMessage(const String &requestJson) {

  char charBuffer[requestJson.length()];
  requestJson.toCharArray(charBuffer, requestJson.length());
  const char *charPtr = requestJson.c_str();
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(charPtr);

  if (!root.success()) {
    Serial.println("parseRequestJson failed");
    return;
  }

  String msgType = root["messageType"];
  Serial.println("MSG_TYPE :");
  Serial.println(msgType);

  if (msgType != NULL) {
    if (msgType.equals(CONFIG_REQUEST)) {
      String thingId = root["thingId"];
      String configMsg = root[CONFIG_REQUEST];
      handleThingConfigurations(thingId, configMsg);

    } else if (msgType.equals(ACTION_REQUEST)) {
      String thingId = root["thingId"];
      String actionMsg = root[ACTION_REQUEST];
      handleThingActions(thingId, actionMsg);
    } else if (msgType.equals(RESET_REQUEST)) {
      // reset esp and start in ap mode.
      resetConfigurations();
    } else if (msgType.equals(STATUS_REQUEST)) {
      //  Serial.println("Status request received.");
      handleInventoryStatus(requestJson);
    } else {
      thingHandler->unknownMessageReceived(requestJson);
    }
  } else {
    thingHandler->unknownMessageReceived(requestJson);
  }
}

void IgniteWifiManager::handleThingConfigurations(String &thingId, String &configMsg) {


  String configuration = configMsg;
  configuration.replace("\\", "");
  //  Serial.println("CONFIG JSON !!!!!!!!!!!!!!!!!!!!!!!!111");
  //  Serial.println(configuration);
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(configuration);
  if (root.success()) {
    float dataFreq = root["dataReadingFrequency"];
    thingHandler->thingConfigurationReceived(thingId, (long) dataFreq);

  } else {
    //Serial.println("Parse Config Json Failed");
  }

}
void IgniteWifiManager::handleThingActions(String &thingId, String &actionMsg) {

  String action = actionMsg;
  action.replace("\\", "");
  thingHandler->thingActionReceived(thingId, action);
}
void IgniteWifiManager::handleInventoryStatus(const String &msg) {
  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);
  String statusMsg = root["status"];
  //  Serial.println("STATUS MSG");
  //Serial.println(statusMsg);
  if (statusMsg.equals("OK")) {
    isInventorySend = true;
    heartbeatTimer->init();
  }
}
