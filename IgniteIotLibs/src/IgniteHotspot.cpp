#include "IgniteHotspot.h"



const String IgniteHotspot::ignitePrefix =  ACCESS_POINT_NAME;

void IgniteHotspot::createHotspotName() {

  hotspotName = ACCESS_POINT_NAME + uniqueId;
}


void IgniteHotspot::setUniqueId(String uniqueId) {
  this->uniqueId = uniqueId;
  createHotspotName();
}

String IgniteHotspot::getHotspotName() {
  return hotspotName;
}
