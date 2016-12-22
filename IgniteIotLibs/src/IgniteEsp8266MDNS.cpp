#include "IgniteEsp8266MDNS.h"



#define SERVICE "iotignite"
#define PROTO "tcp"
#define ESP "esp8266 "

IgniteEsp8266MDNS::IgniteEsp8266MDNS() {
  serviceCount = 0;
  service = SERVICE;
  proto = PROTO;
}

IgniteEsp8266MDNS::~IgniteEsp8266MDNS() {

}

int IgniteEsp8266MDNS::queryService() {

  String name = ESP + getMacAddress();
  IPAddress localIP = WiFi.localIP();

  if (!MDNS.begin(&name[0])) {
    Serial.println("Error setting up MDNS responder!");
    serviceCount = 0;
    return serviceCount;
  }

  Serial.println("Sending mDNS query");
  Serial.println("Searching GW..");
  serviceCount = MDNS.queryService(service, proto); // Send out query for esp tcp services
  Serial.println("mDNS query done");
  return serviceCount;
}

bool IgniteEsp8266MDNS::findGateway() {

  bool retVal = false;
  if (gatewayId == NULL) {
    return retVal;
  }

  for (int i = 0; i < serviceCount; ++i) {
    // Print details for each service found
    Serial.print(i + 1);
    Serial.print(": ");

    Serial.print(MDNS.hostname(i));
    Serial.print(" (");
    Serial.print(MDNS.IP(i));
    Serial.print(":");
    Serial.print(MDNS.port(i));
    Serial.println(")");

    // check gwId with hostname
    Serial.println("GW ID :");
    Serial.println(gatewayId);
    String hostnameStr = MDNS.hostname(i);

    byte byteArrayString[hostnameStr.length()];

    hostnameStr.getBytes(byteArrayString, sizeof(byteArrayString));

    Serial.println("Hostname :");
    Serial.println(hostnameStr);

    String replacedGW = gatewayId;

    replacedGW.replace(":", "");

    replacedGW.replace("@", "");

    Serial.println("Replaced GW");
    Serial.println(replacedGW);

    if (replacedGW.equals(hostnameStr)) {

      Serial.println("GW Found. IP and Port :");
      gatewayIp = MDNS.IP(i).toString();
      Serial.println(gatewayIp);
      gatewayPort = MDNS.port(i);
      Serial.println(gatewayPort);

      retVal = true;
      break;
    }
  }

  return retVal;
}

String IgniteEsp8266MDNS::getMacAddress() {

  byte mac[6];
  WiFi.macAddress(mac);
  String cMac = "";
  for (int i = 0; i < 6; ++i) {
    cMac += String(mac[i], HEX);
    if (i < 5)
      cMac += ":";
  }
  cMac.toUpperCase();
  return cMac;
}
