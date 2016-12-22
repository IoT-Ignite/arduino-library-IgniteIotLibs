#include "IgniteEsp8266WebServer.h"
#include <ArduinoJson.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


ESP8266WebServer *IgniteEsp8266WebServer::server;
IgniteEsp8266WifiNodeConfig  IgniteEsp8266WebServer::configUtil;
String IgniteEsp8266WebServer::successState;
String IgniteEsp8266WebServer::setupPage;


IgniteEsp8266WebServer::IgniteEsp8266WebServer(String successState) {

  IgniteEsp8266WebServer::server = new ESP8266WebServer(80);
  isStarted = false;
  IgniteEsp8266WebServer::successState = successState;
}

IgniteEsp8266WebServer::~IgniteEsp8266WebServer() {

}


void IgniteEsp8266WebServer::start() {

  if (!isStarted) {
    server->on("/", [](){
    refreshIndex();
    });
    server->on("/node-configurations", HTTP_POST, IgniteEsp8266WebServer::handleConfigurations);
    server->on("/saveConfigAndRestart",HTTP_POST, IgniteEsp8266WebServer::saveConfigAndRestart);
    server->onNotFound (IgniteEsp8266WebServer::handleNotFound);
    server->begin();
    Serial.println("HTTP Server started(AP Mode)");
    isStarted = true;
  }

}

void IgniteEsp8266WebServer::loop() {

  if (isStarted) {
    server->handleClient();
  }

}


void IgniteEsp8266WebServer::handleConfigurations() {

  if (server->hasArg("node-configurations")) {
    Serial.println("Configuration received.");
    String configJsonMsg = server->arg("node-configurations");
    configJsonMsg.trim();
    char charBuffer[configJsonMsg.length()];
    configJsonMsg.toCharArray(charBuffer, configJsonMsg.length());
    const char *charPtr = configJsonMsg.c_str();
    StaticJsonBuffer<250> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(charPtr);
    if (!root.success()) {
      Serial.println("parseObject() failed");
      server->send (400, "text/plain", "Json parse failure." );
      return;
    } else {
      String nodeId = root["node-configurations"]["nodeId"];
      String ssid = root["node-configurations"]["ssid"];
      String password = root["node-configurations"]["password"];
      String gatewayId = root["node-configurations"]["gatewayId"];

      configUtil.setWifiCredentials(ssid,password);
      configUtil.setGatewayId(gatewayId);
      configUtil.setNodeId(nodeId);
      if (configUtil.writeConfigs()) {
        server->send (200, "text/plain", "Configuration received successfully. Restarting node..." );
        Serial.println("NodeID:");
        Serial.println(nodeId);
        Serial.println("GwID:");
        Serial.println(gatewayId);
        Serial.println("SSID:");
        Serial.println(ssid);
        Serial.println("Passwd:");
        Serial.println(password);
        configUtil.setState(successState);
        ESP.restart();
      } else {
        server->send (500, "text/plain", "Configuration failure. Please try again." );
      }
    }
  } else {
    int count = server->args();
    Serial.println("Count ");
    Serial.println(count);
    server->send (400, "text/plain", "Bad request. Please try again." );

  }

}

void IgniteEsp8266WebServer::handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server->uri();
  message += "\nMethod: ";
  message += ( server->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server->args();
  message += "\n";

  for ( uint8_t i = 0; i < server->args(); i++ ) {
    message += " " + server->argName ( i ) + ": " + server->arg ( i ) + "\n";
  }
  server->send ( 404, "text/plain", message );

}

bool IgniteEsp8266WebServer::isServerStarted() {
  return this->isStarted;
}


void IgniteEsp8266WebServer::saveConfigAndRestart(){

  String nodeId = server->arg("nodeId");
  String gatewayId = server->arg("gwId");
  String ssid = server->arg("ssid");
  String password = server->arg("password");
  String state = server->arg("state");
  if(nodeId.length() > 0 && gatewayId.length() > 0 && ssid.length() > 0){

  configUtil.setWifiCredentials(ssid,password);
  configUtil.setGatewayId(gatewayId);
  configUtil.setNodeId(nodeId);
    if (configUtil.writeConfigs()) {
      server->send (200, "text/plain", "Configuration received successfully. Restarting node..." );
      Serial.println("NodeID:");
      Serial.println(nodeId);
      Serial.println("GwID:");
      Serial.println(gatewayId);
      Serial.println("SSID:");
      Serial.println(ssid);
      Serial.println("Passwd:");
      Serial.println(password);
      configUtil.setState(successState);
      ESP.restart();
    } else {
      server->send (500, "text/plain", "Configuration failure. Please try again." );
    }
  } else {
  server->send (500, "text/plain", "Configuration failure. Check your parameters and try again." );
  }
}

