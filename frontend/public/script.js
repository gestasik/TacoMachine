// Подключение к MQTT, конфиг берём из config.js
const client = mqtt.connect(MQTT_CONFIG.host, {
  username: MQTT_CONFIG.username,
  password: MQTT_CONFIG.password
});



// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyAQXonpGaA65D_tVVKGl1L0xyRH-MJig8w",
  authDomain: "tacomachine-mipt.firebaseapp.com",
  projectId: "tacomachine-mipt",
  storageBucket: "tacomachine-mipt.firebasestorage.app",
  messagingSenderId: "541785862522",
  appId: "1:541785862522:web:503cc73a6fd566b50326c3",
  measurementId: "G-5CP1YVZPMF"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);
const analytics = getAnalytics(app);






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


window.addEventListener("beforeunload", function () {
    client.end();  // закрывает MQTT-сессию
});