#include "IgniteEsp8266Hotspot.h"

#include <ESP8266WiFi.h>



IgniteEsp8266Hotspot::IgniteEsp8266Hotspot() {

  isStarted = false;

  setUniqueId(String(ESP.getChipId(), HEX));
}



IgniteEsp8266Hotspot::~IgniteEsp8266Hotspot() {
}

void IgniteEsp8266Hotspot::startHotspot() {

  if (!isStarted) {
    WiFi.mode(WIFI_AP);
    WiFi.hostname(hotspotName);
    WiFi.softAP(hotspotName.c_str());
    isStarted = true;
  }
}


bool IgniteEsp8266Hotspot::isHotspotStarted() {
  return this->isStarted;
}
