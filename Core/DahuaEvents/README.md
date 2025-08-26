# DahuaEvents — Digest Auth подписка на события Dahua IP-камеры

Этот модуль реализует подписку на события IP-камер Dahua через их HTTP API с авторизацией по Digest Auth.  
STM32 с Ethernet-чипом (W5500) подключается к камере, обрабатывает multipart JSON stream и извлекает интересующие события (например, номера автомобилей из WhiteList).

---

## Основные файлы

- `http_digest.c` — реализация HTTP-клиента с Digest авторизацией и парсером событий.  
- `http_digest.h` — заголовочный файл (типы, объявления функций).  
- `digest.h` / `client.h` — вспомогательные библиотеки для Digest-аутентификации: генерация заголовков, разбор WWW-Authenticate.  
- `logger.h` / `relay.h` — логирование и управление реле при срабатывании события.
- `server.c` - серверная часть Digest (генерация WWW-Authenticate, поддержка nonce, qop, opaque и пр.).

---

## Принцип работы

1. STM32 открывает TCP-сокет на камеру Dahua.  
2. Отправляет GET-запрос на URL событий.  
3. Если камера отвечает `401 Unauthorized`, извлекаются параметры Digest (`realm`, `nonce`, `opaque`, `qop`, `algorithm`) и формируется заголовок Authorization.  
4. После успешной авторизации камера начинает передавать поток `multipart/mixed` с JSON-событиями.  
5. Модуль построчно парсит поток и ищет ключи:  
   - `"WhiteList"`  
   - `"PlateNumber"`  
6. При обнаружении `"PlateNumber"` внутри `"WhiteList"` — логирует событие и вызывает `Relay_On()`.

---

## Настраиваемые параметры

| Параметр           | Где задаётся                                  | Описание                                    |
|--------------------|----------------------------------------------|---------------------------------------------|
| IP камеры          | аргумент `http_digest_init(uint8_t *ip, ...)`| 4-байтовый массив с IP, например `{192,168,1,108}` |
| Порт камеры        | `http_digest_init(..., uint16_t port, ...)`  | Обычно 80 или 8080                           |
| URL событий        | `http_digest_init(..., const char *url, ...)`| Путь к API, например: `"/cgi-bin/eventManager.cgi?action=attach&codes=[All]"` |
| Имя пользователя   | `http_digest_init(..., const char *user, ...)` | Логин администратора камеры                  |
| Пароль             | `http_digest_init(..., const char *pass, ...)` | Пароль администратора                         |
| Сокет W5500        | `http_digest_init(uint8_t sock, ...)`         | Номер сокета (0–7)                            |
| Буфер приёма       | `HTTP_BUF_SIZE`                               | Размер временного буфера для HTTP-ответов    |
| Парсинг события    | `process_stream_chunk()`                       | Логика фильтрации по ключам `"WhiteList"` и `"PlateNumber"` |

---

## Использование

Иницилизация подключения:

``` C
CLIENT_SOCKET = 0;

device_config - структура, инициализация в /HttpConfig/config.c

http_digest_init(CLIENT_SOCKET, device_config.camera_ip, device_config.camera_port,
	device_config.camera_url,
	device_config.camera_login, device_config.camera_password);

```


В основном цикле main.c вызывается:

```

while (1) {
	http_digest_status_t st = http_digest_process();
	if (st != HTTP_DIGEST_OK) {
	// обработка ошибок / перезапуск
	}
}

```

При событии `"WhiteList + PlateNumber"` модуль автоматически:

- логирует номер машины,  
- вызывает `Relay_On()`.

---

## Логирование

Все этапы работы (CONNECT, AUTH, STREAM) логируются через `logger.h`.

Пример сообщений:

```
WhiteList Plate detected: A123BC
STREAM: peer closed connection
HTTP_STATE_CONNECT: sock connected
```

