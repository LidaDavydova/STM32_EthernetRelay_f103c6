/*
 * logger.h
 *
 *  Created on: Aug 22, 2025
 *      Author: hedg
 */

#pragma once

#include <stdint.h>
#include <stdarg.h>

/* Уровни логирования */
typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR
} LogLevel;

/* Включение/выключение уровней на этапе компиляции */
#ifndef LOG_ENABLE
#define LOG_ENABLE 1
#endif

#ifndef LOG_LEVEL_DEBUG_ENABLE
#define LOG_LEVEL_DEBUG_ENABLE 1   /* По умолчанию включён */
#endif

#ifndef LOG_LEVEL_INFO_ENABLE
#define LOG_LEVEL_INFO_ENABLE 1
#endif

#ifndef LOG_LEVEL_WARN_ENABLE
#define LOG_LEVEL_WARN_ENABLE 1
#endif

#ifndef LOG_LEVEL_ERROR_ENABLE
#define LOG_LEVEL_ERROR_ENABLE 1
#endif

/* Макросы для логирования */
#if LOG_ENABLE && LOG_LEVEL_DEBUG_ENABLE
#define LOGD(fmt, ...) log_message_internal(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define LOGD(...) ((void)0)
#endif

#if LOG_ENABLE && LOG_LEVEL_INFO_ENABLE
#define LOGI(fmt, ...) log_message_internal(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#else
#define LOGI(...) ((void)0)
#endif

#if LOG_ENABLE && LOG_LEVEL_WARN_ENABLE
#define LOGW(fmt, ...) log_message_internal(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#else
#define LOGW(...) ((void)0)
#endif

#if LOG_ENABLE && LOG_LEVEL_ERROR_ENABLE
#define LOGE(fmt, ...) log_message_internal(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#else
#define LOGE(...) ((void)0)
#endif

/* API */
void Logger_SetServer(uint8_t *server_ip, uint16_t port);
void Logger_Process(void);
void log_message_internal(LogLevel level, const char *format, ...);
void log_printf(const char *format, ...);

