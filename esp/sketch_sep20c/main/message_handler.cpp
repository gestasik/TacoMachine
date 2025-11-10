#include "message_handler.h"
#include "electro_utils.h"
#include "config.h"



// const unsigned long PIN_LIFETIME = 60000;    // 60 секунд
// const unsigned long WARNING_TIME = 10000;    // За 10 секунд до конца начинаем мигание
// const unsigned long BLINK_INTERVAL = 500;    // Интервал мигания 0.5 сек

unsigned long pinStartTime = 0;              // Время, когда PIN был создан
unsigned long lastBlinkTime = 0;             // Время последнего мигания
int currentPin = 0;                          // Активный PIN
bool blinking = false;                       // Флаг мигания
bool PinDisplayState = false; 




void handle_incoming_message(const String& topic, const String& payload) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(payload);

  if (topic == MQTT_TOPIC_LED_CONTROL) {
    if (payload == "on") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON");
    } else if (payload == "off") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");
    }
  } 
  else if (topic == MQTT_TOPIC_QTY) {
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
void parse_form(String input, int qty[QTY_COUNT]) {
  for (int i = 1; i <= QTY_COUNT; i++) {
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
  int qty[QTY_COUNT] = {0};

  parse_form(payload, qty);  // заполняем массив

  Serial.print("PIN = ");
  Serial.println(pin);

  for (int i = 0; i < QTY_COUNT; i++) {
    Serial.print("qty[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(qty[i]);
  }
  if (qty[0]==0){
    if (qty[1] == 1) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON");
    } else if (qty[1] == 0) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");
    }
  }
}

// =============================
// Обработка пина
// =============================

// ======================================================
// Генерация нового PIN
// ======================================================
void generateNewPin() {
  currentPin = random(1000, 9998);
  pinStartTime = millis();
  blinking = false;
  PinDisplayState = false;
  PinDisplaySet(currentPin);
  Serial.print("Новый PIN: ");
  Serial.println(currentPin);
}

// ======================================================
// Проверка срока действия PIN
// ======================================================
void checkPinTimeout() {
  unsigned long elapsed = millis() - pinStartTime;

  // Включаем мигание за 10 секунд до конца
  if (elapsed >= (PIN_LIFETIME - WARNING_TIME) && elapsed < PIN_LIFETIME) {
    blinking = true;
  }

  // Обновляем PIN после 60 секунд
  if (elapsed >= PIN_LIFETIME) {
    Serial.println("PIN истёк, обновляю...");
    generateNewPin();
  }

  // Если активировано мигание — моргаем LED
  if (blinking) {
    unsigned long now = millis();
    if (now - lastBlinkTime >= BLINK_INTERVAL) {
      PinDisplayState = !PinDisplayState;
      if (PinDisplayState){
        PinDisplaySet(currentPin);
      }
      else{
        PinDisplayOff();
      }
      lastBlinkTime = now;
    }
  }
}



bool isPinValid(int enteredPin) {
  // Проверяем, не истёк ли срок действия пина
  unsigned long elapsed = millis() - pinStartTime;
  if (elapsed >= PIN_LIFETIME) {
    Serial.println("PIN истёк — недействителен.");
    return false;
  }

  // Сравниваем введённый PIN с текущим
  if (enteredPin == currentPin) {
    Serial.println("PIN совпадает — доступ разрешён!");
    return true;
  } else {
    Serial.println("PIN не совпадает — отказано.");
    return false;
  }
}
