#include "IgniteThingHandler.h"
#include <ArduinoJson.h>
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

SendMessageFunc IgniteThingHandler::sendMessageFunc;
ResetFunc IgniteThingHandler::resetFunc;

IgniteThingHandler::IgniteThingHandler(String nodeType, String uniqueId) {
  thingSize = 0;
  this->nodeType = nodeType;
  this->uniqueId = uniqueId;
}

IgniteThingHandler::~IgniteThingHandler() {
  if (holder != NULL) {
    delete holder;
  }
}

void IgniteThingHandler::addThingToInventory(const String thingID, const String thingType,
    const String connectedPin, bool actuator, const String vendor,
    const String dataType, IgniteThingTimer *timer) {
  ThingHolder newHolder;
  newHolder.thingId = thingID;
  newHolder.thingType = thingType;
  newHolder.connectedPin = connectedPin;
  newHolder.actuator = actuator;
  newHolder.vendor = vendor;
  newHolder.dataType = dataType;
  newHolder.timer = timer;

  const int arraySize = thingSize + 1;
    Serial.printf("\n New ThingSize : %d", thingSize);
  ThingHolder *newHolderArr = new ThingHolder[arraySize]();
  for (int i = 0; i < thingSize; ++i) {
    newHolderArr[i] = holder[i];
  }

  newHolderArr[thingSize++] = newHolder;

  if (holder != NULL) {
    delete holder;
  }

  holder = newHolderArr;
}


String IgniteThingHandler::getInventory() {

  String json = "";
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonObject& node_inventory = root.createNestedObject("node-inventory");
  JsonArray& things = node_inventory.createNestedArray("things");

  node_inventory["nodeId"] = getNodeId();

  node_inventory["uniqueId"] = uniqueId;

  if (nodeType != NULL) {
    node_inventory["nodeType"]  = nodeType;
  }

  Serial.printf("\n ThingSize : %d", thingSize);
  for (int i = 0; i < thingSize; ++i) {
    createSensorItemAttributes(things.createNestedObject(), holder[i]);
  }


  root.printTo(json);

  return json;
}


JsonObject& IgniteThingHandler::createSensorItemAttributes(JsonObject &thing, ThingHolder &holder) {
  thing["thingId"] = holder.thingId;
  thing["thingType"] = holder.thingType;
  thing["connectedPin"] = holder.connectedPin;
  thing["actuator"] = holder.actuator ;
  thing["vendor"] = holder.vendor;
  thing["dataType"] = holder.dataType;

  return thing;
}


void IgniteThingHandler::thingConfigurationReceived(String thingId, long readFreq) {

  for (int i = 0; i < thingSize; i++) {

    if (holder[i].thingId.equals(thingId)) {

      holder[i].timer->setFreq(readFreq);
      holder[i].timer->init();
      break;
    }
  }
}


void IgniteThingHandler::loop() {
  for (int i = 0; i < thingSize; i++) {
    if (holder[i].timer->getFreq() > 0) {
      holder[i].timer->update();
    }
  }
}

void IgniteThingHandler::sendMessage(String message) {
  sendMessageFunc(message);
}

void IgniteThingHandler::setSendMessageFunction(SendMessageFunc func) {
  IgniteThingHandler::sendMessageFunc = func;
}
void IgniteThingHandler::reset() {
  resetFunc();
}

void IgniteThingHandler::setResetFunction(ResetFunc func) {
  IgniteThingHandler::resetFunc = func;
}

void IgniteThingHandler::setNodeId(String nodeId){
  this->nodeId = nodeId;
}
String IgniteThingHandler::getNodeId(){
  return this->nodeId;
}
