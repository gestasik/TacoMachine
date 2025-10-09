#pragma once
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

void setup_mqtt(bool insecureMode);
void maintain_mqtt();
void mqtt_callback(char* topic, byte* payload, unsigned int length);
