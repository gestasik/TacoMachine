#include "wifi_utils.h"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_system.h>
#include "config.h"
// Wi-Fi credentials
// const char* ssid = "D3F";
// const char* password = "FabMIPT2022";

void setup_wifi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if (retries > 60) {
      Serial.println("\nFailed to connect to Wi-Fi. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void maintain_wifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi lost, reconnecting...");
    setup_wifi();
  }
}
