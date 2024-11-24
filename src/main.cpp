#include <Arduino.h>
#include "SimpleOTA.h"
#include "WiFiManager.h"
#include "secret_data.h"

SimpleOTA *simpleOTA = new SimpleOTA();

void setup() {
  Serial.begin(115200);

  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(120);
  wifiManager.autoConnect("Project Name");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("starting OTA");
    simpleOTA->begin(512, SERVER_ADDRESS, TOKEN_ID, true);
  }
}

void loop() {
  simpleOTA->checkUpdates(300);
}
