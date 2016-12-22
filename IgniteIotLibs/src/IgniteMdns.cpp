#include "IgniteMdns.h"

String IgniteMdns::getGatewayIp() {
  return gatewayIp;
}

int IgniteMdns::getGatewayPort() {
  return gatewayPort;
}

void  IgniteMdns::setGatewayId(String gatewayId) {
  this->gatewayId = gatewayId;
}
