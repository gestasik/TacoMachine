
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

// Данные Wi-Fi
const char* ssid = "D3F";
const char* password = "FabMIPT2022";

// Данные MQTT брокера (HiveMQ Cloud)
const char* mqtt_server = "a81e525ac8434ca0b03de8585d2b9e19.s1.eu.hivemq.cloud";
const int mqtt_port = 8883;
const char* mqttUsr = "gestasik";
const char* mqttPass = "Gestasik2343";

// SSL клиент и MQTT клиент
WiFiClientSecure espClient;
PubSubClient client(espClient);

// Встроенный светодиод
const int BUILTIN_LED = 2;

// Буферы
String inmsg = "";
long lastMsg = 0;

// Для теста можно временно отключить проверку сертификата
bool insecureMode = true;

void setup_wifi() {
  if(WiFi.status() == WL_CONNECTED) return;

  Serial.print("Connecting to Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retries++;
    if(retries > 60){ // через 30 секунд перезагружаем ESP
      Serial.println("\nFailed to connect to Wi-Fi. Restarting...");
      ESP.restart();
    }
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Callback для входящих MQTT сообщений
void callback(char* topic, byte* payload, unsigned int length) {
  inmsg = "";
  for (int i = 0; i < length; i++) inmsg += (char)payload[i];
  
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]: ");
  Serial.println(inmsg);

  if (inmsg == "off") {
    digitalWrite(BUILTIN_LED, LOW);
  } else if (inmsg == "on") {
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

// Подключение к MQTT
void reconnectMQTT() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-" + String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(), mqttUsr, mqttPass)) {
      Serial.println("connected");
      client.publish("/miptfab/esp32led/ledState/", "connected");
      client.subscribe("/miptfab/esp32led/ledControl/");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retry in 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  setup_wifi();

  if(insecureMode){
    espClient.setInsecure(); // ⚠️ отключает проверку SSL для теста
  } 
  // Если хочешь использовать сертификат CA, вместо setInsecure() раскомментируй:
  // espClient.setCACert(root_ca);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  // Если Wi-Fi отключился — переподключаемся
  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Wi-Fi lost, reconnecting...");
    setup_wifi();
  }

  // Если MQTT не подключён — переподключаемся
  if(!client.connected()){
    reconnectMQTT();
  }

  client.loop();
}