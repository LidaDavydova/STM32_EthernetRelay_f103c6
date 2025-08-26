# STM32 + W5500 Ethernet Init

Этот модуль инициализирует Ethernet-чип W5500 по SPI с использованием HAL и библиотеки Wiznet.  
Позволяет задать MAC, IP-адрес, маску подсети, шлюз и DNS, а также получить текущие настройки.

## Файлы
- `wiznet_init.c` – реализация инициализации и API.
- `eth_w5500.h` – заголовок с объявлениями функций.
- `logger.h` – простой логгер (замените на свой вывод через UART/SWO и т.п.).

## Возможности
- Сброс и инициализация чипа W5500
- Настройка сетевых параметров (MAC, IP, SN, GW, DNS)
- Обновление конфигурации в рантайме
- Вывод текущего состояния через логгер

## Требования
- STM32 HAL (SPI, GPIO, UART/HTTP для логов)
- WIZnet ioLibrary (wizchip_conf.h, socket.h и др.)
- Настроенный SPI (например, hspi1)
- Пины CS и RST прописаны в main.h



## Использование

В `main.c`:

```c
#include "eth_w5500.h"

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART1_UART_Init(); // для логов

    // Инициализация W5500
    ETH_W5500_Init();

    // Вывод текущих сетевых настроек
    ETH_W5500_PrintNetInfo();

    // Обновление сетевых параметров
    wiz_NetInfo newInfo = {
        .mac  = {0x00, 0x08, 0xDC, 0xAA, 0xBB, 0xCC},
        .ip   = {192, 168, 1, 50},
        .sn   = {255, 255, 255, 0},
        .gw   = {192, 168, 1, 1},
        .dns  = {8, 8, 8, 8},
        .dhcp = NETINFO_STATIC
    };
    ETH_W5500_SetNetInfo(&newInfo);

    while (1)
    {
        // Периодическая проверка состояния сети
        ETH_W5500_Tick();

        HAL_Delay(1000);
    }
}
```
