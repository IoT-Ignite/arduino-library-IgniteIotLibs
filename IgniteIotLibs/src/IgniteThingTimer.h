#ifndef _INCL_IGNITE_THING_TIMER
#define _INCL_IGNITE_THING_TIMER


class IgniteThingTimer{

  public :

  IgniteThingTimer(){
    freqInMilliSeconds=-1;
  }
  virtual void init() = 0;
  virtual void update() = 0;
  virtual void stop() = 0;

  void setFreq(long freq){
    this->freqInMilliSeconds = freq;
  }
  long getFreq(){
    return freqInMilliSeconds;
  }

  protected :

  long freqInMilliSeconds;

  private :


};


#endif
