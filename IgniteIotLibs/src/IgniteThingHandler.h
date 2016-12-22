#ifndef _INCL_IGNITE_THING_HANDLER
#define _INCL_IGNITE_THING_HANDLER

#include "WString.h"
#include "IgniteThingTimer.h"
#include <ArduinoJson.h>

typedef void (*SendMessageFunc)(String);

typedef void (*ResetFunc)();

class IgniteThingHandler {

  public :

    IgniteThingHandler(String nodeType, String uniqueId);
    ~IgniteThingHandler();
    virtual void thingActionReceived(String thingId, String action) = 0;
    virtual void inventorySetup() = 0;
    virtual void unknownMessageReceived(String msg) = 0;
    virtual void loop();
    virtual void setup() = 0;

    static void sendMessage(String message);
    void setSendMessageFunction(SendMessageFunc func);

    static void reset();
    void setResetFunction(ResetFunc func);

    void addThingToInventory(const String thingID, const String thingType,
                             const String connectedPin, bool actuator,
                             const String vendor, const String dataType, IgniteThingTimer *timer);

    String getInventory();
    void thingConfigurationReceived(String thingId, long readFreq);
    void setNodeId(String nodeId);
    String getNodeId();

  private :

    typedef struct ThingHolder {
      String thingId;
      String thingType;
      String connectedPin;
      bool actuator;
      String vendor;
      String dataType;
      IgniteThingTimer *timer;
    } ThingHolder;


    JsonObject& createSensorItemAttributes(JsonObject &thing, ThingHolder &holder);
    int thingSize;
    String nodeType;
    String uniqueId;
    String nodeId;

    ThingHolder *holder;

    static SendMessageFunc sendMessageFunc;
    static ResetFunc resetFunc;
};

#endif
