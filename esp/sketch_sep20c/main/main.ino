#include <Arduino.h>
#include "wifi_utils.h"
#include "mqtt_utils.h"
#include "message_handler.h"
#include "config.h"


void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  setup_wifi();
  setup_mqtt(insecureMode);

  randomSeed(analogRead(0));

  generateNewPin();
}

void loop() {
  maintain_wifi();
  maintain_mqtt();
  checkPinTimeout();
}
