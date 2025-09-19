// Подключение к MQTT, конфиг берём из config.js
const client = mqtt.connect(MQTT_CONFIG.host, {
  username: MQTT_CONFIG.username,
  password: MQTT_CONFIG.password
});

client.on("connect", function() {
  document.getElementById("status").innerText = "✅ Подключено к MQTT";
});

client.on("error", function(err) {
  document.getElementById("status").innerText = "❌ Ошибка: " + err.message;
});

// Обработчик формы
document.getElementById("controlForm").addEventListener("submit", function(e) {
  e.preventDefault();

  const name = document.getElementById("name").value;
  const command = document.getElementById("command").value;

  const payload = JSON.stringify({
    user: name,
    cmd: command
  });

  client.publish("esp32/form", payload);

  document.getElementById("status").innerText = "📤 Отправлено: " + payload;
});
