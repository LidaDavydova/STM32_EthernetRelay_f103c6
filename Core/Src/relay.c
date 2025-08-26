/*
 * relay.c
 *
 *  Created on: Aug 6, 2025
 *      Author: hedg
 */


#include "relay.h"
#include "stm32f1xx_hal.h"

// Используем, например, PB0 для реле
#define RELAY_GPIO_PORT GPIOB
#define RELAY_PIN       GPIO_PIN_9

static uint8_t relay_state = 0;

void Relay_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = RELAY_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(RELAY_GPIO_PORT, &GPIO_InitStruct);

    Relay_Off();
}

void Relay_On(void)
{
    HAL_GPIO_WritePin(RELAY_GPIO_PORT, RELAY_PIN, GPIO_PIN_SET);
    relay_state = 1;
}

void Relay_Off(void)
{
    HAL_GPIO_WritePin(RELAY_GPIO_PORT, RELAY_PIN, GPIO_PIN_RESET);
    relay_state = 0;
}

uint8_t Relay_GetState(void)
{
    return relay_state;
}
