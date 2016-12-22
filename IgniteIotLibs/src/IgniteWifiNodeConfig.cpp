#include "IgniteWifiNodeConfig.h"

#include <ArduinoJson.h>

#define NODE_ID "nodeId"
#define SSID "ssid"
#define PASSWORD "password"
#define STATE "state"
#define GATEWAY_ID "gatewayId"



    void IgniteWifiNodeConfig::setGatewayId(String gwId){
      this->gatewayId = gwId;
      writeConfigs();
    }
    String IgniteWifiNodeConfig::getGatewayId(){
      return this->gatewayId;
    }
    void IgniteWifiNodeConfig::setWifiCredentials(String ssid, String password){
      this->ssid = ssid;
      this->password = password;
      writeConfigs();
    }
    String IgniteWifiNodeConfig::getSSID(){
      return this->ssid;
    }
    String IgniteWifiNodeConfig::getPassword(){
      return this->password;
    }
    void IgniteWifiNodeConfig::setNodeId(String nodeId){
      this->nodeId = nodeId;
      writeConfigs();
    }
    String IgniteWifiNodeConfig::getNodeId(){
      return this->nodeId;
    }
    void IgniteWifiNodeConfig::setState(String state){
      this->state = state;
      writeConfigs();
    }
    String IgniteWifiNodeConfig::getState(){
      return this->state;
    }
    void IgniteWifiNodeConfig::setup(){

      this->nodeId ="";
      this->ssid = "";
      this->password ="";
      this->gatewayId = "";
      this->state = "";

      String jsonString = read();
      StaticJsonBuffer<250> jsonBuffer;


      if(jsonString == NULL || jsonString.equals("")){
        writeConfigs();
      }else{
        JsonObject& root = jsonBuffer.parseObject(jsonString);
        if(root.success()){

        this->nodeId = root[NODE_ID].asString();
        this->ssid = root[SSID].asString();
        this->password =root[PASSWORD].asString();
        this->gatewayId = root[GATEWAY_ID].asString();
        this->state = root[STATE].asString();
      }
      }
    }
    bool IgniteWifiNodeConfig::writeConfigs(){

      String jsonString="";

      StaticJsonBuffer<250> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();

      root[NODE_ID] = getNodeId();
      root[SSID] = getSSID();
      root[PASSWORD] = getPassword();
      root[GATEWAY_ID] = getGatewayId();
      root[STATE] = getState();


      root.printTo(jsonString);

      return write(jsonString);
    }

    void IgniteWifiNodeConfig::clear() {
      write("");
    }
