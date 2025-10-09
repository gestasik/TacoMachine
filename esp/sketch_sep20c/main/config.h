#pragma once
#include <Arduino.h>

// ====== Настройки устройства ======
#define DISPLAY_PIN 1            // Пин для подключения диода
#define LED_PIN 2                // Пин для светодиода
#define BUILTIN_LED 2            // Он же :)
#define PIN_LIFETIME 60000       // Срок действия пина (мс) — 60 секунд
#define WARNING_TIME 10000       // За сколько до истечения начинать мигать (мс)
#define BLINK_INTERVAL 500       // Интервал мигания (мс)

// ====== Настройки MQTT ======
#define mqtt_server "a81e525ac8434ca0b03de8585d2b9e19.s1.eu.hivemq.cloud"  // или свой адрес брокера
#define mqtt_port 8883
#define mqttUsr "gestasik"
#define mqttPass "Gestasik2343"
#define MQTT_TOPIC_LED_CONTROL "/miptfab/esp32led/ledControl/"
#define MQTT_TOPIC_LED_STATE "esp32/ledState"
#define MQTT_TOPIC_QTY "esp32/qty"
#define insecureMode true

// ======= Настройки WiFI =======
#define ssid "D3F"
#define password "FabMIPT2022"

// ====== Прочее ======

#define QTY_COUNT 8            // Количество товаров