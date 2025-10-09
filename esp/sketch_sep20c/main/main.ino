#include <Arduino.h>
#include "wifi_utils.h"
#include "mqtt_utils.h"

const int BUILTIN_LED = 2;
bool insecureMode = true;

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  setup_wifi();
  setup_mqtt(insecureMode);
}

void loop() {
  maintain_wifi();
  maintain_mqtt();
}
