#include "IgniteEsp8266Timer.h"

IgniteEsp8266Timer::IgniteEsp8266Timer(DataFunc dataFunc){
  this->dataFunc = dataFunc;
}
IgniteEsp8266Timer::~IgniteEsp8266Timer(){

}
void IgniteEsp8266Timer::init(){
  stop();
  if(freqInMilliSeconds > 0){
  timerInt = timer.every((unsigned long)freqInMilliSeconds, dataFunc);
  }

}
void IgniteEsp8266Timer::update(){

  if(freqInMilliSeconds > 0){
   timer.update();
 }
}

void IgniteEsp8266Timer::stop(){
  timer.stop(timerInt);
}
