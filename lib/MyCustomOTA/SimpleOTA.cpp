#include "SimpleOTA.h"

static SimpleOTA *instance = NULL;

SimpleOTA::SimpleOTA() {
  #ifdef DEBUG
    Serial.println("starting the OTA client");
  #endif
  t1 = 0;
  instance = this;
}


void SimpleOTA::init(int EEPROMSize, const char * base_url, const char * API_KEY){
  this->initVersion(EEPROMSize);
  this->API_KEY = API_KEY;
  checkUpdates(0);
}

/**
 * @brief calling this function the update is checked automaticaly
 * Firmware data will be saved to the last EEPROM address
 */
void SimpleOTA::begin(int EEPROMSize, const char * base_url, const char * API_KEY) {
  this->initNetwork(base_url, NULL);
  init(EEPROMSize,base_url,API_KEY);
}

/**
 * @brief calling this function will automaticaly check the update.
 * Firmware data will be saved to the last EEPROM address
 * Fingerprint is needed to perform HTTPS request
 */
void SimpleOTA::begin(int EEPROMSize,const char * base_url, const char * fingerPrint, const char * API_KEY) {
  this->initNetwork(base_url, fingerPrint);
  init(EEPROMSize,base_url,API_KEY);
}

/**
 * @brief return false if something goes wrong
 * called from the main thread
 */
bool SimpleOTA::checkUpdates(unsigned long seconds) {
    if (millis() - t1 >= seconds * 1000) {
      t1 = millis();
      if(network->isConnected()) {
        return this->serverFirmwareCheck();
      } 
    }
    return true;
}

void SimpleOTA::initVersion(int EEPROMSize) {
  #ifdef DEBUG
    Serial.println("*OTA: initVersion");
  #endif
  version = new FirmwareData(EEPROMSize);
  #ifdef DEBUG
    Serial.printf("*OTA: Current Version: %s\n",version->getNewFirmwareVersion().c_str());
  #endif
}

void SimpleOTA::initNetwork(const char * base_url, const char * fingerPrint) {
  #ifdef DEBUG
    Serial.println("*OTA: initNetwork");
  #endif
  network = new Network(base_url, fingerPrint); 
  network->WiFiBegin();
}

bool SimpleOTA::startDownload() {
  if(network->fileDownload(API_KEY,version->getFirmwareMD5Image(),version->getOldFirmwareVersion())){
    version->saveVersion(version->getNewFirmwareVersion());//save only if update goes fine
    #ifdef DEBUG
      Serial.println("*OTA: Restarting the board");
    #endif
    delay(1000); // Wait a second and restart
    ESP.restart();
  }
  return false;
}
/**
 * return false if failed, true if no update found.
 */
bool SimpleOTA::serverFirmwareCheck() {
  version->setNewFirmware(network->checkVersion(API_KEY));
  if (version->getNewFirmwareVersion() == "-1") {
    #ifdef DEBUG
      Serial.println("*OTA: Server Not Responding");
    #endif
    return false;
  } else {
    if (version->hasNewUpdate()) {
      #ifdef DEBUG
        Serial.println("*OTA: New Build Available!");
        Serial.println("*OTA: Starting the donwload!");
      #endif
      return startDownload();
    }else{
      #ifdef DEBUG
        Serial.println("*OTA: Current version up to date");
      #endif
    }
    return true;
  }
}


