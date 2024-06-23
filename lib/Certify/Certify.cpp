#include "Certify.h"

Certify::Certify(const char *host){
  this->host = host;
}

const char * Certify::getFingerprintFromServer(){
  HTTPClient http;
  WiFiClient wifiClient;
  const char* payload;
  if (http.begin(wifiClient,this->host)){
    if (http.GET() != HTTP_CODE_OK) {
      return NULL;
    }
    payload = http.getString().c_str();
    fingerPrint = (char *)malloc(sizeof(char)*60);
    for (int i = 0; i < 59; i++){
      fingerPrint[i] = payload[i];
    }
    fingerPrint[59]= '\0';
    return (const char *)fingerPrint;
  }
  return NULL;
}

const char * Certify::getFingerprint(){
  free(fingerPrint); //in case the fingerprint renew
  return getFingerprintFromServer();
}





