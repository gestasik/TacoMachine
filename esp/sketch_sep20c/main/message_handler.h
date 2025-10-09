#pragma once
#include <Arduino.h>
#include "config.h"
// Обработка входящих MQTT сообщений
void handle_incoming_message(const String& topic, const String& payload);

// Обёртка для PubSubClient callback
void mqtt_callback(char* topic, byte* payload, unsigned int length);

// Работа с пином
int handle_pin(const long pin);
int parse_pin(String input);

// Парсинг формы (qty1..qty8)
void parse_form(String input, int qty[QTY_COUNT]);

// Основная функция обработки формы
void handle_form_message(const String& payload);

// Обработка пина
void generateNewPin();
void checkPinTimeout();
bool isPinValid(int enteredPin);