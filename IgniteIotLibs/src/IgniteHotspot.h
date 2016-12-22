#ifndef _INCL_IGNITE_HOTSPOT
#define _INCL_IGNITE_HOTSPOT

#include "WString.h"

#define ACCESS_POINT_NAME "IotIgnite-Node-"


class IgniteHotspot {

  public :

    virtual void startHotspot() = 0;
    String getHotspotName();

  protected :

    void createHotspotName();

    void setUniqueId(String uniqueId);

    String hotspotName;
    String uniqueId;
    static const String ignitePrefix;

};
#endif
