#include "IgniteEsp8266WifiClient.h"


IgniteEsp8266WifiClient::IgniteEsp8266WifiClient() {

}

IgniteEsp8266WifiClient::~IgniteEsp8266WifiClient() {

}

bool IgniteEsp8266WifiClient::isConnected() {
  return wifiClient.connected();
}


bool IgniteEsp8266WifiClient::connect(String ipAddress, int port) {

  IPAddress ipAddressObj;
  if (ipAddressObj.fromString(ipAddress)) {
    return wifiClient.connect(ipAddressObj, port);
  }

  return false;
}

void IgniteEsp8266WifiClient::close() {
  wifiClient.stop();
}

String IgniteEsp8266WifiClient::readStringUntil(char c) {
  return wifiClient.readStringUntil(c);
}


int IgniteEsp8266WifiClient::write(String message) {

  if (message != NULL) {
    return  wifiClient.write((uint8_t*) &message[0], message.length());
  }
  return -1;
}
