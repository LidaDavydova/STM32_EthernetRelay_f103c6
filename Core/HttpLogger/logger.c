/*
 * logger.c
 *
 *  Created on: Aug 22, 2025
 *      Author: hedg
 */

#include "logger.h"
#include "socket.h"
#include "wizchip_conf.h"
#include "string.h"
#include "stdio.h"

#define LOG_BUFFER_SIZE 512   /* было 3000, но обычно хватает 256-512 */
#define MAX_RETRIES     3
#define LOG_SOCKET      7

static uint8_t log_server_ip[4];
static uint16_t log_server_port;
static uint8_t log_initialized = 1;
static char buffer[LOG_BUFFER_SIZE];

/* Общая функция соединения */
static int8_t connect_to_server(void) {
    close(LOG_SOCKET);

    if (socket(LOG_SOCKET, Sn_MR_TCP, 0, 0) != LOG_SOCKET) {
        return -1;
    }

    if (connect(LOG_SOCKET, log_server_ip, log_server_port) != SOCK_OK) {
        close(LOG_SOCKET);
        return -1;
    }
    return 0;
}

/* Внутренняя универсальная функция */
void log_message_internal(LogLevel level, const char *format, ...) {
    if (!log_initialized) return;

    static const char *level_str[] = {"DEBUG", "INFO", "WARN", "ERROR"};
    va_list args;

    va_start(args, format);
    int len = snprintf(buffer, sizeof(buffer), "[%s] ", level_str[level]);
    len += vsnprintf(buffer + len, sizeof(buffer) - len, format, args);
    va_end(args);

    if (len < (int)sizeof(buffer) - 2) {
        if (buffer[len-1] != '\n') {
            buffer[len++] = '\r';
            buffer[len++] = '\n';
            buffer[len] = '\0';
        }
    }

    for (uint8_t attempt = 0; attempt < MAX_RETRIES; attempt++) {
        if (getSn_SR(LOG_SOCKET) != SOCK_ESTABLISHED) {
            if (connect_to_server() != 0) {
                HAL_Delay(50);
                continue;
            }
        }

        uint16_t sent = send(LOG_SOCKET, (uint8_t*)buffer, len);
        if (sent == len) break;

        HAL_Delay(50);
    }
}

/* printf без уровня */
void log_printf(const char *format, ...) {
    if (!log_initialized) return;

    va_list args;
    va_start(args, format);
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    if (len > 0) {
        for (uint8_t attempt = 0; attempt < MAX_RETRIES; attempt++) {
            if (getSn_SR(LOG_SOCKET) != SOCK_ESTABLISHED) {
                if (connect_to_server() != 0) {
                    HAL_Delay(50);
                    continue;
                }
            }
            uint16_t sent = send(LOG_SOCKET, (uint8_t*)buffer, len);
            if (sent == len) break;
            HAL_Delay(50);
        }
    }
}

/* Проверка и авто-реконнект */
void Logger_Process(void) {
    if (getSn_SR(LOG_SOCKET) == SOCK_CLOSED) {
        connect_to_server();
    }
}

void Logger_SetServer(uint8_t *server_ip, uint16_t port) {
    memcpy(log_server_ip, server_ip, 4);
    log_server_port = port;
}
