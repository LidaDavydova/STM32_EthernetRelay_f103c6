/*
 * config.c
 *
 *  Created on: Aug 26, 2025
 *      Author: hedg
 */


#include "config.h"
#include <string.h>
#include <stdlib.h>
#include "stm32f1xx_hal.h"

Config_t device_config = {
	.mac  = {0x00, 0x08, 0xDC, 0x56, 0x78, 0x23},
    .ip = {192, 168, 10, 3},
    .sn = {255, 255, 255, 0},
    .gw = {192, 168, 10, 1},
	.dns  = {8, 8, 8, 8},
	.dhcp = NETINFO_STATIC,
	.logger_server_ip = {192, 168, 10, 2},
	.logger_server_port = 12345,
	.camera_ip = {192, 168, 10, 2},
	.camera_port = 5000,
	.camera_url = "/\0",
	.camera_login = "admin\0",
	.camera_password = "sdfSDF543\0",
    .relay_enabled = 1,
    .relay_timer = 0,
    .function_select = 0
};


#define FLASH_CONFIG_START_ADDR 0x08007C00  // Например, предпоследняя страница флеша (уточним ниже)
#define FLASH_PAGE_SIZE 1024

void Flash_Write_Config(Config_t* config) {
    uint32_t *flash_ptr = (uint32_t*)FLASH_CONFIG_START_ADDR;
    uint32_t buffer[FLASH_PAGE_SIZE / 4]; // буфер для страницы

    // Считать текущую страницу во временный буфер
    memcpy(buffer, (uint32_t*)FLASH_CONFIG_START_ADDR, FLASH_PAGE_SIZE);

    // Заменить начало буфера на данные конфига (с учётом выравнивания)
    memcpy(buffer, config, sizeof(Config_t));

    HAL_FLASH_Unlock();

    // Стереть страницу
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_CONFIG_START_ADDR;
    EraseInitStruct.NbPages = 1;

    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK) {
        // Обработка ошибки стирания
        HAL_FLASH_Lock();
        return;
    }

    // Записать данные странично
    for (uint32_t i = 0; i < (FLASH_PAGE_SIZE / 4); i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(FLASH_CONFIG_START_ADDR + i*4), buffer[i]) != HAL_OK) {
            // Ошибка записи
            HAL_FLASH_Lock();
            return;
        }
    }

    HAL_FLASH_Lock();
}

