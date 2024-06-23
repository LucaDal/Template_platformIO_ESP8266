#include "Network.h"
//#define DEBUG
Network::Network(const char * base_url, const char * fingerPrint){
  this->BASE_URL = base_url;
  this->fingerPrint = fingerPrint;
  if(fingerPrint != NULL){
    client->setFingerprint(fingerPrint);
  }
}

void Network::WiFiBegin() {
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
}

bool Network::isConnected(){
  if( WiFi.status() == WL_CONNECTED ){
    return true;
  }
  return false;
}

bool Network::startConnectionWith(String server_api_address, String apy_key){
  bool http_connected = false;
  if (fingerPrint != NULL){
    String targetURL = server_api_address + apy_key;
    #ifdef DEBUG
      String host = this->BASE_URL;
      Serial.println("*OTA: Connecting to: "+ host + targetURL);
    #endif
    //tls connection on port 443
    http_connected = https.begin(*client, this->BASE_URL, 443 , targetURL.c_str(), true);
  }else{
    WiFiClient client;
    String httpRequestData = this->BASE_URL + server_api_address + apy_key;
    #ifdef DEBUG
      String host = this->BASE_URL;
      Serial.println("*OTA: Connecting to: "+ host + httpRequestData);
    #endif
    http_connected = https.begin(client, httpRequestData);
  }
  return http_connected;
}


Firmware Network::checkVersion(String apy_key) {

  Firmware firmware;
  firmware.version = "-1";
  #ifdef DEBUG
    Serial.println("*OTA: checking version");
  #endif
  if (isConnected()) {    
    String server_api_address = "/ota/api/get/version/";
    bool http_connected = startConnectionWith(server_api_address, apy_key);

    if(http_connected){
        #ifdef DEBUG
          Serial.println("connesso");
        #endif
      int httpCode = https.GET();
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        #ifdef DEBUG
          Serial.println(payload);
        #endif
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          #ifdef DEBUG
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
          #endif
          return firmware;
        }
        firmware.version = doc["version"].as<String>();
        firmware.md5_checksum = doc["md5Checksum"].as<String>();
      } else {
        #ifdef DEBUG
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        #endif
      }
      https.end();
    }else{
        #ifdef DEBUG
          Serial.println("[HTTPS] Unable to connect\n");
        #endif
    }
  }
  return firmware;
}

bool Network::fileDownload(String apy_key, String md5Checksum, String currentVersion){
  String httpRequestData = "/ota/api/post/update/";
  if (isConnected()) {
    MyUpdater update = MyUpdater(md5Checksum);
    bool http_connected = startConnectionWith(httpRequestData, apy_key);
    if(http_connected){
      bool return_value = update.startUpdate(this->https, currentVersion);
      https.end();
      return return_value;
    }
  }
  return false;
}