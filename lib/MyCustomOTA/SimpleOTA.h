#ifndef SIMLPEOTA_H
#define SIMLPEOTA_H

//to activate serial.Print()  
#define DEBUG 

#include <Arduino.h>
#include "FirmwareData.h"
#include "Network.h"

class SimpleOTA {
private:
  FirmwareData *version;
  Network *network;
  unsigned long t1;
  const char * API_KEY;

  void initVersion(int EEPROMSize);
  void initNetwork(const char * base_url, const char * fingerPrint);
  bool serverFirmwareCheck();
  bool startDownload();
  void init(int EEPROMSize, const char * base_url, const char * API_KEY);
  //void updateFirmware();

public:
  SimpleOTA();
  void begin(int EEPROMSize,const char * base_url, const char * fingerPrint, const char * API_KEY);
  void begin(int EEPROMSize,const char * base_url, const char * API_KEY);
  bool checkUpdates(unsigned long seconds);
};

#endif