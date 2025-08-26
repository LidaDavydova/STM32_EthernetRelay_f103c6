/*
 * check_RAM_size.c
 *
 *  Created on: Aug 21, 2025
 *      Author: hedg
 */

#include "check_RAM_size.h"
#include <stdint.h>
#include "stm32f1xx_hal.h"

/* Конфигурация */
#define RAM_BASE 0x20000000U
#define MAX_TEST_KB 128U
#define KB_SIZE 1024U

uint8_t test_address(uint32_t addr) {
    volatile uint32_t *ptr = (volatile uint32_t *)addr;
    const uint32_t test_pattern = 0xA5A55A5A;
    uint32_t original_value = *ptr;

    // Простая запись и чтение
    *ptr = test_pattern;

    // Небольшая задержка вместо барьера
    for (volatile int i = 0; i < 10; i++);

    uint32_t read_back = *ptr;

    // Восстанавливаем значение если нужно
    if (read_back == test_pattern) {
        *ptr = original_value;
        return 1;
    }

    return 0;
}

void check_RAM(void)
{

    uint32_t last_valid_kb = 0;
    uint32_t detected_ram_size = 0;

    for (uint32_t kb = 0; kb < MAX_TEST_KB; kb++) {
        uint32_t addr = RAM_BASE + (kb * KB_SIZE);

        if (test_address(addr)) {
            last_valid_kb = kb;
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
			HAL_Delay(1000);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			HAL_Delay(1000);
        } else {
            break;
        }

        // Задержка между тестами
        for (volatile int i = 0; i < 50000; i++);
    }

    // Расчет итогового размера
    detected_ram_size = last_valid_kb + 1; // +1 потому что считаем с 0

}
