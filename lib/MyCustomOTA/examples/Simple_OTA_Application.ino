#include "SimpleOTA.h"
SimpleOTA * simpleOTA  = new SimpleOTA();

void setup() {
  int EEPROMsize = 512;
  //connect the board to internet first
  //if you want an https connection use:
  //simpleOTA->begin(512,"youraddress","fingerprint","THIS_IS_MY_OWN_API_KEY");
  //otherwise:
  simpleOTA->begin(EEPROMsize,"http://youraddress","THIS_IS_MY_OWN_API_KEY");
}

void loop() {
  //check update every 30 seconds
  simpleOTA->checkUpdates(300); 
  // put your main code here, to run repeatedly:
}
