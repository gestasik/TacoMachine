#include "message_handler.h"

void handle_incoming_message(const String& topic, const String& payload) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(payload);

  if (topic == "/miptfab/esp32led/ledControl/") {
    if (payload == "on") {
      digitalWrite(2, HIGH);
      Serial.println("LED ON");
    } else if (payload == "off") {
      digitalWrite(2, LOW);
      Serial.println("LED OFF");
    }
  } 
  else if (topic == "esp32/qty") {
    Serial.println("Received form from site!");
    handle_form_message(payload);
  }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  String topicStr = String(topic);
  String payloadStr;

  for (unsigned int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }

  handle_incoming_message(topicStr, payloadStr);
}

int handle_pin(const long pin) {
  return true;
}

// =============================
// Парсим pin из JSON-строки
// =============================
int parse_pin(String input) {
  int pin = 0;
  int pinPos = input.indexOf("\"pin\"");
  if (pinPos != -1) {
    int colonPos = input.indexOf(":", pinPos);
    int commaPos = input.indexOf(",", colonPos);
    if (commaPos == -1) commaPos = input.indexOf("}", colonPos);
    String pinStr = input.substring(colonPos + 1, commaPos);
    pinStr.trim();
    pin = pinStr.toInt();
  }
  return pin;
}

// =============================
// Парсим qty1..qty8 из JSON
// =============================
void parse_form(String input, int qty[8]) {
  for (int i = 1; i <= 8; i++) {
    String key = "\"qty" + String(i) + "\"";
    int pos = input.indexOf(key);
    if (pos != -1) {
      int colonPos = input.indexOf(":", pos);
      int commaPos = input.indexOf(",", colonPos);
      if (commaPos == -1) commaPos = input.indexOf("}", colonPos);
      String valStr = input.substring(colonPos + 1, commaPos);
      valStr.trim();
      qty[i - 1] = valStr.toInt();
    }
  }
}

// =============================
// Обработка формы
// =============================
void handle_form_message(const String& payload) {
  int pin = parse_pin(payload);
  int qty[8] = {0};

  parse_form(payload, qty);  // заполняем массив

  Serial.print("PIN = ");
  Serial.println(pin);

  for (int i = 0; i < 8; i++) {
    Serial.print("qty[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(qty[i]);
  }
}
