#include <Arduino.h>
#include "SimpleOTA.h"
#include "Certify.h"
#include "WiFiManager.h"
#include "secret_data.h"

Certify cert("http://SERVER_ADDRESS/domotica/api/get/fingerprint");
SimpleOTA *simpleOTA = new SimpleOTA();
const char* fingerPrint;

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Project Name");

  if (WiFi.status() == WL_CONNECTED) {
    fingerPrint = cert.getFingerprint();
    Serial.println("starting OTA");
    Serial.println(fingerPrint);
    simpleOTA->begin(512, SERVER_ADDRESS, fingerPrint, TOKEN_ID);
  }
}

void loop() {
  simpleOTA->checkUpdates(300);
}
