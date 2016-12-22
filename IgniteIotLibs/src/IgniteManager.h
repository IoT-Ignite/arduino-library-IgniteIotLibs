#ifndef _INCL_IGNITE_MANAGER
#define _INCL_IGNITE_MANAGER

class IgniteManager {

  public :

    virtual bool setup() = 0;
    virtual void loop() = 0;

};

#endif
