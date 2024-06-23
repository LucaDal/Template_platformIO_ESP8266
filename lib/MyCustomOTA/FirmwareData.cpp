#include "FirmwareData.h"

FirmwareData::FirmwareData(int EEPROMSize) {
  this->EEPROMSize = EEPROMSize;
  EEPROM.begin(EEPROMSize);
  this->loadVersion();
  hasNewFirmware = false;
}

/*
Version is stored in in the last two bit
*/
void FirmwareData::loadVersion() {
  
  uint8_t firstValue = EEPROM.read(EEPROMSize-2);
  uint8_t SecondValue = EEPROM.read(EEPROMSize-1);
  this-> newFirmware.version = String(firstValue) + '.' + String(SecondValue); ;
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void FirmwareData::saveVersion(String version) {
  uint8_t firstValue =  getValue(version,'.',0).toInt();
  uint8_t secondValue = getValue(version,'.',1).toInt();
  #ifdef DEBUG
    Serial.printf("firstValue = %i, secondValue = %i\n",firstValue,secondValue);
  #endif
  EEPROM.write(EEPROMSize-2, firstValue);
  EEPROM.write(EEPROMSize-1, secondValue);
  EEPROM.commit();
}

void FirmwareData::setNewFirmware(Firmware firmware) {

  if (newFirmware.version == firmware.version) {
      return;
  }
  #ifdef DEBUG
    Serial.printf("newFirmware.version = {%s} == {%s} : ",newFirmware.version.c_str(), firmware.version.c_str());
    Serial.println(newFirmware.version == firmware.version);
  #endif
  oldFirmwareVersion = newFirmware.version;
  newFirmware = firmware;
  if (newFirmware.version != "-1") {
    #ifdef DEBUG
      Serial.printf("version: %s\n", newFirmware.version.c_str());
      Serial.printf("MD5Checksum: %s\n", newFirmware.md5_checksum.c_str());
    #endif
    hasNewFirmware = true;
  }
}

bool FirmwareData::hasNewUpdate() {
  return hasNewFirmware;
}

String FirmwareData::getFirmwareMD5Image() {
  return newFirmware.md5_checksum;
}

String FirmwareData::getNewFirmwareVersion() {
  return newFirmware.version;
}

String FirmwareData::getOldFirmwareVersion() {
  return oldFirmwareVersion;
}
