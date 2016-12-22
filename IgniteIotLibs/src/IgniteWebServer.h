#ifndef _INCL_IGNITE_WEB_SERVER
#define _INCL_IGNITE_WEB_SERVER



class IgniteWebServer {

  public :
    virtual void start() = 0;
    virtual void loop() = 0;

};
#endif
