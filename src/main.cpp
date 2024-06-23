#include <Arduino.h>
#include "SimpleOTA.h"
#include "Certify.h"
#include "WiFiManager.h"

Certify cert("http://lucadalessandro.tech/domotica/api/get/fingerprint");
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
    simpleOTA->begin(512, "lucadalessandro.tech", fingerPrint, "TOKEN_ID");
  }
}

void loop() {
  simpleOTA->checkUpdates(300);
}
