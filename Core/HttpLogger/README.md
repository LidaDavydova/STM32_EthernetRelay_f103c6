# Логгер для STM32 (W5500 / TCP)

Этот модуль позволяет удобно логировать сообщения с разных уровней (`DEBUG`, `INFO`, `WARN`, `ERROR`) и отправлять их по TCP на удалённый сервер.  

---

## Возможности

- Отправка логов на TCP-сервер.
- Поддержка уровней логирования:
  - `DEBUG` (по умолчанию выключен).
  - `INFO`.
  - `WARN`.
  - `ERROR`.
- Автовосстановление соединения при обрыве.
- Минимальное потребление RAM (буфер 256 байт).
- Гибкая настройка уровней через `logger.h`.

---

## Макросы логирования

```c
LOGD("Отладка: значение = %d", val);   // DEBUG
LOGI("Событие: %s", name);             // INFO
LOGW("Предупреждение: %d", code);      // WARN
LOGE("Ошибка: %d", err);               // ERROR

Они зависят от конфигурации в logger.h:

```c
#define LOG_LEVEL_DEBUG  0   /* DEBUG выключен */
#define LOG_LEVEL_INFO   1   /* INFO включен */
#define LOG_LEVEL_WARN   1   /* WARN включен */
#define LOG_LEVEL_ERROR  1   /* ERROR включен */

! Если уровень отключён — макрос не занимает место в бинарнике.



## Пример сервера для логов

```python
import socket

HOST = "0.0.0.0"
PORT = 8080

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(1)
    print(f"Listening on {HOST}:{PORT}")
    conn, addr = s.accept()
    with conn:
        print("Connected by", addr)
        while True:
            data = conn.recv(1024)
            if not data:
                break
            print(data.decode("utf-8"), end="")

## Ограничения

* Максимальный размер одного сообщения: 256 байт.

* При большом потоке сообщений часть может потеряться.

* Требуется TCP-сервер для приёма логов.