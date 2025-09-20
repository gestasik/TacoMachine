// ИНЦИАЛИЗАЦИЯ =========================================================


// Подключение к MQTT
const client = mqtt.connect(MQTT_CONFIG.host, {
  username: MQTT_CONFIG.username,
  password: MQTT_CONFIG.password
});

// Подключение к Firebase
// firebaseConfig теперь берётся из config.js
const app = firebase.initializeApp(firebaseConfig);
const analytics = firebase.analytics(app);


// ФРОНТЕНД ====================================================================

// Имитация проверки соединения (можно заменить на реальную логику)
function updateConnectionStatus(connected) {
	const dot = document.getElementById('status-dot');
	const text = document.getElementById('status-text');
	if (connected) {
		dot.classList.add('connected');
		dot.classList.remove('disconnected');
		text.textContent = 'соединено';
		text.style.color = '#388e3c';
	} else {
		dot.classList.add('disconnected');
		dot.classList.remove('connected');
		text.textContent = 'нет соединения';
		text.style.color = '#b71c1c';
	}
}


client.on("connect", function() {
	const dot = document.getElementById('status-dot');
	const text = document.getElementById('status-text');
  dot.classList.add('connected');
	dot.classList.remove('disconnected');
	text.textContent = 'соединено';
	text.style.color = '#388e3c';
  console.log("Connected to MQTT broker");
});
client.on("error", function(err) {
	const dot = document.getElementById('status-dot');
	const text = document.getElementById('status-text');
	dot.classList.add('disconnected');
	dot.classList.remove('connected');
	text.textContent = 'нет соединения';
	text.style.color = '#b71c1c';
  console.log("Connection error: " + err.message);
});


// Пример: случайное соединение при загрузке
document.addEventListener('DOMContentLoaded', function() {
	// ...существующий код...

	

	// Валидация формы и вывод ошибок
	const form = document.querySelector('form');
	const errorDiv = document.getElementById('error-message');
	form.addEventListener('submit', function(e) {
    // Статус соединения
    function isPINcorrect(PIN) {
    // Здесь должна быть реальная логика проверки PIN
      console.log('пин', PIN, 'прошёл проверку' );
      return true; // Заглушка, всегда возвращает true
    }
		e.preventDefault();
		errorDiv.textContent = '';
		let hasError = false;
		const pin = form.elements['pin'].value.trim();
		if (!/^\d{4}$/.test(pin) || Number(pin) < 1000 || Number(pin) > 9999) {
			errorDiv.textContent = 'PIN должен быть числом от 1000 до 9999';
			hasError = true;
		}
		// Собираем все qty
		let quantities = {};
		for (let i = 1; i <= 8; i++) {
			let val = form.elements['qty' + i]?.value;
			quantities['qty' + i] = val !== undefined ? Number(val) : null;
		}
		// Если есть ошибки, не отправляем
		if (hasError) {
			return;
		}
		// Собираем все данные в один JSON
		const data = {
			...quantities
		};
		// Можно отправить на сервер или сохранить как файл
		// Для примера: выводим в консоль и предотвращаем отправку
    if(!isPINcorrect(pin)) {
      errorDiv.style.color = '#c22121ff';
      errorDiv.textContent = 'Неверный PIN';
      return;
    }
    else{ 
      const payload =  JSON.stringify(data, null, 2)
      console.log('Данные формы:', payload);
      client.publish("esp32/qty", payload);
      console.log('Данные отправлены на ESP32');
      e.preventDefault();
		  errorDiv.style.color = '#388e3c';
		  errorDiv.textContent = 'Данные собраны в JSON (см. консоль)';

    }
		
		setTimeout(()=>{errorDiv.textContent=''; errorDiv.style.color='#e53935';}, 2000);
	});
});

// Логика для кнопок + и - и ограничения ввода
document.addEventListener('DOMContentLoaded', function() {
	document.querySelectorAll('.quantity-control').forEach(function(control) {
		const input = control.querySelector('.qty-input');
		const btns = control.querySelectorAll('.qty-btn');
		btns.forEach(function(btn) {
			btn.addEventListener('click', function() {
				let value = parseInt(input.value) || 0;
				if (btn.dataset.action === 'increment') {
					if (value < 4) input.value = value + 1;
				} else {
					if (value > 0) input.value = value - 1;
				}
			});
		});
		input.addEventListener('input', function() {
			let value = parseInt(input.value) || 0;
			if (value < 0) input.value = 0;
			if (value > 4) input.value = 4;
		});
	});
});



// ОТРАВКА ДАННЫХ НА ESP32 ====================================================

/*
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

 */

// Закрытие соединения с HiveMQ при уходе со страницы
window.addEventListener("beforeunload", function () {
    client.end();
});