void IgniteEsp8266WebServer::refreshIndex() {


      configUtil.setup();
      setupPage = "<!DOCTYPE html>\n"
      "<html>\n"
      "<script>\n"
    "function confirmDialog() {\n"
    "return confirm(\"Do you want to apply configs ? \");}\n"
    "</script>\n"
      "<style>\n"
      "form {\n"
      "    border: 3px solid #f1f1f1;\n"
      "    width: 50%;\n"
      "    margin: 0 auto;\n"
      "}\n"
      "\n"
      "input[type=text], input[type=password] {\n"
      "    width: 100%;\n"
      "    padding: 12px 20px;\n"
      "    margin: 8px 0;\n"
      "    display: inline-block;\n"
      "    border: 1px solid #ccc;\n"
      "    box-sizing: border-box;\n"
      "}\n"
      "\n"
      "button {\n"
      "    background-color: #4CAF50;\n"
      "    color: white;\n"
      "    padding: 14px 20px;\n"
      "    margin: 8px 0;\n"
      "    border: none;\n"
      "    cursor: pointer;\n"
      "    width: 100%;\n"
      "}\n"
      "\n"
      ".container {\n"
      "    padding: 16px;\n"
      "}\n"
      "\n"
      ".center {\n"
      "    text-align:center;\n"
      "}\n"
      "</style>\n"
      "<body>\n"
      "<h2 class=\"center\">Node Configuration</h2>\n"
      "<form action=\"saveConfigAndRestart\" method=\"post\" onsubmit=\"return confirmDialog();\">\n"
      "<div class=\"container\">\n"
      "  <label for=\"nodeId\"><b>Node ID:</b></label>\n"
      "  <input type=\"text\" placeholder=\"Enter Node ID\" name=\"nodeId\" id=\"nodeId\" value=\"";

      setupPage+=configUtil.getNodeId();
      setupPage+= "\">\n"
      "\n"
      "  <label for=\"gwId\"><b>Gateway ID:</b></label>\n"
      "  <input type=\"text\" placeholder=\"Enter Gateway ID\" name=\"gwId\" id=\"gwId\" value=\"";
      setupPage+=configUtil.getGatewayId();
      setupPage+="\">\n"
      "\n"
      "  <label for=\"ssid\"><b>SSID:</b></label>\n"
      "  <input type=\"text\" placeholder=\"SSID\" name=\"ssid\" id=\"ssid\" value=\"";
      setupPage+=configUtil.getSSID();
      setupPage+="\">\n"
      //"  <input type=\"text\" placeholder=\"SSID\" name=\"ssid\" id=\"ssid\" value=\"asdsasdsad\">\n"
      "  <label for=\"password\"><b>Password:</b></label>\n"
      "  <input type=\"password\" placeholder=\"Enter Password\" name=\"password\"  id=\"password\" value=\"";
      setupPage+=configUtil.getPassword();
      setupPage += "\">\n"
      "  <label for=\"state\"><b>State:</b></label>\n"
      "  <input type=\"text\" placeholder=\"\" name=\"state\" id=\"state\" disabled=\"disabled\" value=\"";

      String state = configUtil.getState();
      if(state.equals("AP")){
        setupPage+="Started in Access Point Mode";
      }else if(state.equals("SEARCH_GATEWAY_STA")){
        setupPage+="Searching Gateway...";
      }else if(state.equals("NODE_READY")){
        setupPage+="Node is Online";
      }
      setupPage+="\">\n"
      "  <button type=\"submit\">Save Configurations And Restart</button>\n"
      "<h3 class=\"center\"><a href=\"https://www.iot-ignite.com/#/\" target=\"_blank\">www.iot-ignite.com</a></h3>\n"
      "<h3 class=\"center\">Powered by ARDIC</h3>\n"
      "</div>\n"
      "</form>\n"
      "</body>\n"
      "</html>";
      server->send(200, "text/html", setupPage);
}
