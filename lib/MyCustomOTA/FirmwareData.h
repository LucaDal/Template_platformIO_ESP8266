#ifndef FIRMWAREDATA_H
#define FIRMWAREDATA_H

#include <EEPROM.h>   
#include "MyFirmware.h"

class FirmwareData {
private:
  Firmware newFirmware;
  String oldFirmwareVersion = "";
  void loadVersion();
  bool hasNewFirmware;
  int EEPROMSize;

public:
  FirmwareData(int EEPROMSize);
  void saveVersion(String version);
  void setNewFirmware(Firmware new_firmware_version);
  bool hasNewUpdate();
  String getFirmwareMD5Image();
  String getNewFirmwareVersion();
  String getOldFirmwareVersion();
};

#endif 