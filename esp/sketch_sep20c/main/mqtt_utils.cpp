#include "mqtt_utils.h"
#include "message_handler.h"
#include <Arduino.h>
#include "wifi_utils.h"
#include "config.h"

// const char* mqtt_server = "a81e525ac8434ca0b03de8585d2b9e19.s1.eu.hivemq.cloud";
// const int mqtt_port = 8883;
// const char* mqttUsr = "gestasik";
// const char* mqttPass = "Gestasik2343";

WiFiClientSecure espClient;
PubSubClient client(espClient);

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqttUsr, mqttPass)) { // Проверка подключения
      Serial.println("connected");
      client.publish(MQTT_TOPIC_LED_STATE, "connected");

      //Подписка на топики
      client.subscribe(MQTT_TOPIC_LED_CONTROL);
      client.subscribe(MQTT_TOPIC_QTY);
       
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 5s");
      delay(5000);
    }
  }
}

void setup_mqtt(bool insecureMode) {
  if (insecureMode) {
    espClient.setInsecure();
  }
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqtt_callback);
}

void maintain_mqtt() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
}
